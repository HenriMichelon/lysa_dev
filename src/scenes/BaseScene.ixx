export module scene.base;

import lysa;

export namespace app {

    class BaseScene : public lysa::Node {
    public:
        void onReady() override;

    protected:
        std::shared_ptr<lysa::Camera> camera;

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

    };

}