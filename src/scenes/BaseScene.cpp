module scene.triangles;

import lysa;

namespace app {

    void BaseScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        const auto camera = std::make_shared<lysa::Camera>();
        camera->setPosition(0.0f, 0.0f, 2.0f);
        addChild(camera);
    }

}