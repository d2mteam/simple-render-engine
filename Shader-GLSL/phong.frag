#version 430 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 texCoord;
in mat3 TBN;

uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Normal;

void main() {
    vec3 albedo = texture(u_Diffuse, texCoord).rgb;
    vec3 normal = normalize(TBN * (2.0 * texture(u_Normal, texCoord).rgb - 1.0));

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);

    float ambientStrength = 0.000000000;
    vec3 ambient = ambientStrength * lightColor;


    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = 0.04 * diff * lightColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    vec3 finalColor = (ambient + diffuse + specular) * albedo;
    FragColor = vec4(pow(finalColor, vec3(1.0/2.2)), 1.0);
}


