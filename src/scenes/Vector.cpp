module scene.vector;

import lysa;

namespace app {

    void VectorScene::onReady() {
        BaseScene::onReady();
        player->setPosition({0.0f,0.0f, 4.0f});
    }

    void VectorScene::onPhysicsProcess(float deltaTime) {
        BaseScene::onPhysicsProcess(deltaTime);
        drawCube();
    }

    void VectorScene::drawCube() {
        // auto& renderer = getViewport()->getWindow().getOutlinesRenderer();
        //
        // const lysa::float4 red     = { 1, 0, 0, 1 };
        // const lysa::float4 green   = { 0, 1, 0, 1 };
        // const lysa::float4 blue    = { 0, 0, 1, 1 };
        // const lysa::float4 yellow  = { 1, 1, 0, 1 };
        // const lysa::float4 magenta = { 1, 0, 1, 1 };
        // const lysa::float4 cyan    = { 0, 1, 1, 1 };
        //
        // // Face +Z
        // renderer.drawTriangle({ -0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f }, red);
        // renderer.drawTriangle({ -0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, red);
        //
        // // Face -Z
        // renderer.drawTriangle({  0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, green);
        // renderer.drawTriangle({  0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f }, green);
        //
        // // Face +X
        // renderer.drawTriangle({ 0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f,  0.5f, -0.5f }, blue);
        // renderer.drawTriangle({ 0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f, -0.5f }, { 0.5f,  0.5f,  0.5f }, blue);
        //
        // // Face -X
        // renderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, yellow);
        // renderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f, -0.5f }, yellow);
        //
        // // Face +Y
        // renderer.drawTriangle({ -0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f, -0.5f }, magenta);
        // renderer.drawTriangle({ -0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, magenta);
        //
        // // Face -Y
        // renderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f,  0.5f }, cyan);
        // renderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f,  0.5f }, { -0.5f, -0.5f,  0.5f }, cyan);
    }

}