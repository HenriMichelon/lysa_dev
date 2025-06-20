module nodes.player;

namespace app {

    Player::Player():
        Character{
            1.8, 0.5,
            PLAYER,
            L"Player" } {
    }

    bool Player::onInput(lysa::InputEvent &event) {
        // if the mouse is captured, the player can rotate the view with the mouse
        if (mouseCaptured && (event.getType() == lysa::InputEventType::MOUSE_MOTION)) {
            const auto &eventMouseMotion = dynamic_cast<lysa::InputEventMouseMotion &>(event);
            const auto  yRot= -eventMouseMotion.getRelativeX() * mouseSensitivity;
            rotateY(yRot);
            cameraPivot->rotateX(eventMouseMotion.getRelativeY() * mouseSensitivity * mouseInvertedAxisY);
            cameraPivot->setRotationX(std::clamp(cameraPivot->getRotationX(), maxCameraAngleDown, maxCameraAngleUp));
            return true;
        }
        // capture the mouse on the first click if not already captured
        if ((event.getType() == lysa::InputEventType::MOUSE_BUTTON) && (!mouseCaptured)) {
            const auto &eventMouseButton = dynamic_cast<lysa::InputEventMouseButton &>(event);
            if (!eventMouseButton.isPressed()) {
                captureMouse();
                return true;
            }
        }
        // if the mouse is captured, process the player keyboard events
        if ((event.getType() == lysa::InputEventType::KEY) && mouseCaptured) {
            const auto &eventKey = dynamic_cast<lysa::InputEventKey &>(event);
            // release the mouse with the ESC key
            if ((eventKey.getKey() == lysa::KEY_ESCAPE) && !eventKey.isPressed()) {
                releaseMouse();
                return true;
            }
        }
        // if the mouse is captured, process the game pad events
        if ((event.getType() == lysa::InputEventType::GAMEPAD_BUTTON) && mouseCaptured) {
            const auto &eventGamepadButton = dynamic_cast<lysa::InputEventGamepadButton &>(event);
            // release the mouse with the START button
            if ((eventGamepadButton.getGamepadButton() == lysa::GamepadButton::START) && !eventGamepadButton.isPressed()) {
                releaseMouse();
                return true;
            }
            // check if the player wants to pull or push a crate with RL or RB
            const auto push = (eventGamepadButton.getGamepadButton() == lysa::GamepadButton::RB) && eventGamepadButton.
                    isPressed();
            // send the push/pull state to the scene
            if (push) { emit(on_push); }
        }
        // continue to propagate the event
        return false;
    }

