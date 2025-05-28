export module scene.triangles;

import lysa;
import scene.base;

export namespace app {

    class TrianglesScene : public BaseScene {
    public:
        void onReady() override;

        void onPhysicsProcess(float delta) override;

        bool onInput(lysa::InputEvent &inputEvent) override;

    private:
        struct {
            float gamma{2.4f};
        } gammaCorrectionData;

        std::shared_ptr<lysa::MemoryArray> vertexArray;
        std::shared_ptr<lysa::MemoryArray> indexArray;

        std::shared_ptr<lysa::MeshInstance>     triangle1; // right triangle
        std::shared_ptr<lysa::MeshInstance>     triangle2; // left triangle
        std::shared_ptr<lysa::StandardMaterial> material1; // material for the right triangle
        std::shared_ptr<lysa::ShaderMaterial>   material2; // material for the left triangle

        float gradient{0.0f}; // color gradient and scale for the triangles, used by the shaders
        float gradientSpeed{0.5f}; // speed for the gradient change
        bool rotate{true}; // true if both the triangles rotates

        void onMenuRotate();

        void onMenuShader();
    };

}