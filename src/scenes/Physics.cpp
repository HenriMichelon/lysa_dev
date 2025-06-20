module scene.physics;

import nodes.player;
import nodes.crate;
import layers;

namespace app {

    PhysicsMainScene::PhysicsMainScene():
        Node{L"Main Scene"} {
    }

    void PhysicsMainScene::onReady() {
        getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        // make the scene node not pauseable
        setProcessMode(lysa::ProcessMode::ALWAYS);
        // add the global environment
        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0, .75f}));

        // add a game node and make it pauseable since the scene can't be paused
        const auto game = std::make_shared<Node>(L"Game");
        game->setProcessMode(lysa::ProcessMode::PAUSABLE);
        addChild(game);

        // add the Sun
        const auto directionalLight1 = std::make_shared<lysa::DirectionalLight>(lysa::float4{1.0f, 1.0f, 1.0f, 0.8f});
        directionalLight1->rotateX(lysa::radians(-45.0f));
        directionalLight1->rotateY(lysa::radians( 45.0f));
        directionalLight1->setCastShadows(true);
        game->addChild(directionalLight1);

        // add the player
        player = std::make_shared<Player>();
        player->setPosition({0.0f, 0.0f, 10.0f});
        game->addChild(player);

        // add an optional spotlight
        // const auto spotLight1 = std::make_shared<lysa::SpotLight>(
        //         8.0f, 12.0f, 10.0f,
        //         vec4{1.0f, 1.0f, 0.0f, 2.0f});
        // spotLight1->setPosition({0.0, 1.0, -0.5});
        // spotLight1->rotateX(lysa::radians(-20.0f));
        // player->addChild(spotLight1);

        // raycast used to detect crates in front of the player
        rayCast = std::make_shared<lysa::RayCast>(lysa::AXIS_FRONT * 50.0f, PLAYER_RAYCAST);
        rayCast->setPosition({0.0f, 0.5f, 0.0f});
        player->addChild(rayCast);

        // generates crates nodes with random positions
        const auto crateScene = std::make_shared<Node>();
        lysa::AssetsPack::load(*crateScene, L"app://res/models/crate.assets");
        const auto &crateModel = crateScene->getChild(L"Crate");
        std::shared_ptr<Crate> first;
        for (int x = 0; x < 4; x++) {
            for (int z = 0; z < 4; z++) {
                const auto model = crateModel->duplicate();
                auto body  = std::make_shared<Crate>();
                body->addChild(model);
                body->setPosition({x * 5 - 1.5 * 5, 2.0 + std::rand() % 5, -z * 5 + 5});
                game->addChild(body);
                if (!first) {
                    first = body;
                }
            }
        }
        first->scale(2.0f);
        lysa::GAME1(first->getMass());

        // create the material to outline the crates in front of the player
        rayCastOutlineMaterial = std::make_shared<lysa::ShaderMaterial>(L"highlight.frag");
        rayCastOutlineMaterial->setParameter(0, {.4, .4, 0.05, 0.0});

        // // create material to outline the crate colliding with the player
        collisionOutlineMaterial = std::make_shared<lysa::ShaderMaterial>(L"highlight.frag");
        collisionOutlineMaterial->setParameter(0, {0.0, .2, 0.0, 0.0});

        // build the scene floor node and associated static body
        const auto floorScene = std::make_shared<Node>();
        lysa::AssetsPack::load(*floorScene, L"app://res/models/playground.assets");
        auto floorModel = floorScene->findFirstChild(L"Box001_asphalt_0");
        if (floorModel == nullptr) throw lysa::Exception("Floor not found");
        auto floorPhysicsMaterial =
            lysa::Application::getPhysicsEngine().createMaterial(0.5f, 0.5f);
        lysa::Application::getPhysicsEngine().setRestitutionCombineMode(floorPhysicsMaterial, lysa::CombineMode::MAX);
        std::vector<lysa::SubShape> floorSubShapes;
        floorSubShapes.push_back(lysa::SubShape{
            std::make_shared<lysa::ConvexHullShape>(floorModel, floorPhysicsMaterial)});
        // add virtual walls
        floorSubShapes.push_back(lysa::SubShape{
            std::make_shared<lysa::BoxShape>(lysa::float3{25.0, 10.0, 1.0}), lysa::float3{0.0, 5.0, -15.0}});
        floorSubShapes.push_back(lysa::SubShape{
            std::make_shared<lysa::BoxShape>(lysa::float3{25.0, 10.0, 1.0}), lysa::float3{0.0, 5.0, 15.0}});
        floorSubShapes.push_back(lysa::SubShape{
            std::make_shared<lysa::BoxShape>(lysa::float3{1.0, 10.0, 30.0}), lysa::float3{12.5, 5.0, 0.0}});
        floorSubShapes.push_back(lysa::SubShape{
            std::make_shared<lysa::BoxShape>(lysa::float3{1.0, 10.0, 30.0}), lysa::float3{-12.5, 5.0, 0.0}});
        // the static body to make the floor collides with the player and the crates
        const auto floor = std::make_shared<lysa::StaticBody>(
                make_shared<lysa::StaticCompoundShape>(floorSubShapes),
                WORLD,
                L"Floor");
        floor->addChild(floorScene);
        game->addChild(floor);

        // connect the player signals for the "push" action
        player->connect(Player::on_push, [this] {pushing = true;});
    }

    void PhysicsMainScene::onPhysicsProcess(float delta) {
        // currentCollisions.clear();
        // for (const auto &collision : currentCollisions) {
            // collision.object->findFirstChild<lysa::MeshInstance>()->setSurfaceOverrideMaterial(0, nullptr);
        // }
        // detect all the colliding crates
        std::unordered_set<std::shared_ptr<lysa::MeshInstance>> newCollisions;
        for (const auto &collision : player->getCollisions()) {
            // only if the player is not on top of a crate
            if ((!player->isGround(*collision.object) &&
                (collision.normal.y < 0.8))) {
                // push or pull the colliding crate in the colliding direction
                if (pushing) {
                    (dynamic_cast<lysa::RigidBody*>(collision.object))->addImpulse(
                            force * collision.normal * -1.0f,
                            collision.position);
                    pushing = false;
                }
                // outline the colliding crate
                const auto meshInstance = collision.object->findFirstChild<lysa::MeshInstance>();
                if (meshInstance->getSurfaceOverrideMaterial(0) != collisionOutlineMaterial) {
                    meshInstance->setSurfaceOverrideMaterial(0, collisionOutlineMaterial);
                }
                // save the colliding crate to disable the outline during the next frame
                newCollisions.insert(meshInstance);
            }
        }
        for (const auto &meshInstance : currentCollisions) {
            if (!newCollisions.contains(meshInstance) && meshInstance->getSurfaceOverrideMaterial(0) == collisionOutlineMaterial) {
                meshInstance->setSurfaceOverrideMaterial(0, nullptr);
            }
        }
        currentCollisions = newCollisions;
    }

    void PhysicsMainScene::onProcess(float alpha) {
        // if a crate is already selected (and optionally outlined) from the previous frame,
        // deselect it for the current frame
        // detects if a crate is in front of the player
        if (rayCast->isColliding()) {
            const auto& collider = rayCast->getCollider();
            const auto& meshInstance = collider->findFirstChild<lysa::MeshInstance>();
            if (meshInstance != previousSelection) {
                if (meshInstance->getSurfaceOverrideMaterial(0) == nullptr) {
                    meshInstance->setSurfaceOverrideMaterial(0, rayCastOutlineMaterial);
                }
                if (previousSelection && (previousSelection->getSurfaceOverrideMaterial(0) == rayCastOutlineMaterial)) {
                    previousSelection->setSurfaceOverrideMaterial(0, nullptr);
                }
                previousSelection = meshInstance;
            }
        } else if (previousSelection != nullptr) {
            if (previousSelection->getSurfaceOverrideMaterial(0) == rayCastOutlineMaterial) {
                previousSelection->setSurfaceOverrideMaterial(0, nullptr);
            }
            previousSelection = nullptr;
        }

        // if we have collisions we display an information box for the first colliding crate
        // if (!currentCollisions.empty()) {
        //     // only change the info box content if not already displayed
        //     if (!infoBox->isVisible()) {
        //         // name of the colliding object
        //         infoText->setText(currentCollisions.front().object->toString());
        //         const auto width = std::max(infoText->getWidth(), actionsText->getWidth());
        //         // resize and show the info box
        //         infoBox->setWidth(width + infoBox->getWidget().getPadding() * 2);
        //         infoBox->setX((app().getVectorExtent().x - infoBox->getWidth()) / 2);
        //         infoBox->show();
        //     }
        // } else if (infoBox->isVisible()) {
        //     infoBox->hide();
        // }

        if (lysa::Input::isKeyJustPressed(lysa::KEY_F1) && getViewport()->getDebugRenderer()) {
            getViewport()->setDisplayDebug(!getViewport()->getDisplayDebug());
        }
    }

    void PhysicsMainScene::onEnterScene() {
        // add the scene menu
        // menu = make_shared<ui::Window>(ui::Rect{0, 850, 130, 100});
        // Application::get().add(menu);
        // menu->getWidget().setPadding(5);
        // menu->getWidget().setTransparency(0.2f);
        //
        // // buttons of the scene menu
        // const auto textJump  = make_shared<ui::Text>("[SPACE] Jump");
        // const auto textMouse = make_shared<ui::Text>("[ESC] Toggle mouse");
        // menu->getWidget().add(textJump, ui::Widget::TOP);
        // menu->getWidget().add(textMouse, ui::Widget::TOP);
        // menu->setHeight(textJump->getHeight() + textMouse->getHeight() + menu->getWidget().getPadding() * 3);
        //
        // // build the information box displayed when we collide a crate
        // infoBox = make_shared<ui::Window>(ui::Rect{0, 800, 200, 100});
        // infoBox->hide();
        // Application::get().add(infoBox);
        // infoText = make_shared<ui::Text>("Info---------------------");
        // infoText->setTextColor({0.8f, 0.2f, 0.2f, 1.0f});
        // infoBox->getWidget().add(infoText, ui::Widget::TOPCENTER);
        // actionsText = make_shared<ui::Text>("[P][RB] : Push   [O][LB] : Pull");
        // actionsText->setTextColor({0.2f, 0.2f, 0.8f, 1.0f});
        // infoBox->getWidget().add(actionsText, ui::Widget::TOPCENTER);
        // infoBox->getWidget().setTransparency(0.8);
        // infoBox->getWidget().setPadding(5);
        // infoBox->setHeight(infoText->getHeight() + actionsText->getHeight() + infoBox->getWidget().getPadding() * 3);
    }

    void PhysicsMainScene::onExitScene() {
        // remove the scene menu and info box before returning to the main menu
        // Application::get().remove(menu);
        // Application::get().remove(infoBox);
    }

}