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

        std::shared_ptr<lysa::MeshInstance>     triangle1; // right triangle
        std::shared_ptr<lysa::MeshInstance>     triangle2; // left triangle
        std::shared_ptr<lysa::StandardMaterial> material1; // material for the right triangle
        std::shared_ptr<lysa::ShaderMaterial>   material2; // material for the left triangle
    };

}