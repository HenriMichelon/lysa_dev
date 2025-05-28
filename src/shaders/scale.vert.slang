/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "scene_data.inc.slang"

VertexOutput vertexMain(uint vertexId: SV_VertexID ) {
    Index index = indices[vertexId];
    InstanceData instanceData = instancesData[index.surfaceIndex];
    Vertex vertex = vertices[index.index + instanceData.firstVertex];
    Material material = materials[instanceData.materialIndex];

    vertex.position = vertex.position * float3(1.0f + material.parameters[1].x);
    return vertexMVP(
        instanceData,
        vertex);
}