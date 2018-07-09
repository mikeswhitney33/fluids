#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stb_image.h>


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
            processInput();
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
    virtual void processInput() {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    static unsigned int read_texture(const std::string &name, bool flip = true) {
        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
        if(data) {
            if(nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else if(nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else {
                std::cout << "nrChannels: " << nrChannels << std::endl;
            }
        }
        else {
            std::cout << "Failed to load texture: " << name << std::endl;
        }
        stbi_image_free(data);
        return tex;
    }


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
