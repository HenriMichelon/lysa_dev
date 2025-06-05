module scene.view_assets;

import lysa;

namespace app {

    void ViewAssetsScene::onReady() {
        BaseScene::onReady();
        // player->setRotation({0.000000,0.290285,0.000000,0.956941});
        // player->setPosition({2.359956,-2.000000,2.579495});
        // player->setRotation({0.000000,0.949024,0.000000,0.315239});
        // player->setPosition({-5.947837,2.416667,-1.021150});
        // player->setRotation({0.000000,-0.388723,0.000000,0.921357});
        // player->setPosition({-2.403165,-1.500000,2.001667});

        // player->rotateY(180.0f);
        player->setPosition({0.0f,0.0f, 4.0f});

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, 1.05f}));

        // const auto dirLight = std::make_shared<lysa::DirectionalLight>(lysa::float4{1.0, 1.0, 1.0, 1.0f});
        // dirLight->rotateX(90.0);
        // addChild(dirLight);

        auto node  = std::make_shared<Node>(L"Cube");
        lysa::AssetsPack::load(*node, L"app://res/models/crates.assets");
        // node->scale(.01f);
        addChild(node);
        printTree();

        lysa::GAME1("Scene loaded...");
    }

}