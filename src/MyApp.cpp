import lysa;
import main_scene;

namespace app {

    lysa::ApplicationConfiguration appConfig {
        // .backend = vireo::Backend::VULKAN,
        .backend = vireo::Backend::DIRECTX,
    };

    lysa::WindowConfiguration windowConfig {
        .viewportConfig = {
        },
        .renderingConfig = {
            .presentMode = vireo::PresentMode::IMMEDIATE,
            .clearColor = lysa::float3{0.0f, 0.2f, 0.4f},
        },
    };

    class MyWindow : public lysa::Window {
    public:
        MyWindow(void* windowHandle) :
        Window{windowConfig, windowHandle, std::make_shared<MainScene>()} {

        }
    };

    class MyApp : public lysa::Application {
    public:
        MyApp(void* windowHandle) :
            Application{appConfig},
            window{std::make_shared<MyWindow>(windowHandle)} {
            addWindow(window);
        }

        auto& getWindow() { return window; }

    private:
        std::shared_ptr<lysa::Window> window;
    };

    constexpr auto WIDTH{1280};
    constexpr auto HEIGHT{720};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "os/WindowsMain.cpp"
#endif
