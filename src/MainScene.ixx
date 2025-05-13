export module main_scene;

import lysa;

export namespace app {

    class MainScene : public lysa::Node {
    public:
        void onReady() override;

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;
    };

}