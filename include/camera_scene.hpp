#ifndef CAMERA_SCENE_H
#define CAMERA_SCENE_H

#include <graphics_scene.hpp>
#include <camera.hpp>

class CameraScene : public GraphicsScene {
public:
    CameraScene(const char* title, int width, int height):camera(glm::vec3(0.0f, 0.0f, 3.0f)),GraphicsScene(title, width, height){
        // camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        deltaTime = 0.0f;
        lastFrame = 0.0f;
        lastX = screen_width / 2;
        lastY = screen_height / 2;
        firstMouse = true;
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glEnable(GL_DEPTH_TEST);
    }
    virtual ~CameraScene() {}
protected:
    Camera camera;

    virtual void processInput() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        GraphicsScene::processInput();
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera.ProcessKeyboard(DOWN, deltaTime);
        }
    }
private:
    float deltaTime;
    float lastFrame;
    float lastX;
    float lastY;
    bool firstMouse;



    static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        CameraScene* scene = static_cast<CameraScene*>(glfwGetWindowUserPointer(window));
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if(scene->firstMouse) {
                scene->lastX = xpos;
                scene->lastY = ypos;
                scene->firstMouse = false;
            }

            float x_offset = xpos - scene->lastX;
            float y_offset = scene->lastY - ypos;

            scene->lastX = xpos;
            scene->lastY = ypos;

            scene->camera.ProcessMouseMovement(-x_offset, -y_offset);
        }
        else {
            scene->firstMouse = true;
        }
    }
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        CameraScene* scene = static_cast<CameraScene*>(glfwGetWindowUserPointer(window));
        scene->camera.ProcessMouseScroll(yoffset);
    }
};

#endif
