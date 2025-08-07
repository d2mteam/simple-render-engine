#version 430 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 texCoord;
in mat3 TBN;
in vec3 Normal;

uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Normal;

layout(std140, binding = 1) uniform Material {
    vec4 baseColor;
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 emissiveColor;
    vec4 ambientColor;
    vec4 scalarParams; // x: roughness, y: metallic, z: shininess, w: opacity
    vec4 alphaParams;  // alphaCutoff, doubleSided, transparent, alphaMasked
} material;

float orenNayarDiffuse(vec3 N, vec3 V, vec3 L, float roughness)
{
    float sigma2 = roughness * roughness;

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    float A = 1.0 - (0.5 * sigma2 / (sigma2 + 0.33));
    float B = 0.45 * sigma2 / (sigma2 + 0.09);

    float theta_r = acos(NdotV);
    float theta_i = acos(NdotL);

    float alpha = max(theta_i, theta_i); // same as max(theta_i, theta_r)
    float beta = min(theta_i, theta_i);  // same as min(theta_i, theta_r)

    vec3 V_perp = normalize(V - N * NdotV);
    vec3 L_perp = normalize(L - N * NdotL);
    float cos_phi_diff = max(dot(V_perp, L_perp), 0.0);

    float diffuse = A + B * cos_phi_diff * sin(alpha) * tan(beta);
    return diffuse * NdotL;
}
// ---- Fresnel-Schlick Approximation ----
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// ---- Normal Distribution Function: GGX/Trowbridge-Reitz ----
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.141592 * denom * denom;
    return num / denom;
}

// ---- Geometry Function: Schlick-GGX ----
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

// ---- Smith's Geometry Function ----
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 computeF0(vec3 n, vec3 k) {
    vec3 numerator   = (n - 1.0) * (n - 1.0) + k * k;
    vec3 denominator = (n + 1.0) * (n + 1.0) + k * k;
    return numerator / denominator;

}
void main() {


    vec3 albedo = texture(u_Diffuse, texCoord).rgb;
    vec3 normalMap = texture(u_Normal, texCoord).rgb;
    vec3 normal = normalize(TBN * (normalMap * 2.0 - 1.0));

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);

    float distance = length(lightPos - FragPos);
    float attenuation = 1.0;
    vec3 radiance = lightColor * attenuation;

    // ---- Cook-Torrance BRDF ----
//    float roughness = material.scalarParams.x;
    float metallic = material.scalarParams.y;
    float roughness = pow(material.scalarParams.z / 1000, 2);

    vec3 F0 = mix(vec3(0.04), material.baseColor.rgb, metallic); // reflectivity
    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.001) * max(dot(N, L), 0.001);
    vec3 specular = numerator / max(denominator, 0.001);

    // ---- Diffuse term ----
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = orenNayarDiffuse(N, V, L, roughness) * albedo / 3.141592;
    vec3 Lo = (kD * diffuse + specular) * radiance * NdotL;

    // ---- Ambient term ----
    vec3 ambient = material.ambientColor.rgb * (0.03) * albedo;
    vec3 color = ambient + Lo;

    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, material.scalarParams.w);
}