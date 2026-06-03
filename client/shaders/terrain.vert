#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec4 fragPosition;
out vec2 texCoord;
out vec3 normal;
out vec3 worldNormal;
out float v_Height;

uniform mat4 modelview;
uniform mat4 projection;
uniform mat3 normalMatrix;

layout (binding = 0) uniform sampler2D heightMap;
uniform float heightScale;

void main() {
    float texelSize = 1.0 / float(textureSize(heightMap, 0).x);
    float h  = texture(heightMap, aTexCoord).r * heightScale;
    float hL = texture(heightMap, aTexCoord + vec2(-texelSize, 0.0)).r * heightScale;
    float hR = texture(heightMap, aTexCoord + vec2( texelSize, 0.0)).r * heightScale;
    float hD = texture(heightMap, aTexCoord + vec2(0.0, -texelSize)).r * heightScale;
    float hU = texture(heightMap, aTexCoord + vec2(0.0,  texelSize)).r * heightScale;

    v_Height = h / heightScale;

    vec3 n;
    n.x = hL - hR;
    n.z = hD - hU;
    n.y = 2.0 * texelSize * heightScale;
    normal = normalize(normalMatrix * normalize(n));
    worldNormal = normalize(n);

    vec3 displacedPos = vec3(aPos.x, h, aPos.z);
    fragPosition = modelview * vec4(displacedPos, 1.0);
    texCoord = aTexCoord;

    gl_Position = projection * fragPosition;
}