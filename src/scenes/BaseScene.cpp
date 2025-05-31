module scene.triangles;

import lysa;

namespace app {

    void BaseScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        camera = std::make_shared<lysa::Camera>();
        addChild(camera);
    }

}