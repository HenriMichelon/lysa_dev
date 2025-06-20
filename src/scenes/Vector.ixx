export module scene.vector;

import lysa;
import scene.base;

export namespace app {

    class VectorScene : public BaseScene {
    public:
        void onReady() override;

        void onPhysicsProcess(float deltaTime) override;

    private:
        void drawCube();
    };

}