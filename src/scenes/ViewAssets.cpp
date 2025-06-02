module scene.view_assets;

import lysa;

namespace app {

    void ViewAssetsScene::onReady() {
        BaseScene::onReady();
        // camera->setPosition(0.0f, 2.0f, 0.0f);
        // camera->rotateY(lysa::radians(45.0f));
        camera->setPosition(0.0f, 2.0f, 4.0f);
        // camera->rotateX(lysa::radians(-45.0f));
        // camera->rotateY(lysa::radians(35.0f));

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        cube = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*cube, L"app://res/models/city_buildings.assets");
        // cube->scale(.01f);
        // cube->printTree();
        addChild(cube);

        lysa::GAME1("Scene loaded...");
    }

}