module scene.vector;

import lysa;

namespace app {

    void VectorScene::onReady() {
        BaseScene::onReady();
        player->setPosition({0.0f,0.0f, 4.0f});
    }

    void VectorScene::onPhysicsProcess(float deltaTime) {
        drawCube();
    }

    void VectorScene::drawCube() {
        auto& debugRenderer = getViewport()->getDebugRenderer();
        const lysa::float4 red     = { 1, 0, 0, 1 };
        const lysa::float4 green   = { 0, 1, 0, 1 };
        const lysa::float4 blue    = { 0, 0, 1, 1 };
        const lysa::float4 yellow  = { 1, 1, 0, 1 };
        const lysa::float4 magenta = { 1, 0, 1, 1 };
        const lysa::float4 cyan    = { 0, 1, 1, 1 };

        // Face +Z
        debugRenderer.drawTriangle({ -0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f }, red);
        debugRenderer.drawTriangle({ -0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, red);

        // Face -Z
        debugRenderer.drawTriangle({  0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, green);
        debugRenderer.drawTriangle({  0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f }, green);

        // Face +X
        debugRenderer.drawTriangle({ 0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f,  0.5f, -0.5f }, blue);
        debugRenderer.drawTriangle({ 0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f, -0.5f }, { 0.5f,  0.5f,  0.5f }, blue);

        // Face -X
        debugRenderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, yellow);
        debugRenderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f, -0.5f }, yellow);

        // Face +Y
        debugRenderer.drawTriangle({ -0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f, -0.5f }, magenta);
        debugRenderer.drawTriangle({ -0.5f, 0.5f,  0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, magenta);

        // Face -Y
        debugRenderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f,  0.5f }, cyan);
        debugRenderer.drawTriangle({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f,  0.5f }, { -0.5f, -0.5f,  0.5f }, cyan);
    }

}