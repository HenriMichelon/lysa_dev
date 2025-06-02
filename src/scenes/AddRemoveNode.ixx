export module scene.add_remove;

import lysa;
import scene.base;

export namespace app {

    class AddRemoveNodeScene : public BaseScene {
    public:
        void onReady() override;

    private:
        std::shared_ptr<Node> cube;
    };

}