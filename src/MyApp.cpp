import lysa;
import scene.view_assets;
import scene.triangles;
import scene.add_remove;

namespace app {

    lysa::ApplicationConfiguration appConfig {
        .loggingMode{lysa::LOGGING_MODE_STDOUT},
        // .backend = vireo::Backend::VULKAN,
        .backend = vireo::Backend::DIRECTX,
        .resourcesConfig = {
            // .maxVertexInstances = 50000000,
            // .maxIndexInstances  = 20000000,
        }
    };

    lysa::WindowConfiguration windowConfig {
        .width = 1280,
        .height = 720,
        .mainViewportConfig = {
            .sceneConfig = {
                // .maxVertexPerFramePerPipeline = 3000000
                // .maxVertexPerFrame = 30000000
            }
        },
        .renderingConfig = {
            .presentMode = vireo::PresentMode::IMMEDIATE,
            .clearColor = lysa::float3{0.0f, 0.2f, 0.4f},
            .forwardDepthPrepass = false,
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
        Window{windowConfig, std::make_shared<AddRemove>()} {
            viewport2Config.viewport.x = (getExtent().width - viewport2Config.viewport.width)/2;
            // viewport2 = addViewport(std::make_shared<lysa::Viewport>(viewport2Config));
            // viewport2->setRootNode(std::make_shared<TrianglesScene>());
        }

        void onResize() override {
            viewport2Config.viewport.x = (getExtent().width - viewport2Config.viewport.width)/2;
            // viewport2->setViewport(viewport2Config.viewport);
        }

    private:
        std::shared_ptr<lysa::Viewport> viewport2;
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
            // windowConfig.x = 100;
            addWindow(std::make_shared<MyWindow>());

            // windowConfig.x = 950;
            // addWindow(std::make_shared<MyWindow>());

            if constexpr (vireo::isMemoryUsageEnabled()) {
                size_t totalBuffers{0};
                for (const auto& usage : vireo::Buffer::getMemoryAllocations()) {
                    totalBuffers += usage.size;
                }
                size_t totalImages{0};
                for (const auto& usage : vireo::Image::getMemoryAllocations()) {
                    totalImages += usage.size;
                }
                std::cout << "Buffers : " << totalBuffers << " bytes (" << totalBuffers/1024/1024 << "Mb)" << std::endl;
                std::cout << "Images: " << totalImages << " bytes (" << totalImages/1024/1024 << "Mb)" << std::endl;
            }
        }
    };

}

int lysaMain() {
    app::MyApp{}.run();
    return 0;
}
