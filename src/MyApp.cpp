import std;
import lysa;
import vireo;
import main_scene;

namespace app {
    lysa::SurfaceConfig surfaceConfig {
        .rootNode = std::make_shared<MainScene>(),
        .backend = vireo::Backend::VULKAN,
        // .backend = vireo::Backend::DIRECTX,
        .clearColor = lysa::float3{0.0f, 0.2f, 0.4f}
    };

    lysa::ApplicationConfig applicationConfig {
    };

    constexpr auto WIDTH{1280};
    constexpr auto HEIGHT{720};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "os/WindowsMain.cpp"
#endif
