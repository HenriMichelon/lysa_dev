module scene.triangles;

import lysa;

namespace app {

    void BaseScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        cameraPivot = std::make_shared<lysa::Node>();
        addChild(cameraPivot);
        camera = std::make_shared<lysa::Camera>();
        cameraPivot->addChild(camera);
    }

}