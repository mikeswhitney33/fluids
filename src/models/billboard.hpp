#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <shader.hpp>
#include <my_utils.hpp>


class Billboard {
public:
    Billboard(const char* texture_filename, const glm::vec2 &_size):shader("shaders/vertex/billboard.vert", "shaders/fragment/billboard.frag") {
        GLfloat vertices[] = {
            // Positions          // Texture Coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left
        };
        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        //texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        texture = read_texture(texture_filename);
        size = _size;
    }

    void render(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &pos) {
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        shader.setVec3("billboard_center", pos);
        shader.setMat4("view", view);
        shader.setVec2("billboard_size", size);

        shader.setMat4("projection", projection);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // uniform vec3 billboard_center;
        // uniform mat4 view;
        // uniform vec2 billboard_size;

    }

    unsigned int VAO;
private:
    Shader shader;

    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;
    glm::vec2 size;

};

#endif
