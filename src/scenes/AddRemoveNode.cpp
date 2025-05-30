module scene.add_remove;

import lysa;

namespace app {

    void AddRemoveNodeScene::onReady() {
        BaseScene::onReady();
        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        lysa::AssetsPack::load(*this, L"app://res/models/crates.assets");

        lysa::GAME1("Scene loaded...");
    }

}