    void Player::onPhysicsProcess(const float delta) {
        // In case of a collision we move the camera on top of the player head
        // to have a closer view of the colliding object
        // if ((cameraCollisionTarget != nullptr) && ((cameraInTween == nullptr) || (!cameraInTween->isRunning()))) {
        //     const auto pos  = cameraAttachment->getPosition();
        //     const auto dest = lysa::float3{0.0f, 0.0f, -pos.z};
        //     if (any(cameraPivot->getPosition() != dest)) {
        //         // we use a tween for a smooth camera movement
        //         cameraInTween = cameraPivot->createPropertyTween(
        //                 (lysa::PropertyTween<lysa::float3>::Setter)(&Node::setPosition),
        //                 cameraPivot->getPosition(),
        //                 dest,
        //                 0.5f);
        //         // stop a possible existing tween to avoid movement collision
        //         cameraPivot->killTween(cameraOutTween);
        //     }
        // }

        // reset the movement states
        previousState = currentState;
        currentState  = State{};

        // get the input vector, first from the game pad, second for the keyboard
        lysa::float2 input = lysa::FLOAT2ZERO;
        if (gamepad != -1) {
            input = lysa::Input::getGamepadVector(gamepad, lysa::GamepadAxisJoystick::LEFT);
        }
        if (all(input == lysa::FLOAT2ZERO)) {
            input = lysa::Input::getKeyboardVector(lysa::KEY_A, lysa::KEY_D, lysa::KEY_W, lysa::KEY_S);
        }

        // Determine news basic velocity
        const auto onGround = isOnGround();
        const auto currentVerticalVelocity = dot(getVelocity(), getUpVector()) * getUpVector();
        if (onGround) {
            // we move if the player is on the ground or on an object
            currentState.velocity = mul(lysa::float3x3{getTransformGlobal()}, getGroundVelocity());
            // jump !
            if (lysa::Input::isKeyPressed(lysa::KEY_SPACE) || lysa::Input::isGamepadButtonPressed(gamepad, lysa::GamepadButton::A)) {
                currentState.velocity += (jumpSpeed + currentMovementSpeed / 2.0f) * getUpVector();
            }
        } else {
            // preserve the vertical velocity if we are in the air
            currentState.velocity = currentVerticalVelocity;
            // we can't escape gravity
            currentState.velocity += getViewport()->getPhysicsScene().getGravity() * getUpVector() * delta;
        }

        // check if the player wants to move
        if (any(input != lysa::FLOAT2ZERO)) {
            // capture the mouse if the player wants to move
            if (!mouseCaptured) { captureMouse(); }
            if (onGround) {
                // if the player is on the ground or on an object, we move in the input direction
                const auto direction = mul(lysa::TRANSFORM_BASIS, lysa::float3{input.x, 0, input.y});
                if (currentMovementSpeed == 0) {
                    // player start moving
                    currentMovementSpeed = minMovementsSpeed;
                } else {
                    // player already moving, accelerate
                    currentMovementSpeed += acceleration * delta;
                    currentMovementSpeed = std::min(currentMovementSpeed, maxMovementsSpeed);
                }
                // move
                currentState.velocity += direction * currentMovementSpeed;
            }
        } else {
            // stop all movements
            currentMovementSpeed = 0;
        }
        if (!onGround) {
            // Player doing a jump, preserve horizontal velocity.
            // We can't stop while in the air!
            const auto currentHorizontalVelocity = previousState.velocity - currentVerticalVelocity;
            currentState.velocity += currentHorizontalVelocity;
        }

        // If the mouse is captured, we can use the game pad or the keyboard
        // to rotate the view
        if (mouseCaptured) {
            lysa::float2 inputDir = lysa::FLOAT2ZERO;
            if (gamepad != -1) {
                inputDir = lysa::Input::getGamepadVector(gamepad, lysa::GamepadAxisJoystick::RIGHT);
            }
            if (all(inputDir == lysa::FLOAT2ZERO)) {
                inputDir = lysa::Input::getKeyboardVector(lysa::KEY_LEFT, lysa::KEY_RIGHT, lysa::KEY_UP, lysa::KEY_DOWN);
            }
            if (any(inputDir != lysa::FLOAT2ZERO)) {
                currentState.lookDir = inputDir * viewSensitivity * delta;
            }
        }

        // send the push/pull state to the scene
        if (lysa::Input::isKeyPressed(lysa::KEY_P)) {
            emit(on_push);
        }
    }

    void Player::onProcess(const float alpha) {
        // If the camera is on top of the player's head
        // and there is no more camera view collision,
        // we move the camera backward
        // if ((cameraCollisionTarget != nullptr) && (!cameraCollisionSensor->wereInContact(cameraCollisionTarget))) {
        //     cameraCollisionCounter -= 1;
        //     if (cameraCollisionCounter == 0) {
        //         // we use a tween for a smooth camera movement
        //         cameraOutTween = cameraPivot->createPropertyTween(
        //                 (lysa::PropertyTween<lysa::float3>::Setter)(&Node::setPosition),
        //                 cameraPivot->getPosition(),
        //                 lysa::FLOAT3ZERO,
        //                 0.5f);
        //         // stop a possible existing tween to avoid movement collision
        //         cameraPivot->killTween(cameraInTween);
        //         cameraCollisionTarget = nullptr;
        //     }
        // }

        setVelocity(previousState.velocity * (1.0f - alpha) + currentState.velocity * alpha);
        // rotate the view
        if (any(currentState.lookDir != lysa::FLOAT2ZERO)) {
            const auto interpolatedLookDir = previousState.lookDir * (1.0f - alpha) + currentState.lookDir * alpha;
            const auto yRot = -interpolatedLookDir.x * 1.0f;
            rotateY(yRot);
            cameraPivot->rotateX(interpolatedLookDir.y * keyboardInvertedAxisY);
            // cameraPivot->setRotationX(std::clamp(cameraPivot->getRotationX(), maxCameraAngleDown, maxCameraAngleUp));
        }
    }

