#ifndef TRIANGLE_SCENE_H
#define TRIANGLE_SCENE_H

/*
 * This Scene is an implementation of the Hello Triangle and Shader tutorials found at:
 * https://learnopengl.com/Getting-started/Hello-Triangle. and
 * https://learnopengl.com/Getting-started/Shaders
 */

#include <graphics_scene.hpp>
#include <shader.hpp>
#include <stb_image.h>
#include <iostream>

class TriangleScene : public GraphicsScene {
public:
    TriangleScene():GraphicsScene("Triangle", 800, 600){}
    ~TriangleScene() {
         delete shader;
    }

private:
    void PreLoop() {
        shader = new Shader("shaders/vertex/triangle.vert", "shaders/fragment/triangle.frag");

        float vertices[] = {
            -0.2f, -0.2f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.2f, -0.2f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
             0.0f,  0.2f, 0.0f, 1.0f, 1.0f, 0.0f,  0.5f, 1.0f
        };

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load("resources/wall.jpg", &width, &height, &nrChannels, 0);

        if(data) {
            if(nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else if(nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        stbi_image_free(data);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        //color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


    }

    void MidLoop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);


        float x_offsets[] = {0.0f, 0.2f, -0.2f};
        float y_offsets[] = {0.2f, -0.2f, -0.2f};

        for(int i = 0;i < 3;i++) {
            shader->setFloat("x_offset", x_offsets[i]);
            shader->setFloat("y_offset", y_offsets[i]);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

    void PostLoop(GLFWwindow* window) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    unsigned int VBO;
    unsigned int VAO;
    unsigned int texture;

    Shader* shader;
};

#endif

int main(int argc, char** argv) {
    GraphicsScene* triscene = new TriangleScene();
    triscene->Render();
    delete triscene;

    return 0;
}
