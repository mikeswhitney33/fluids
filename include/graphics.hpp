#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>



void initialization() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* create_window(std::string title, int width, int height) {
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return window;
}

void load_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void make_adjustable_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

GLFWwindow* basic_setup(std::string title, int width, int height) {
    initialization();
    GLFWwindow* window = create_window(title, width, height);
    glfwMakeContextCurrent(window);
    load_glad();
    make_adjustable_viewport(window, width, height);
    return window;
}
