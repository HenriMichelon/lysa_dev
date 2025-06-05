module scene.triangles;

import lysa;

namespace app {

    void BaseScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        player = std::make_shared<lysa::Node>();
        addChild(player);

        cameraPivot = std::make_shared<lysa::Node>();
        // cameraPivot->setPosition(0.0f, 1.5f, 0.0f);
        player->addChild(cameraPivot);

        camera = std::make_shared<lysa::Camera>();
        camera->setFarDistance(500.0f);
        cameraPivot->addChild(camera);
    }

    void BaseScene::onPhysicsProcess(const float delta) {
        const auto angle = delta * lysa::radians(45.0f);
        const auto velocity = 5.0f * delta;
        if (lysa::Input::isKeyPressed(lysa::KEY_W)) {
            player->translate(player->getFrontVector() * velocity);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_S)) {
            player->translate(player->getBackVector() * velocity);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_A)) {
            player->translate(player->getLeftVector() * velocity);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_D)) {
            player->translate(player->getRightVector() * velocity);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_PAGE_UP)) {
            player->translate(player->getUpVector() * velocity);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_PAGE_DOWN)) {
            player->translate(player->getDownVector() * velocity);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_RIGHT)) {
            player->rotateY(-angle);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_LEFT)) {
            player->rotateY(angle);
        }
        if (lysa::Input::isKeyPressed(lysa::KEY_UP)) {
            camera->rotateX(-angle);
        } else if (lysa::Input::isKeyPressed(lysa::KEY_DOWN)) {
            camera->rotateX(angle);
        }
        if (lysa::Input::isKeyJustPressed(lysa::KEY_BACKSPACE)) {
            lysa::GAME1(lysa::to_string(player->getPositionGlobal()));
            lysa::GAME1(lysa::to_string(player->getRotationGlobal()));
        }
    }

}