    void Player::onReady() {
        // we capture the mouse at startup, ready to play!
        captureMouse();

        // create the player node
        model = std::make_shared<Node>();
        lysa::AssetsPack::load(*model, L"app://res/models/capsule.assets");
        // model->setPosition({0.0, -1.8 / 2.0, 0.0});
        addChild(model);

        // create the attachment point for the camera pivot
        cameraAttachment = std::make_shared<Node>(L"cameraAttachment");
        auto attachmentZOffset = 2.0f;
        auto attachmentYOffset = 2.0f;
        cameraAttachment->setPosition({0.0, attachmentYOffset, attachmentZOffset});
        addChild(cameraAttachment);

        // create the collision sensor used to detect if the camera view area collides with something
        if (cameraCollisions) {
             cameraCollisionSensor = std::make_shared<lysa::CollisionArea>(
                    std::make_shared<lysa::SphereShape>(0.25f),
                    // std::make_shared<lysa::BoxShape>(lysa::float3{1.0f, 1.0f, 1.0f }),
                    WORLD | BODIES,
                    L"cameraCollisionNode"
                    );
            // cameraCollisionSensor->setPosition({0.0, attachmentYOffset, attachmentZOffset / 2.0f - 0.5f});
            cameraCollisionSensor->connect(on_collision_starts,
                                           [this](void*p){this->onCameraCollision((const Collision *)p);});
            cameraCollisionSensor->connect(on_collision_persists,
                                           [this](void*p){this->onCameraCollision((const Collision *)p);});
            cameraAttachment->addChild(cameraCollisionSensor);
        }

        // create the pivot used to rotate the camera
        cameraPivot = std::make_shared<Node>(L"cameraPivot");
        cameraAttachment->addChild(cameraPivot);

        // create the player camera and attach it to the pivot
        camera = std::make_shared<lysa::Camera>(L"Player camera");
        camera->setPerspectiveProjection(75.0, 0.1, 500.0);
        cameraPivot->addChild(camera);
        // replace the current camera with the player camera
        getViewport()->activateCamera(camera);

        // display information about the game pads
        lysa::GAME1(lysa::Input::getConnectedJoypads(), " connected gamepad(s)");
        for (int i = 0; i < lysa::Input::getConnectedJoypads(); i++) {
            if (lysa::Input::isGamepad(i)) {
                gamepad = i;
                break;
            }
        }
        if (gamepad != -1) {
            lysa::GAME1("Using gamepad ", lysa::Input::getJoypadName(gamepad));
        }
        //printTree();
    }

    void Player::releaseMouse() {
        getViewport()->getWindow().setMouseMode(lysa::MouseMode::VISIBLE);
        mouseCaptured = false;
    }

    void Player::captureMouse() {
        if (!mouseCaptured) {
            getViewport()->getWindow().setMouseMode(lysa::MouseMode::HIDDEN_CAPTURED);
            mouseCaptured = true;
        }
    }

    void Player::onCameraCollision(const Collision *collision) {
        cameraCollisionTarget  = collision->object;
        cameraCollisionCounter = cameraCollisionCounterMax;
    }

    const lysa::Signal::signal Player::on_push = "on_push";

}