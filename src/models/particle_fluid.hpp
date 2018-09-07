#ifndef PARTICLE_FLUID_H
#define PARTICLE_FLUID_H

#include <glm/glm.hpp>
#include <vector>
#include <my_utils.hpp>
#include <limits>

struct FluidParticle {
    glm::vec3 x;
    glm::vec3 v;
    float mass;
    float density;
    float pressure;
    // glm::vec3 color;
    glm::vec3 force;
    FluidParticle(const glm::vec3 &x_, const float &m):x(x_), mass(m){
        v = glm::vec3(0, 0, 0);
        density = 0.0f;
        pressure = 0.0f;
        force = glm::vec3(0,0,0);
    }
};

float getDensity(FluidParticle p){
    return p.density;
}

float getPressure(FluidParticle p) {
    return p.pressure;
}

class ParticleFluid {
public:
    ParticleFluid(const int &num_particles, const float &h, const float &k=1, const float &p_0=1, const float &mew_=1):
        particle("resources/water.png", glm::vec2(0.05f, 0.05f)),
        smoothing_width(h), gas_constant(k), rest_density(p_0), mew(mew_) {
        for(int i = 0;i < num_particles;i++) {
            particles.push_back(FluidParticle(glm::vec3(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f)), 1.0f));
        }
    }

    virtual void render(const glm::mat4 &projection, const glm::mat4 &view, const float &deltaTime) {
        step(deltaTime);
        for(int i = 0;i < particles.size();i++) {
            particle.render(projection, view, particles[i].x);
        }
    }
private:
    std::vector<FluidParticle> particles;
    float smoothing_width;
    Billboard particle;
    float t;
    float gas_constant;
    float rest_density;
    float mew;

    std::vector<int> getNeighbors(int j) {
        std::vector<int> neighbors;
        for(int i = 0;i < particles.size();i++) {
            if(i != j && glm::distance(particles[i].x, particles[j].x) < 2 * smoothing_width) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }

    void step(const float &deltaTime) {
        for(int j = 0;j < particles.size();j++) {

        }
    }

    float A_s(glm::vec3 r, float(*A)(FluidParticle), float(*W)(glm::vec3, float)) {
        float val = 0.0f;
        for(int j = 0;j < particles.size();j++) {
            val += particles[j].mass * A(particles[j]) / particles[j].density * W(r - particles[j].x, smoothing_width);
        }
        return val;
    }

    float P_s(glm::vec3 r, float (*W)(glm::vec3, float)) {
        float val = 0.0f;
        for(int j = 0;j < particles.size();j++) {
            val += particles[j].mass * W(r - particles[j].x, smoothing_width);
        }
        return val;
    }

    glm::vec3 f_pressure(const int &i, glm::vec3 (*gradient_W)(glm::vec3, float)) {
        glm::vec3 val = glm::vec3(0.0f, 0.0f, 0.0f);
        for(int j = 0;j < particles.size();j++) {
            val += particles[j].mass  * (p(i) + p(j)) / (2 * particles[j].density) * gradient_W(particles[i].x - particles[j].x, smoothing_width);
        }
        return val;
    }

    float p(const int &i) {
        return gas_constant * (particles[i].density - rest_density);
    }

    glm::vec3 f_viscosity(const int &i, glm::vec3(*laplacian_W)(glm::vec3, float)) {
        glm::vec3 val(0.0f, 0.0f, 0.0f);
        for(int j = 0;j < particles.size();j++) {
            val += particles[j].mass * ((particles[j].v - particles[i].v)/particles[j].density) * laplacian_W(particles[i].x - particles[j].x, smoothing_width);
        }
        return mew * val;
    }




    // void calcDensity(const int &j, const std::vector<int> &neighbors) {
    //     particles[j].density = 0.0f;
    //     for(int i = 0;i < neighbors.size();i++) {
    //         particles[j].density += particles[j].mass * gaussian_kernel(j, neighbors[i]);
    //     }
    // }
    //
    //
    //
    // void calcPressure(const int &j, const std::vector<int> &neighbors) {
    //     particles[j].pressure = 0.0f;
    //     for(int i = 0;i < neighbors.size();i++) {
    //         particles[j].pressure +=
    //     }
    // }

    glm::vec3 calcAcceleration(const int &j) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 grad_W(glm::vec3 r, float h, float (*W)(glm::vec3, float)) {
        float b = FLT_EPSILON;
        return glm::vec3(
            W(glm::vec3(r.x + b, r.y, r.z) - glm::vec3(r.x - b, r.y, r.z), h),
            W(glm::vec3(r.x, r.y + b, r.z) - glm::vec3(r.x, r.y - b, r.z), h),
            W(glm::vec3(r.x, r.y, r.z + b) - glm::vec3(r.x, r.y, r.z - b), h)
        );
    }

    float W_poly6(glm::vec3 r, float h) {
        float r_ = glm::length(r);
        if (!(0 <= r_ && r_ <= h)) {
            return 0;
        }
        float a = 315.0f / (64.0f * M_PI * pow(h, 9));
        float b = pow(pow(h, 2) - pow(r_, 2), 3);
        return a * b;
    }



    float gaussian_kernel(int i, int j) {
        float dist = glm::distance(particles[i].x, particles[j].x);
        return 1 / (pow(M_PI, 1.5) * pow(smoothing_width, 3)) * exp(pow(dist, 2) / pow(smoothing_width, 2));
    }
};

#endif
