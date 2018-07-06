#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

class GraphicsScene {
public:
    GraphicsScene(const char* title, int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if(window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        glViewport(0, 0, width, height);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    void Render() {
        PreLoop();
        while(!glfwWindowShouldClose(window)) {
            MidLoop();

            // Check and call events and swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        PostLoop();
        glfwTerminate();
    }

protected:
    GLFWwindow* window;
    virtual void PreLoop(){};
    virtual void MidLoop(){};
    virtual void PostLoop(){};
private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};



#endif
