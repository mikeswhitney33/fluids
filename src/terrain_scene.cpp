#ifndef TERRAIN_SCENE_H
#define TERRAIN_SCENE_H

#include <shader.hpp>
#include <camera_scene.hpp>
#include "models/random_terrain.hpp"
#include <vector>
#include <time.h>


class TerrainScene : public CameraScene {
public:
    TerrainScene(int _n, int range):CameraScene("Terrain", 800, 600){
        RandomTerrain* terrain = new RandomTerrain(_n, range);
        vertices = terrain->getVertsXYZN();
        indices = terrain->getFaces();
        min_val = terrain->getMinValue();
        max_val = terrain->getMaxValue();
        delete terrain;
        shader = new Shader("shaders/vertex/terrain.vert", "shaders/fragment/terrain.frag");
    }

    ~TerrainScene() {
        delete shader;
    }

private:
    std::vector<float> vertices;
    std::vector<int> indices;
    Shader* shader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    float min_val, max_val;

    void PreLoop() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

        //vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void MidLoop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        glBindVertexArray(VAO);

        glm::mat4 model(1);
        glm::mat4 view(1);
        glm::mat4 projection(1);

        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));

        view = camera->GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float) screen_height, 0.1f, 100.0f);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);

        shader->setFloat("min_val", min_val);
        shader->setFloat("max_val", max_val);
        shader->setVec3("viewPos", camera->Position);
        shader->setVec3("lightPos", glm::vec3(1.0f, 1.0f, 0.0f));
        shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void PostLoop() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};



#endif


int main() {
    srand(time(NULL));
    CameraScene* scene = new TerrainScene(8, 3);
    scene->Render();
    delete scene;
    return 0;
}
