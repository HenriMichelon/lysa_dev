module main_scene;

import std;

namespace app {

    void MainScene::onReady() {
        std::cout << "Hello" << std::endl;
        getSurface()->addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        const std::vector<lysa::Vertex> vertices{
                {.position = {0.0, 0.5, 0.0}, .uv = {0.5, 0.25}},
                {.position = {0.5, -0.5, 0.0}, .uv = {0.75, 0.75}},
                {.position = {-0.5, -0.5, 0.0f}, .uv = {0.25, 0.75}},
        };
        const std::vector<uint32_t> indices{
            0,
            1,
            2
        };
        // Index-based surface for the first triangle
        const std::vector surfaces1{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
    };
    }

}