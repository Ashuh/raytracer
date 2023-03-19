#ifndef RAYTRACER_RENDER_MANAGER_H
#define RAYTRACER_RENDER_MANAGER_H

#include "gui.h"
#include "gui_listener.h"
#include <memory>

class RenderManager : public GuiListener {
private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<HittableList> scene;
    std::shared_ptr<Gui> gui;

    std::atomic_int numSamplesRequired = 1;
    bool hasWork = true;
    bool isExiting = false;

    std::condition_variable cond;
    std::mutex mutex;

    std::thread thread = std::thread([this]() {
        while (!isExiting) {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock, [this] { return hasWork || isExiting; });
            lock.unlock();

            if (isExiting) {
                break;
            }

            auto img = renderer->render(*camera, *scene);

            if (!img) {
                continue;
            }

            gui->setImage(img);

            if (renderer->getSamplesAccumulated() >= numSamplesRequired) {
                lock.lock();
                hasWork = false;
                lock.unlock();
            }
        }
    });

    void beginRendering() {
        std::unique_lock<std::mutex> lock(mutex);
        hasWork = true;
        lock.unlock();
        cond.notify_one();
    }

    void stopRendering() {
        std::unique_lock<std::mutex> lock(mutex);
        hasWork = false;
        lock.unlock();
        cond.notify_one();
    }

    void shutdown() {
        std::unique_lock<std::mutex> lock(mutex);
        hasWork = false;
        isExiting = true;
        lock.unlock();
        cond.notify_all();
        if (thread.joinable()) {
            thread.join();
        }
    }

public:
    RenderManager(const shared_ptr<Renderer> &renderer,
                  const shared_ptr<Camera> &camera,
                  const shared_ptr<HittableList> &scene,
                  const shared_ptr<Gui> &gui) : renderer(renderer),
                                                camera(camera),
                                                scene(scene),
                                                gui(gui) {
        gui->setNumSamples(numSamplesRequired);
        gui->setMaxDepth(renderer->getMaxDepth());
        gui->setLensRadius(camera->getLensRadius());
    }

    void onWindowClosing() override {
        shutdown();
    }

    void onSamplesChanged(int value) override {
        if (renderer->getSamplesAccumulated() > value) {
            renderer->interrupt();
            stopRendering();
            renderer->reset();
            numSamplesRequired = value;
            beginRendering();
        } else if (renderer->getSamplesAccumulated() < value) {
            stopRendering();
            numSamplesRequired = value;
            beginRendering();
        } else {
            renderer->interrupt();
            stopRendering();
            numSamplesRequired = value;
        }
        gui->setNumSamples(value);
    }

    void onMaxDepthChanged(int value) override {
        renderer->interrupt();
        stopRendering();
        renderer->reset();
        renderer->setMaxDepth(value);
        beginRendering();
        gui->setMaxDepth(value);
    }

    void onLensRadiusChanged(double value) override {
        renderer->interrupt();
        stopRendering();
        renderer->reset();
        camera->setLensRadius(value);
        beginRendering();
        gui->setLensRadius(value);
    }
};

#endif//RAYTRACER_RENDER_MANAGER_H
