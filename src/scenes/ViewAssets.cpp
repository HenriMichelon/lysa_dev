module scene.view_assets;

import lysa;

namespace app {

    void ViewAssetsScene::onReady() {
        BaseScene::onReady();
        // player->setRotation({0.000000,0.477159,0.000000,0.878818});
        // player->setPosition({4.306047,2.833333,0.506063});
        player->setPosition({0.0f,3.0f, 4.0f});

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0,  .05f}));

        const auto dirLight = std::make_shared<lysa::DirectionalLight>(lysa::float4{1.0, 1.0, 1.0, 1.0f});
        dirLight->rotateX(45.0);
        dirLight->rotateY(45.0);
        addChild(dirLight);

        const auto node  = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*node, L"app://res/models/playground.assets");
        // node->scale(.01f);
        addChild(node);
        // printTree();

        lysa::GAME1("Scene loaded...");
    }

}