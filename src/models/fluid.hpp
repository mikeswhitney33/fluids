#ifndef FLUID_H
#define FLUID_H

#include "billboard.hpp"
#include <vector>

class Fluid {
public:
    Fluid(const int &numParticles): particle("resources/water.png", glm::vec2(0.05f, 0.05f)) {
        for(int i = 0;i < numParticles;i++) {
            locations.push_back(glm::vec3(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f)));
        }
    }
    virtual ~Fluid(){}

    virtual void render(const glm::mat4 projection, const glm::mat4 view) {
        for(int i = 0;i < locations.size();i++) {
            particle.render(projection, view, locations[i]);
        }
    }

private:
    unsigned int instanceVBO;
    Billboard particle;
    std::vector<glm::vec3> locations;
};

#endif
