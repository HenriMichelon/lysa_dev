export module nodes.crate;

import lysa;

export namespace app {

    // a crate with a colliding body
    class Crate : public lysa::RigidBody {
    public:
        Crate();

        Crate(const std::shared_ptr<Node>& body);
    };

}
