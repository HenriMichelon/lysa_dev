export module scene.base;

import lysa;

export namespace app {

    class BaseScene : public lysa::Node {
    public:
        void onReady() override;

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

    };

}