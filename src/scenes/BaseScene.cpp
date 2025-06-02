module scene.triangles;

import lysa;

namespace app {

    void BaseScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        player = std::make_shared<lysa::Node>();
        addChild(player);
        cameraPivot = std::make_shared<lysa::Node>();
        cameraPivot->setPosition(0.0f, 1.5f, 0.0f);
        player->addChild(cameraPivot);
        camera = std::make_shared<lysa::Camera>();
        cameraPivot->addChild(camera);
    }

    void BaseScene::onPhysicsProcess(const float delta) {
        const auto angle = delta * lysa::radians(45.0f) / 2;
        if (lysa::Input::isKeyPressed(lysa::KEY_W)) {
            auto forward = normalize(globalTransform[2].xyz);
            auto velocity = lysa::float3{0.0f, 0.0f, -10.0f * delta};
            player->translate(forward * velocity);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_S)) {
            player->translate(0.0f, 0.0f, 10.0f * delta);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_RIGHT)) {
            cameraPivot->rotateY(-angle);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_LEFT)) {
            cameraPivot->rotateY(angle);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_UP)) {
            player->rotateX(-angle);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_DOWN)) {
            player->rotateX(angle);
        }
    }

}