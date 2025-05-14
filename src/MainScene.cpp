module main_scene;

import lysa;

namespace app {

    void MainScene::onReady() {
        std::cout << "Hello" << std::endl;
        getSurface()->addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        const auto camera = std::make_shared<lysa::Camera>();
        camera->setPosition(0.0f, 0.0f, 2.0f);
        addChild(camera);

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
        const std::vector surface1{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
        };

        // Mesh for the first triangle
        auto mesh1 = std::make_shared<lysa::Mesh>(vertices, indices, surface1);

        // Standard material for the first triangle
        // With only a color and alpha transparency enabled
        material1 = std::make_shared<lysa::StandardMaterial>();
        material1->setAlbedoColor({0.75, 0.75, 0.75, 0.75});
        material1->setTransparency(lysa::Transparency::ALPHA);
        material1->setCullMode(vireo::CullMode::NONE);

        // We apply the material to the unique surface
        mesh1->setSurfaceMaterial(0, material1);

        // Create, place and add the Node to the scene
        triangle1 = make_shared<lysa::MeshInstance>(mesh1, L"Triangle 1");
        triangle1->setPosition(1.0f, 0.0f, 0.0f);
        addChild(triangle1);

    }

}