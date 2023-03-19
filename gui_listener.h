#ifndef RAYTRACER_GUI_LISTENER_H
#define RAYTRACER_GUI_LISTENER_H

class GuiListener {
public:
    virtual void onWindowClosing() = 0;
    virtual void onSamplesChanged(int value) = 0;
    virtual void onMaxDepthChanged(int value) = 0;
    virtual void onLensRadiusChanged(double value) = 0;
};

#endif//RAYTRACER_GUI_LISTENER_H
