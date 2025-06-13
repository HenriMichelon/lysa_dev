export module nodes.player;

import lysa;
import layers;

export namespace app {

    class Player : public lysa::Character {
    public:
        // signal emmited on user interaction
        static const lysa::Signal::signal on_push_pull;

        // parameters for the above signal
        struct PushOrPullAction {
            bool push; // player want to push a crate
            bool pull; // player want to pull a crate
        };

        Player();

        bool onInput(lysa::InputEvent &event) override;

        void onPhysicsProcess(float delta) override;

        void onProcess(float alpha) override;

        void onReady() override;

        // release the mouse to access the menus
        void releaseMouse();

    protected:
        bool cameraCollisions{true};

    private:
        // movement and view state
        struct State {
            lysa::float3 velocity{lysa::FLOAT3ZERO};
            lysa::float2 lookDir{lysa::FLOAT2ZERO};

            State &operator=(const State &other) = default;
        };

        State previousState;
        State currentState;

        // movement starting speed
        const float minMovementsSpeed{1.5f};
        // maximum movement speed
        const float maxMovementsSpeed{4.0f};
        // acceleration from minMovementsSpeed to maxMovementsSpeed
        const float acceleration{2.0f};
        // jump height
        const float jumpSpeed{5.5f};
        // view rotation speed when the mouse is used
        const float mouseSensitivity{0.007f};
        // view rotaion speed when the gamepad or keyboard is used
        const float viewSensitivity{0.2f};
        // max camera vertical angle
        const float maxCameraAngleUp{lysa::radians(60.0)};
        // min camera vertical angle
        const float maxCameraAngleDown{-lysa::radians(45.0)};

        // current game pad id
        int gamepad{-1};
        // did the mouse is captured ?
        bool mouseCaptured{false};
        // invert the vertical mouse axis
        float mouseInvertedAxisY{1.0};
        // invert the vertical keyboard axis
        float keyboardInvertedAxisY{1.0};
        // current player speed
        float currentMovementSpeed{minMovementsSpeed};
        // player model
        std::shared_ptr<Node> model;
        // player camera
        std::shared_ptr<lysa::Camera> camera;
        // pivot to rotate the camera
        std::shared_ptr<Node> cameraPivot;
        // camera pivot attachment point
        std::shared_ptr<Node> cameraAttachment;
        // camera view collision sensor
        std::shared_ptr<lysa::CollisionArea> cameraCollisionSensor;
        // object colliding with the camera view
        CollisionObject *cameraCollisionTarget{nullptr};
        // counter used to avoid too many camera movements in case of a collision
        int cameraCollisionCounter{0};
        // starting value of the above counter
        int cameraCollisionCounterMax{100};
        // tween used for a smooth camera movement in case of a camera view collision
        std::shared_ptr<lysa::Tween> cameraInTween;
        // tween used for a smooth camera movement when the camera returns to the original position
        std::shared_ptr<lysa::Tween> cameraOutTween;

        // capture the mouse to allow the use of the keyboard & gamepad
        void captureMouse();

        // signal handler called when the camera collides or stops collinding with something
        void onCameraCollision(const Collision *collision);
    };
}