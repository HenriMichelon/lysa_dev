/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "scene.inc.slang"

VertexOutput vertexMain(VertexInput input) {
    VertexOutput output;
    Instance instance = instances[instanceIndex];
    MeshSurface surface = meshSurfaces[instance.meshSurfaceIndex];
    Material material = materials[instance.materialIndex];
    float4x4 model = meshInstances[instance.meshInstanceIndex].transform;
    float4 position = float4(input.position.xyz * float3(1.0f + material.parameters[1].x), 1.0);
    float4 positionW = mul(model, position);
    output.position = mul(scene.projection, mul(scene.view, positionW));
    output.uv = float2(input.position.w, input.normal.w);
    output.materialIndex = instance.materialIndex;
    return output;
}

