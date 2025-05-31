module scene.add_remove;

import lysa;

namespace app {

    void AddRemoveNodeScene::onReady() {
        BaseScene::onReady();
        camera->setPosition(2.0f, 2.0f, 4.0f);
        camera->rotateX(lysa::radians(-25.0f));
        camera->rotateY(lysa::radians(25.0f));

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        lysa::AssetsPack::load(*this, L"app://res/models/crate.assets");

        lysa::GAME1("Scene loaded...");
    }

}