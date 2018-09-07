#ifndef TEST_BILLBOARD_H
#define TEST_BILLBOARD_H

#include <camera_scene.hpp>
#include "models/fluid.hpp"
#include "models/particle_fluid.hpp"
#include <camera.hpp>
#include <time.h>

#include <iostream>

class BillboardScene : public CameraScene {
public:
    BillboardScene():fluid(20, 0.5f),CameraScene("BillBoard", 800, 600){}
    ~BillboardScene() {}
private:
    ParticleFluid fluid;
    virtual void PreLoop(){

    }
    virtual void MidLoop(){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float) screen_height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        fluid.render(projection, view, deltaTime);
    }
};

#endif


int main(int argc, char** argv) {
    srand(time(NULL));
    GraphicsScene* cubescene = new BillboardScene();
    cubescene->Render();
    delete cubescene;

    return 0;
}
