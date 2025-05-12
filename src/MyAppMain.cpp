import lysa;
import vireo;
import glm;

lysa::SurfaceConfig surfaceConfig {
    .backend = vireo::Backend::VULKAN,
    // .backend = vireo::Backend::DIRECTX,
    .clearColor = glm::vec3{0.0f, 0.2f, 0.4f}
};

lysa::ApplicationConfig applicationConfig {
    .appDir = "..",
    .defaultFontName = "app://res/fonts/MontserratMedium.otf",
    .defaultFontSize = 30,
    .loggingMode = lysa::LOGGING_MODE_FILE | lysa::LOGGING_MODE_STDOUT,
};

constexpr auto WIDTH{1280};
constexpr auto HEIGHT{720};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "os/WindowsMain.cpp"
#endif
