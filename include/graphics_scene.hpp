#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>


class GraphicsScene {
public:
    GraphicsScene(const char* title, int width, int height) {
        screen_width = width;
        screen_height = height;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

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

        glfwSetWindowUserPointer(window, this);

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
    int screen_width, screen_height;

private:
    virtual void resized(int w, int h) {
        screen_width = w;
        screen_height = h;
    }
    virtual void PreLoop(){};
    virtual void MidLoop(){};
    virtual void PostLoop(){};
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        GraphicsScene* gc = static_cast<GraphicsScene*>(glfwGetWindowUserPointer(window));
        gc->resized(width, height);
    }
};

#endif
