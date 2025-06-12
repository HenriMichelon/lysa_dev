module scene.add_remove;

import lysa;

namespace app {

    void AddRemove::onReady() {
        BaseScene::onReady();
        player->setPosition({0.0f,0.0f, 2.0f});

        addChild(std::make_shared<lysa::Environment>(lysa::float4{1.0, 1.0, 1.0,  1.05f}));

        node1 = std::make_shared<Node>();
        node2 = std::make_shared<Node>();
        lysa::AssetsPack::load(*node1, L"app://res/models/crate.assets");
        lysa::AssetsPack::load(*node2, L"app://res/models/sphere.assets");
        addNode();
    }

    void AddRemove::addNode() {
        const auto newNode = lysa::randomi(1) == 0 ? node1->duplicate() : node2->duplicate();
        newNode->setPosition(
            lysa::randomf(10.0f) - 5.0f,
            lysa::randomf(10.0f) - 5.0f,
            lysa::randomf(10.0f) - 15.0f);
        if (addChild(newNode)) {
            nodes.push_back(newNode);
            lysa::GAME1(nodes.size(), " nodes");
        }
    }

    void AddRemove::removeNode() {
        if (removeChild(nodes.back())) { nodes.pop_back(); }
    }

    void AddRemove::toggleVisibility() {
        if (visible) {
            for (const auto& node : nodes) {
                node->setVisible(lysa::randomi(1) == 0);
            }
            visible = false;
        } else {
            for (const auto& node : nodes) {
                node->setVisible(true);
            }
            visible = true;
        }
    }

    bool AddRemove::onInput(lysa::InputEvent &inputEvent) {
        if (inputEvent.getType() == lysa::InputEventType::KEY) {
            const auto &eventKey = dynamic_cast<lysa::InputEventKey &>(inputEvent);
            if ((eventKey.getKey() == lysa::KEY_ENTER) && !eventKey.isPressed()) {
                addNode();
                return true;
            }
            if ((eventKey.getKey() == lysa::KEY_BACKSPACE) && !eventKey.isPressed()) {
                removeNode();
                return true;
            }
            if ((eventKey.getKey() == lysa::KEY_V) && !eventKey.isPressed()) {
                toggleVisibility();
                return true;
            }
        }
        return false;
    }

}