module scene.vector;

import lysa;

namespace app {

    void VectorScene::onReady() {
        BaseScene::onReady();
        player->setPosition({0.0f,0.0f, 4.0f});
        auto& debugRenderer = getViewport()->getDebugRenderer();
        debugRenderer.drawLine(
            {-1.0f, -1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 1.0f});
    }

}