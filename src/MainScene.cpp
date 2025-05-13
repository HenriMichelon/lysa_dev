module main_scene;

import std;

namespace app {

    void MainScene::onReady() {
        std::cout << "Hello" << std::endl;
        getSurface()->addPostprocessing(L"gamma_correction");
    }

}