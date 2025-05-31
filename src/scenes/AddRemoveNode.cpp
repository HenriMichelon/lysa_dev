module scene.add_remove;

import lysa;

namespace app {

    void AddRemoveNodeScene::onReady() {
        BaseScene::onReady();
        camera->setPosition(0.0f, 2.0f, 0.0f);
        camera->rotateY(lysa::radians(0.0f));
        // camera->setPosition(2.0f, 2.0f, 2.0f);
        // camera->rotateX(lysa::radians(-45.0f));
        // camera->rotateY(lysa::radians(35.0f));

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        cube = std::make_shared<Node>();
        lysa::AssetsPack::load(*cube, L"app://res/models/sponza.assets");
        cube->scale(1.0f);
        addChild(cube);

        lysa::GAME1("Scene loaded...");
    }

    void AddRemoveNodeScene::onPhysicsProcess(const float delta) {
        if (rotate) {
            const auto angle = delta * lysa::radians(45.0f) / 2;
            // cube->rotateY(-angle);
        }
    }
}