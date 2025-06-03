module scene.view_assets;

import lysa;

namespace app {

    void ViewAssetsScene::onReady() {
        BaseScene::onReady();
        // player->setRotation({0.000000,0.290285,0.000000,0.956941});
        // player->setPosition({2.359956,-2.000000,2.579495});
        player->setRotation({0.000000,0.949024,0.000000,0.315239});
        player->setPosition({-5.947837,2.416667,-1.021150});

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, .05f}));

        const auto dirLight = std::make_shared<lysa::DirectionalLight>(lysa::float4{1.0, 1.0, 1.0, 1.0f});
        dirLight->rotateX(90.0);
        addChild(dirLight);

        cube = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*cube, L"app://res/models/sponza.assets");
        // cube->scale(.01f);
        // cube->printTree();
        addChild(cube);

        lysa::GAME1("Scene loaded...");
    }

}