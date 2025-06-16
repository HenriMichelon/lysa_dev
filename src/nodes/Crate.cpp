module nodes.crate;

import layers;

namespace app {

    // Crate::Crate(const std::shared_ptr<Node>& body):
    //     RigidBody{
    //         std::make_shared<lysa::ConvexHullShape>(body),
    //         BODIES,
    //         L"CrateBody"} {
    // }

    Crate::Crate():
        RigidBody{
            std::make_shared<lysa::BoxShape>(lysa::float3{2.0f, 2.0f, 2.0f}),
            BODIES,
            L"CrateBody"} {
    }

}