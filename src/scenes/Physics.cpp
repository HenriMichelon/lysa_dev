module scene.physics;

import nodes.player;
import nodes.crate;
import layers;

namespace app {

    PhysicsMainScene::PhysicsMainScene():
        Node{L"Main Scene"} {
    }

    void PhysicsMainScene::onReady() {
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
        raycast = std::make_shared<lysa::RayCast>(lysa::AXIS_FRONT * 50.0f, PLAYER_RAYCAST);
        raycast->setPosition({0.0f, 0.5f, 0.0f});
        player->addChild(raycast);

        // generates crates nodes with random positions
        const auto crateScene = std::make_shared<Node>();
        lysa::AssetsPack::load(*crateScene, L"app://res/models/crate.assets");
        const auto &crateModel = crateScene->getChild(L"Crate");
        for (int x = 0; x < 4; x++) {
            for (int z = 0; z < 4; z++) {
                const auto model = crateModel->duplicate();
                auto body  = std::make_shared<Crate>();
                body->addChild(model);
                body->setPosition({x * 5 - 1.5 * 5, 1.0 + std::rand() % 5, -z * 5 + 5});
                game->addChild(body);
            }
        }

        // create the material to outline the crates in front of the player
        // auto& outlineMaterials = Application::get().getOutlineMaterials();
        // raycastOutlineMaterial = std::make_shared<lysa::ShaderMaterial>(outlineMaterials.get(0));
        // raycastOutlineMaterial->setParameter(0, {1.0, 0.0, 0.0, 1.0});
        // raycastOutlineMaterial->setParameter(1, vec4{0.005});
        // outlineMaterials.add(raycastOutlineMaterial);
        //
        // // create material to outline the crate in collision with the player
        // collisionOutlineMaterial = std::make_shared<lysa::ShaderMaterial>(outlineMaterials.get(0));
        // collisionOutlineMaterial->setParameter(0, {0.0, 1.0, 0.0, 1.0});
        // collisionOutlineMaterial->setParameter(1, vec4{0.02});
        // outlineMaterials.add(collisionOutlineMaterial);

        // build the scene floor node and associated static body
        const auto floorScene = std::make_shared<Node>();
        lysa::AssetsPack::load(*floorScene, L"app://res/models/playground.assets");
        auto floorModel = floorScene->findFirstChild(L"Box001_asphalt_0");
        if (floorModel == nullptr) throw lysa::Exception("Floor not found");
        auto floorPhysicsMaterial =
            lysa::Application::getPhysicsEngine().createMaterial(0.0f, 0.5f);
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
                // std::make_shared<lysa::BoxShape>(lysa::float3{50.0f, 0.5f, 50.0f}),
                WORLD,
                L"Floor");
        floor->addChild(floorScene);
        // floor->setPosition({0.0f, 5.0f, 0.0f});
        game->addChild(floor);

        // connect the player signals for the "push" and "pull" actions
        player->connect(Player::on_push_pull, [this](void*p){this->onPushOrPull((Player::PushOrPullAction *)p);});
    }

    void PhysicsMainScene::onProcess(float alpha) {
        // if a crate is already selected (and optionally outlined) from the previous frame,
        // deselect it for the current frame
        if (previousSelection != nullptr) {
            // if (previousSelection->getOutlineMaterial() == raycastOutlineMaterial) {
            //     previousSelection->setOutlined(false);
            // }
            previousSelection = nullptr;
        }
        // detect if a crate is in front on the player
        if (raycast->isColliding()) {
            const auto& collider = *(raycast->getCollider());
            const auto& meshInstance = collider.findFirstChild<lysa::MeshInstance>();
            // if not already outlined activate and set the outline material
            // if (!meshInstance->isOutlined()) {
            //     meshInstance->setOutlined(true);
            //     meshInstance->setOutlineMaterial(raycastOutlineMaterial);
            //     previousSelection = meshInstance;
            //     //log("Collide ", meshInstance->toString(), " ",  to_string(meshInstance->getId()));
            // }
        }
        // clear all the previously colliding crates
        // and disable the oulines off all colliding crates
        // for (const auto &collision : currentCollisions) {
        //     collision.object->findFirstChild<lysa::MeshInstance>()->setOutlined(false);
        // }
        currentCollisions.clear();
        // detect all the colliding crates
        for (const auto &collision : player->getCollisions()) {
            // only if the player is not on top of a crate
            if ((!player->isGround(*collision.object) &&
                (collision.normal.y < 0.8))) {
                // push or pull the colliding crate in the colliding direction
                if (pushing || pulling) {
                    (dynamic_cast<lysa::RigidBody*>(collision.object))->applyForce(
                            force * collision.normal * (pushing ? -1.0f : 1.0f),
                            collision.position);
                }
                // outline the colliding crate
                // const auto& meshInstance = collision.object->findFirstChild<lysa::MeshInstance>();
                // meshInstance->setOutlined(true);
                // meshInstance->setOutlineMaterial(collisionOutlineMaterial);
                // save the colliding crate to disable the outline during the next frame
                currentCollisions.push_back(collision);
            }
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
        // // build the information box dispalyed when we collide a crate
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

    // signal handler called on a player action
    void PhysicsMainScene::onPushOrPull(Player::PushOrPullAction *action) {
        pushing = action->push;
        pulling = action->pull;
    }
}