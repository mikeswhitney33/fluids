#ifndef CUBE_SCENE_H
#define CUBE_SCENE_H

#include <graphics_scene.hpp>
#include <stb_image.h>
#include <shader.hpp>
#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

static const glm::vec3 cubePositions[10] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

class CubeScene : public GraphicsScene {
public:
    CubeScene():GraphicsScene("Cube", 800, 600){}
    ~CubeScene() {delete shader;delete camera;}
private:
    void PreLoop() {

        glEnable(GL_DEPTH_TEST);
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        shader = new Shader("shaders/vertex/cube.vert", "shaders/fragment/cube.frag");
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        texture1 = read_texture("resources/container.jpg");
        texture2 = read_texture("resources/awesomeface.png");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        //texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        shader->use();
        shader->setInt("texture1", 0);
        shader->setInt("texture2", 1);

        deltaTime = 0.0f;
        lastFrame = 0.0f;
        lastX = screen_width / 2.0f;
        lastY = screen_height / 2.0f;
        firstMouse = true;

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }

    void MidLoop() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader->use();

        glm::mat4 model(1);
        glm::mat4 view(1);
        glm::mat4 projection(1);

        view = camera->GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float) screen_height, 0.1f, 100.0f);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        for(int i = 0;i < 10;i++) {
            glm::vec3 pos = cubePositions[i];
            glm::mat4 model(1);
            model = glm::translate(model, pos);
            float angle = 20.0f * i;
            if(i % 3 == 0) {
                angle += 30 * glfwGetTime();
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void PostLoop(GLFWwindow* window) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int texture1;
    unsigned int texture2;

    Shader* shader;
    Camera* camera;

    float deltaTime;
    float lastFrame;
    float lastX;
    float lastY;
    bool firstMouse;

    void processInput() {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera->ProcessKeyboard(FORWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera->ProcessKeyboard(LEFT, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera->ProcessKeyboard(RIGHT, deltaTime);
        }
    }
    unsigned int read_texture(const std::string &name, bool flip = true) {
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
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        CubeScene* scene = static_cast<CubeScene*>(glfwGetWindowUserPointer(window));
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

            scene->camera->ProcessMouseMovement(-x_offset, -y_offset);
        }
        else {
            scene->firstMouse = true;
        }
    }
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        CubeScene* scene = static_cast<CubeScene*>(glfwGetWindowUserPointer(window));
        scene->camera->ProcessMouseScroll(yoffset);
    }
};

#endif


int main(int argc, char** argv) {
    GraphicsScene* cubescene = new CubeScene();
    cubescene->Render();
    delete cubescene;

    return 0;
}
