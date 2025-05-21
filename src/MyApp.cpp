import lysa;
import main_scene;

namespace app {

    lysa::ApplicationConfiguration appConfig {
        // .backend = vireo::Backend::VULKAN,
        .backend = vireo::Backend::DIRECTX,
        .resourcesConfig = {
            .maxVertexInstances = 1000,
            .maxIndexInstances = 1500,
            .maxMaterialInstances = 100
        }
    };

    lysa::WindowConfiguration windowConfig {
        .rootNode = std::make_shared<MainScene>(),
        .renderingConfig = {
            .presentMode = vireo::PresentMode::IMMEDIATE,
            .clearColor = lysa::float3{0.0f, 0.2f, 0.4f},
        },
    };

    constexpr auto WIDTH{1280};
    constexpr auto HEIGHT{720};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "os/WindowsMain.cpp"
#endif
