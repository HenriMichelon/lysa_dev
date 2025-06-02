module scene.add_remove;

import lysa;

namespace app {

    void AddRemoveNodeScene::onReady() {
        BaseScene::onReady();
        camera->setPosition(0.0f, 2.0f, 0.0f);
        camera->rotateY(lysa::radians(45.0f));
        // camera->setPosition(0.0f, 0.0f, 4.0f);
        // camera->rotateX(lysa::radians(-45.0f));
        // camera->rotateY(lysa::radians(35.0f));

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        cube = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*cube, L"app://res/models/sponza.assets");
        // cube->setPosition(0.0f, 2.0f, 8.0f);
        // cube->scale(.005f);
        // cube->printTree();
        addChild(cube);

        lysa::GAME1("Scene loaded...");
    }

    void AddRemoveNodeScene::onPhysicsProcess(const float delta) {
        const auto angle = delta * lysa::radians(45.0f) / 2;
        if (lysa::Input::isKeyPressed(lysa::KEY_RIGHT)) {
            cube->rotateY(angle);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_LEFT)) {
            cube->rotateY(-angle);
        }
    }
}