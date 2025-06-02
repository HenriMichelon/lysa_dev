module scene.view_assets;

import lysa;

namespace app {

    void ViewAssetsScene::onReady() {
        BaseScene::onReady();
        // player->setPosition(0.0f, 2.0f, 4.0f);
        player->setRotation({0.000000,0.949024,0.000000,0.315239});
        player->setPosition({-5.947837,2.416667,-1.021150});

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.0f}));

        cube = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*cube, L"app://res/models/sponza.assets");
        // cube->scale(.01f);
        // cube->printTree();
        addChild(cube);

        lysa::GAME1("Scene loaded...");
    }

}