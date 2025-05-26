import lysa;
import main_scene;

namespace app {

    lysa::ApplicationConfiguration appConfig {
        .backend = vireo::Backend::VULKAN,
        // .backend = vireo::Backend::DIRECTX,
    };

    lysa::WindowConfiguration windowConfig {
        .width = 800,
        .height = 600,
        .mainViewportConfig = {},
        .renderingConfig = {
            .presentMode = vireo::PresentMode::IMMEDIATE,
            .clearColor = lysa::float3{0.0f, 0.2f, 0.4f},
        },
    };

    lysa::ViewportConfiguration viewport2Config{
        .viewport = {
            .x = 0,
            .y = 0,
            .width = 400,
            .height = 200,
        }
    };

    class MyWindow : public lysa::Window {
    public:
        MyWindow() :
        Window{windowConfig, std::make_shared<MainScene>()} {
            viewport2Config.viewport.x = (getExtent().width - viewport2Config.viewport.width)/2;
            addViewport(std::make_shared<lysa::Viewport>(viewport2Config))
               ->setRootNode(std::make_shared<MainScene>());
        }
    };

    class MyApp : public lysa::Application {
    public:
        MyApp() :
            Application{appConfig} {
            if (!lysa::dirExists("shaders")) {
                throw lysa::Exception{"Shaders directory not found, please run the application from the root of the project"};
            }

            std::wstring title{L"LysaGame"};
            if (appConfig.backend == vireo::Backend::VULKAN) {
                title.append(L" (Vulkan)");
            } else {
                title.append(L" (DirectX)");
            }
            windowConfig.title = title;
            windowConfig.x = 100;
            addWindow(std::make_shared<MyWindow>());

            windowConfig.x = 950;
            addWindow(std::make_shared<MyWindow>());
        }
    };

}

int lysaMain() {
    app::MyApp{}.run();
    return 0;
}
