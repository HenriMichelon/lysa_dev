export module scene.base;

import lysa;

export namespace app {

    class BaseScene : public lysa::Node {
    public:
        void onReady() override {
            getViewport()->getWindow().addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));
        }

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

    };

}