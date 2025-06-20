import lysa;
import scene.view_assets;
import scene.view_scene;
import scene.triangles;
import scene.add_remove;
import scene.physics;
import scene.vector;
import layers;

namespace app {

    lysa::ApplicationConfiguration appConfig {
        .physicsConfig = {
            .layerCollisionTable = lysa::LayerCollisionTable{
                LAYERS_COUNT,
                {
                   { PLAYER, { WORLD, BODIES, USABLE_PROP }},
                   { BODIES, { WORLD, BODIES, USABLE_PROP }},
                   { PLAYER_RAYCAST, { BODIES }},
                   { TRIGGERS, { PLAYER }},
                   { INTERACTIONS, { USABLE_PROP }},
                }
            },
        },
        .loggingMode{lysa::LOGGING_MODE_STDOUT},
        // .backend = vireo::Backend::VULKAN,
        .backend = vireo::Backend::DIRECTX,
        .resourcesConfig = {
            // .maxVertexInstances = 50000000,
            // .maxIndexInstances  = 20000000,
        }
    };

    lysa::WindowConfiguration windowConfig {
        // .mode = lysa::WindowMode::WINDOWED_MAXIMIZED,
        .width = 1280,
        .height = 720,
        .mainViewportConfig = {
            .sceneConfig = {
                // .maxVertexPerFramePerPipeline = 3000000
                // .maxVertexPerFrame = 30000000
            },
            .debugConfig = {
                .enabled = true,
                .displayAtStartup = false,
                .depthTestEnable = true,
                .drawCoordinateSystem = false,
                .coordinateSystemScale = 2.0f,
                .drawRayCast = true,
                .drawShape = true,
                .drawBoundingBox = false,
                .drawVelocity = false,
                .drawCenterOfMass = false,
            }
        },
        .renderingConfig = {
            .rendererType = lysa::RendererType::FORWARD,
            .depthStencilFormat = vireo::ImageFormat::D32_SFLOAT,
            // .rendererType = lysa::RendererType::DEFERRED,
            // .depthStencilFormat = vireo::ImageFormat::D32_SFLOAT_S8_UINT,
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
        Window{windowConfig, std::make_shared<PhysicsMainScene>()} {
            // viewport2Config.viewport.x = (getExtent().width - viewport2Config.viewport.width)/2;
            // viewport2 = addViewport(std::make_shared<lysa::Viewport>(viewport2Config));
            // viewport2->setRootNode(std::make_shared<ViewAssetsScene>());
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
                title.append(L" (Vulkan ");
            } else {
                title.append(L" (DirectX ");
            }
            if (windowConfig.renderingConfig.rendererType == lysa::RendererType::FORWARD) {
                title.append(L"Forward,");
            } else {
                title.append(L"Deferred, ");
            }
            if (lysa::PhysicsEngine::getEngineType() == lysa::PhysicsEngineType::JOLT) {
                title.append(L"Jolt)");
            } else {
                title.append(L"PhysX)");
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
                std::cout << "Images  : " << totalImages << " bytes (" << totalImages/1024/1024 << "Mb)" << std::endl;
            }
        }
    };

}

int lysaMain() {
    app::MyApp{}.run();
    return 0;
}
