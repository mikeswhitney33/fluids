#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <graphics.hpp>

class RenderScene {
public:
    RenderScene(){}
    virtual void Render(GLFWwindow* window) = 0;
    virtual void AfterLoop(GLFWwindow* window) = 0;
};

#endif
