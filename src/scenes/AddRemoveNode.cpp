module scene.add_remove;

import lysa;

namespace app {

    void AddRemoveNodeScene::onReady() {
        BaseScene::onReady();
        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        // const auto camera = std::make_shared<lysa::Camera>();
        // camera->setPosition(0.0f, 0.0f, 2.0f);
        // addChild(camera);

    }

}