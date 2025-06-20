export module scene.physics;

import lysa;
import layers;
import nodes.player;
import nodes.crate;

export namespace app {

    class PhysicsMainScene : public lysa::Node {
    public:
        PhysicsMainScene();

        void onReady() override;

        void onProcess(float alpha) override;

        void onPhysicsProcess(float delta) override;

        void onEnterScene() override;

        void onExitScene() override;

    private:
        // force used when pulling or pushing a crate
        const lysa::float3 force = lysa::float3{500.0f};

        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

        // scene menu
        // std::shared_ptr<ui::Window> menu;
        // ou player node
        std::shared_ptr<Player> player;
        // raycast used to outline the crate in front of the player
        std::shared_ptr<lysa::RayCast> rayCast;
        // material for the selection outline
        std::shared_ptr<lysa::ShaderMaterial> rayCastOutlineMaterial;
        // used to save the currently selected crate to disable the outline
        std::shared_ptr<lysa::MeshInstance> previousSelection{nullptr};
        // information box used when the player collides with a crate
        // std::shared_ptr<ui::Window> infoBox;
        // text displaying the colliding object name
        // std::shared_ptr<ui::Text> infoText;
        // text displaying the actions the player can do when colliding a crate
        // std::shared_ptr<ui::Text> actionsText;
        // outline material for colliding crates
        std::shared_ptr<lysa::ShaderMaterial> collisionOutlineMaterial;
        // list a colliding crate to reset the outline materials
        std::unordered_set<std::shared_ptr<lysa::MeshInstance>> currentCollisions;

        // player actions: push a crate
        bool pushing{false};
;
    };

}