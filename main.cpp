#include <graphics.hpp>


class EmptyScene : public RenderScene {
public:
    EmptyScene():RenderScene() {}
    void Render(GLFWwindow* window) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    void processInput(GLFWwindow* window) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
};

int main(int argc, char** argv) {
    GLFWwindow* window = basic_setup("Empty Scene", 800, 600);
    RenderScene* empty = new EmptyScene();
    basic_loop(window, empty);
    delete empty;
    return 0;
}
