/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
#include "scene_data.inc.slang"

float4 fragmentMain(VertexOutput input) : SV_TARGET {
    float4 color = materials[input.materialIndex].albedoColor;
    return float4(input.uv.x, input.uv.y, color.b, 1.0);
}
