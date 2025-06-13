module scene.triangles;

import lysa;

namespace app {

    void TrianglesScene::onReady() {
        BaseScene::onReady();
        camera->setPosition({0.0f, 0.0f, 2.0f});

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Define triangle datas
        const std::vector<lysa::Vertex> vertices1{
            // First triangle
            {.position = {0.0, 0.5, 0.0}, .uv = {0.5, 0.25}},
            {.position = {0.5, -0.5, 0.0}, .uv = {0.75, 0.75}},
            {.position = {-0.5, -0.5, 0.0f}, .uv = {0.25, 0.75}},
        };
        const std::vector<lysa::Vertex> vertices2{
            // Second triangle
            {.position = {0.0, -0.5, 0.0}, .uv = {0.5, 0.75}},
            {.position = {0.5, 0.5, 0.0}, .uv = {0.75, 0.25}},
            {.position = {-0.5, 0.5, 0.0f}, .uv = {0.25, 0.25}},
        };
        const std::vector<uint32_t> indices{
            0, 1, 2,
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Index-based surface for the first triangle
        const std::vector surface1{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
        };
        // Mesh for the first triangle
        auto mesh1 = std::make_shared<lysa::Mesh>(vertices1, indices, surface1, L"Triangle 1");
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
        triangle1->setPosition({1.0f, 0.0f, 0.0f});
        addChild(triangle1);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // Index-based surface for the second triangle
        const std::vector surfaces2{
            std::make_shared<lysa::MeshSurface>(0, indices.size())
        };
        // Mesh for the second triangle
        auto mesh2 = std::make_shared<lysa::Mesh>(vertices2, indices, surfaces2);
        // Shader-based material for the second triangle
        // With a fragment shader, a vertex shader and alpha transparency enabled
        material2 = std::make_shared<lysa::ShaderMaterial>(
            L"uv_gradient.frag",
            L"scale.vert");
        material2->setTransparency(lysa::Transparency::ALPHA);
        material2->setCullMode(vireo::CullMode::NONE);
        material2->setParameter(0, lysa::float4{0.0f}); // parameter for the fragment shader
        material2->setParameter(1, lysa::float4{0.0f}); // parameter for the vertex shader
        // We apply the material to the unique surface
        mesh2->setSurfaceMaterial(0, material2);
        // Create, place and add the Node to the scene
        triangle2 = std::make_shared<lysa::MeshInstance>(mesh2, L"Triangle 2");
        triangle2->setPosition({-1.0, 0.0, 0.0});
        addChild(triangle2);
    }

    void TrianglesScene::onPhysicsProcess(const float delta) {
        // Rotate the color gradient used by the fragment shader
        gradient += gradientSpeed * delta;
        // Ensure the color gradient remains within the range [0, 1]
        gradient = std::clamp(gradient, 0.0f, 1.0f);
        if (gradient == 1.0f || gradient == 0.0f) {
            gradientSpeed = -gradientSpeed;
        }
        // Send the parameters to the shaders
        // We use the grandient value for the triangle colors
        // AND for the triangle deformation scale
        material2->setParameter(0, lysa::float4{gradient});
        material2->setParameter(1, lysa::float4{gradient});

        if (rotate) {
            const auto angle = delta * lysa::radians(90.0f) / 2;
            triangle1->rotateY(angle);
            triangle2->rotateY(-angle);
        }
    }

    bool TrianglesScene::onInput(lysa::InputEvent &inputEvent) {
        if (inputEvent.getType() == lysa::InputEventType::KEY) {
            const auto &eventKey = dynamic_cast<lysa::InputEventKey &>(inputEvent);
            if ((eventKey.getKey() == lysa::KEY_SPACE) && !eventKey.isPressed()) {
                onMenuRotate();
                return true;
            }
            if ((eventKey.getKey() == lysa::KEY_BACKSPACE) && !eventKey.isPressed()) {
                onMenuShader();
                return true;
            }
        }
        return false;
    }

    void TrianglesScene::onMenuRotate() {
        // stop or restart rotation
        rotate = !rotate;
    }

    void TrianglesScene::onMenuShader() {
        removeChild(triangle1);
        // toggle material of the right triangle
        if (triangle1->getMesh()->getSurfaceMaterial(0).get() == material1.get()) {
            triangle1->getMesh()->setSurfaceMaterial(0, material2);
        } else {
            triangle1->getMesh()->setSurfaceMaterial(0, material1);
        }
        addChild(triangle1);
    }

}