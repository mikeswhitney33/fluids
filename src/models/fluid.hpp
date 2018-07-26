#ifndef FLUID_H
#define FLUID_H

#include "billboard.hpp"
#include <vector>
#include <map>
#include <glm/gtx/component_wise.hpp>

enum CellType {FLUID, SOLID, AIR};

glm::vec3 neighbor_offsets[6] = {
    glm::vec3(-1, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 0, -1),
    glm::vec3(0, 0, 1)
};

struct MarkerCell {
    glm::vec3 loc;
    glm::vec3 u;
    glm::vec3 u0;
    int layer;
    CellType type;
    MarkerCell(){}
};

float resolution = 100.0f;

class Fluid {
public:
    Fluid(const float &cfl, const int &numParticles, const int &num_frames=30, const float &h=1.0f): locations(num_frames), particle("resources/water.png", glm::vec2(0.05f, 0.05f)) {
        // for(int i = 0;i < numParticles;i++) {
        //     locations.push_back(glm::vec3(randf(-resolution, resolution), randf(-resolution, resolution), randf(-resolution, resolution)));
        // }
        k_cfl = cfl;
        height = h;
        max_frames = num_frames;
        current_frame = 0;
        num_particles = numParticles;
        simulate();
    }
    virtual ~Fluid(){}

    virtual void render(const glm::mat4 &projection, const glm::mat4 &view, const float &deltaTime) {
        for(int i = 0;i < locations[current_frame].size();i++) {
            particle.render(projection, view, locations[current_frame][i] / resolution);
        }
        current_frame = (current_frame + 1 >= max_frames)? 0 : current_frame + 1;
    }

private:
    unsigned int instanceVBO;
    Billboard particle;
    std::vector< std::vector<glm::vec3> > locations;
    float k_cfl;
    float height;
    int num_particles;
    int current_frame;
    int max_frames;

    std::map<int, MarkerCell> grid;

    int hash(const glm::vec3 &p) const {
        return 541 * (int)p.x + 79 * (int)p.y + 31 * (int)p.z;
    }

    bool inGrid(const int &p_hash, MarkerCell &C) {
        if(grid.find(p_hash) != grid.end()) {
            C = grid[p_hash];
            return true;
        }
        return false;
    }

    bool inBounds(const glm::vec3 &p) const {
        return p.x > -resolution && p.x < resolution &&
            p.y > -resolution && p.y < resolution &&
            p.z> -resolution && p.z < resolution;
    }

    glm::vec3 getNeighbor(const glm::vec3 &p, const int &i) const {
        return p + neighbor_offsets[i];
    }

    float u_max() {
        float max_val = -999999.0f;
        for (auto& iter : grid) {
            if(glm::compMax(iter.second.u) > max_val) {
                max_val = glm::compMax(iter.second.u);
            }
        }
        return (max_val == 0) ? 1.0f : max_val;
    }

    void simulate() {
        std::vector<glm::vec3> current_particles(num_particles);
        for(int i = 0;i < num_particles;i++) {
            current_particles[i] = glm::vec3(randf(-resolution, resolution), randf(-resolution, resolution), randf(-resolution, resolution));
        }
        locations[0] = current_particles;
        for(int i = 1;i < max_frames;i++) {
            std::cout << i << ": Stepping: " << grid.size() << std::endl;
            step(1.0f / 60.0f, current_particles);
            locations[i] = current_particles;
        }
    }

    void step(const float &deltaTime, std::vector<glm::vec3> &particles) {
        float dt = k_cfl * (height / fabs(u_max()));
        updateGrid(particles);
    }

    void advanceVelocityField() {
        applyConvection();
    }

    void applyConvection() {

    }

    glm::vec3 traceParticle(glm::vec3 particle, float dt) {
        glm::vec3 V = getVelocity(particle);
        V = getVelocity(glm::vec3(particle.x + 0.5f * dt * V.x, particle.y + 0.5f * dt * V.y, particle.z + 0.5f * dt * V.z));
        return particle + dt * V;
    }

    glm::vec3 getVelocity(glm::vec3 particle) {
        glm::vec3 v;
        v.x = getInterpolatedValue(particle.x / height, particle.y / height - 0.5f, particle.z / height - 0.5f, 0);
        v.y = getInterpolatedValue(particle.x / height - 0.5f, particle.y / height, particle.z / height - 0.5f, 1);
        v.z = getInterpolatedValue(particle.x / height - 0.5f, particle.y / height - 0.5f, particle.z / height, 2);
        return v;
    }

    void updateGrid(std::vector<glm::vec3> &particles) {
        for (auto& iter : grid) {
            iter.second.layer = -1;
        }
        for(int i = 0;i < particles.size();i++) {
            MarkerCell C;
            int p_hash = hash(particles[i]);
            if(!inGrid(p_hash, C)) {
                if(inBounds(particles[i])) {
                    C.loc = particles[i];
                    C.type = FLUID;
                    C.layer = 0;
                    grid[p_hash] = C;
                }
            }
            else if(C.type != SOLID) {
                C.loc = particles[i];
                C.type = FLUID;
                C.layer = 0;
                grid[p_hash] = C;
            }
        }

        for(int i = 1;i < std::max(2.0f, ceilf(k_cfl));i++) {
            for(auto& iter : grid) {
                if((iter.second.type == FLUID || iter.second.type == AIR) && iter.second.layer == i - 1) {
                    for(int n_i = 0;n_i < 6;n_i++) {
                        glm::vec3 n_loc = getNeighbor(iter.second.loc, n_i);
                        int n_hash = hash(n_loc);
                        MarkerCell N;
                        if(inGrid(n_hash, N)) {
                            if(N.layer == -1 && N.type != SOLID) {
                                N.type = AIR;
                                N.layer = i;
                                grid[n_hash] = N;
                            }
                        }
                        else {
                            N.loc = n_loc;
                            N.layer = i;
                            if(inBounds(n_loc)) {
                                N.type = AIR;
                            }
                            else {
                                N.type = SOLID;
                            }
                            grid[n_hash] = N;
                        }
                    }
                }
            }
        }
        for(auto iter = grid.begin();iter != grid.end();) {
            if(iter->second.layer == -1) {
                grid.erase(iter++);
            }
            else {
                ++iter;
            }
        }
    }
};

#endif
