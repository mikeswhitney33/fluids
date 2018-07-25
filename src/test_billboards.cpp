#ifndef TEST_BILLBOARD_H
#define TEST_BILLBOARD_H

#include <camera_scene.hpp>
#include "models/billboard.hpp"
#include <camera.hpp>

#include <iostream>

class BillboardScene : public CameraScene {
public:
    BillboardScene():billboard("resources/container.jpg", glm::vec2(0.2f, 0.1f)),CameraScene("BillBoard", 800, 600){}
    ~BillboardScene() {}
private:
    Billboard billboard;
    virtual void PreLoop(){};
    virtual void MidLoop(){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();
        billboard.render(camera, glm::vec3(0.0f, 0.0f,0.0f));
    };
};

#endif


int main(int argc, char** argv) {
    GraphicsScene* cubescene = new BillboardScene();
    cubescene->Render();
    delete cubescene;

    return 0;
}
