export module scene.add_remove;

import lysa;
import scene.base;

export namespace app {

    class AddRemove : public BaseScene {
    public:
        void onReady() override;
        bool onInput(lysa::InputEvent &inputEvent) override;
    private:
        std::shared_ptr<Node> node1;
        std::shared_ptr<Node> node2;
        std::list<std::shared_ptr<Node>> nodes;
        bool visible{true};

        void addNode();
        void removeNode();
        void toggleVisibility();
    };

}