export module scene.base;

import lysa;

export namespace app {

    class BaseScene : public lysa::Node {
    public:
        void onReady() override;
        void onPhysicsProcess(float delta) override;

    protected:
        std::shared_ptr<lysa::Camera> camera;
        std::shared_ptr<lysa::Node> cameraPivot;
        std::shared_ptr<lysa::Node> player;

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

    };

}