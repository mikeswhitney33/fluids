#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

const vec3 GREEN = vec3(0.0, 0.5, 0.01);
const vec3 WHITE = vec3(1.0, 1.0, 1.0);
const vec3 BROWN = vec3(0.54, 0.27, 0.074);
// rgb(139,69,19)

uniform float min_val;
uniform float max_val;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;



void main() {
    float range = max_val - min_val;
    float percent =  (Position.y - min_val) / range;
    vec3 objectColor = mix(GREEN, WHITE, percent);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.25;
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
