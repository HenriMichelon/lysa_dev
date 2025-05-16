module main_scene;

import lysa;

namespace app {

    void MainScene::onReady() {
        std::cout << "Hello" << std::endl;
        getWindow()->addPostprocessing(L"gamma_correction", &gammaCorrectionData, sizeof(gammaCorrectionData));

        const auto camera = std::make_shared<lysa::Camera>();
        camera->setPosition(0.0f, 0.0f, 2.0f);
        addChild(camera);

        const std::vector<lysa::Vertex> vertices{
            // First triangle
            {.position = {0.0, 0.5, 0.0}, .uv = {0.5, 0.25}},
            {.position = {0.5, -0.5, 0.0}, .uv = {0.75, 0.75}},
            {.position = {-0.5, -0.5, 0.0f}, .uv = {0.25, 0.75}},
            // Second triangle
            {.position = {0.0, -0.5, 0.0}, .uv = {0.5, 0.75}},
            {.position = {0.5, 0.5, 0.0}, .uv = {0.75, 0.25}},
            {.position = {-0.5, 0.5, 0.0f}, .uv = {0.25, 0.25}},
        };
        const std::vector<uint32_t> indices{
            0, 1, 2,
        };
        const auto vireo = getWindow()->getVireo();
        const auto vertexBuffer = vireo->createBuffer(vireo::BufferType::VERTEX, sizeof(lysa::Vertex), vertices.size());
        const auto indexBuffer = vireo->createBuffer(vireo::BufferType::INDEX, sizeof(uint32_t), indices.size());
        const auto allocator = vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
        const auto commandList = allocator->createCommandList();
        commandList->begin();
        commandList->upload(vertexBuffer, vertices.data());
        commandList->upload(indexBuffer, indices.data());
        commandList->end();
        getWindow()->getGraphicQueue()->submit({commandList});
        getWindow()->getGraphicQueue()->waitIdle();
        commandList->cleanup();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Index-based surface for the first triangle
        const std::vector surface1{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
        };
        // Mesh for the first triangle
        // auto mesh1 = std::make_shared<lysa::Mesh>(vertices, indices, surface1, L"Triangle 1");
        auto mesh1 = std::make_shared<lysa::Mesh>(
            vertices,
            indices,
            surface1,
            0,
            0,
            vertexBuffer,
            indexBuffer,
            L"Triangle 1");
        // Standard material for the first triangle
        // With only a color and alpha transparency enabled
        material1 = std::make_shared<lysa::StandardMaterial>();
        material1->setAlbedoColor({0., 1.0, 0., 1.0});
        material1->setTransparency(lysa::Transparency::ALPHA);
        material1->setCullMode(vireo::CullMode::NONE);
        // We apply the material to the unique surface
        mesh1->setSurfaceMaterial(0, material1);
        // Create, place and add the Node to the scene
        triangle1 = std::make_shared<lysa::MeshInstance>(mesh1, L"Triangle 1");
        triangle1->setPosition(1.0f, 0.0f, 0.0f);
        addChild(triangle1);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Index-based surface for the second triangle
        const std::vector surfaces2{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
        };
        // Mesh for the second triangle
        // auto mesh2 = std::make_shared<lysa::Mesh>(vertices, indices, surfaces2);
        auto mesh2 = std::make_shared<lysa::Mesh>(
           vertices,
           indices,
           surfaces2,
           0,
           indices.size() * sizeof(lysa::Vertex),
           vertexBuffer,
           indexBuffer,
           L"Triangle 2");
        // Shader-based material for the second triangle
        // With a fragment shader, a vertex shader and alpha transparency enabled
        // material2 = make_shared<ShaderMaterial>(
        //         "examples/uv_gradient.frag",
        //         "examples/scale.vert");
        // material2->setTransparency(Transparency::ALPHA);
        // material2->setCullMode(CullMode::DISABLED);
        // material2->setParameter(0, vec4{0.0f}); // parameter for the fragment shader
        // material2->setParameter(1, vec4{0.0f}); // parameter for the vertex shader
        // We apply the material to the unique surface
        material2 = std::make_shared<lysa::StandardMaterial>();
        material2->setAlbedoColor({1.0, 0.0, 0.0, 1.0});
        material2->setTransparency(lysa::Transparency::ALPHA);
        material2->setCullMode(vireo::CullMode::NONE);
        mesh2->setSurfaceMaterial(0, material2);
        // Create, place and add the Node to the scene
        triangle2 = std::make_shared<lysa::MeshInstance>(mesh2, L"Triangle 2");
        triangle2->setPosition(-1.0, 0.0, 0.0);
        addChild(triangle2);
    }

    void MainScene::onPhysicsProcess(const float delta) {
        auto pos = triangle2->getPosition();
        // std::cout << pos.x << std::endl;
        triangle2->setPosition(pos.x - 0.1 * delta, 0.0, 0.0);
        // triangle2->setPosition(-1.5, 0.0, 0.0);
    }

}