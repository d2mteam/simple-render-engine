#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    vec3 T = normalize(mat3(model) * aTangent);

    vec3 N = normalize(mat3(model) * aNormal);

    vec3 B = normalize(mat3(model) * aBitangent);

    TBN = mat3(T, B, N);
//    TBN = transpose(mat3(T, B, N));

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    texCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
