#ifndef RAYTRACER_GUI_H
#define RAYTRACER_GUI_H

#include <glad/glad.h>

#include "gui_listener.h"
#include "image.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <mutex>
#include <thread>

class Gui {
public:
    Gui();

    void run();

    static void newFrame();

    void update();

    static void render();

    void shutdown();

    void setImage(const std::shared_ptr<Image> &img) {
        std::lock_guard<std::mutex> lock(m);
        image = img;
    }

    std::shared_ptr<Image> getImage() {
        std::lock_guard<std::mutex> lock(m);
        return image;
    }

    [[nodiscard]] bool isClosing() const {
        return glfwWindowShouldClose(window);
    }

    void setListener(const std::shared_ptr<GuiListener> &listener) {
        guiListener = listener;
    }

private:
    std::shared_ptr<GuiListener> guiListener;
    GLFWwindow *window{};
    std::shared_ptr<Image> image;
    GLuint texture{};
    std::mutex m;

    std::atomic_int numSamples;
    std::atomic_int maxDepth;
    std::atomic<float> lensRadius;

public:
    void setNumSamples(int value);

    void setMaxDepth(int value);

    void setLensRadius(float value);

private:
    void init();

    [[nodiscard]] std::pair<int, int> getWindowSize() const {
        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);
        return {width, height};
    }
};

Gui::Gui() {
    init();
}

void Gui::init() {
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit() Error");
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Raytracer", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("window is null");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);// Enable vsync

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Unable to context to OpenGL");
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// Same
}

void Gui::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::update() {
    auto img = getImage();

    if (img != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
        auto [width, height] = getWindowSize();
        ImVec2 size(static_cast<float>(width), static_cast<float>(height));
        ImGui::GetBackgroundDrawList()->AddImage((void *) (intptr_t) texture, ImVec2(0, 0), size);
    }

    // render your GUI
    static int counter = 0;

    ImGui::Begin("Render Settings");
    ImGui::Text("This is some useful text.");

    int sliderNumSamples = numSamples;
    if (ImGui::SliderInt("Samples", &sliderNumSamples, 1, 20)) {
        std::thread t([this, sliderNumSamples]() {
            guiListener->onSamplesChanged(sliderNumSamples);
        });
        t.detach();
    }

    int sliderMaxDepth = maxDepth;
    if (ImGui::SliderInt("Max Depth", &sliderMaxDepth, 1, 20)) {
        std::thread t([this, sliderMaxDepth]() {
            guiListener->onMaxDepthChanged(sliderMaxDepth);
        });
        t.detach();
    }

    float sliderLensRadius = lensRadius;
    if (ImGui::SliderFloat("Lens Radius", &sliderLensRadius, 0, 1)) {
        std::thread t([this, sliderLensRadius]() {
            guiListener->onLensRadiusChanged(sliderLensRadius);
        });
        t.detach();
    }

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Gui::render() {
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::shutdown() {
    guiListener->onWindowClosing();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::run() {
    while (!isClosing()) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        newFrame();
        update();
        render();
        glfwSwapBuffers(window);
    }
    shutdown();
}

void Gui::setNumSamples(int value) {
    numSamples = value;
}

void Gui::setMaxDepth(int value) {
    maxDepth = value;
}

void Gui::setLensRadius(float value) {
    lensRadius = value;
}

#endif//RAYTRACER_GUI_H
