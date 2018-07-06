#include <graphics_scene.hpp>


class EmptyScene : public GraphicsScene {
public:
    EmptyScene():GraphicsScene("Empty", 800, 600){}

protected:
    void MidLoop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput();
    }
private:
    void processInput() {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
};

int main(int argc, char** argv) {
    GraphicsScene* empty = new EmptyScene();
    empty->Render();
    delete empty;
    return 0;
}
