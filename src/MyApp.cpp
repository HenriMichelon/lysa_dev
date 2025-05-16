import lysa;
import main_scene;

namespace app {
    lysa::WindowConfiguration surfaceConfig {
        .rootNode = std::make_shared<MainScene>(),
        .renderingConfig = {
            // .backend = vireo::Backend::VULKAN,
            .backend = vireo::Backend::DIRECTX,
            .presentMode = vireo::PresentMode::IMMEDIATE,
            .clearColor = lysa::float3{0.0f, 0.2f, 0.4f},
            .memoryConfig = {
                .maxMeshSurfacePerBufferCount = 1000,
                .maxMaterialCount = 100
            }
        }
    };

    constexpr auto WIDTH{1280};
    constexpr auto HEIGHT{720};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "os/WindowsMain.cpp"
#endif
