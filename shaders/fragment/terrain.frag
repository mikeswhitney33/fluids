#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

const vec3 GREEN = vec3(0.0, 0.5, 0.01);
const vec3 WHITE = vec3(1.0, 1.0, 1.0);
const vec3 BROWN = vec3(0.54, 0.27, 0.074);
// rgb(139,69,19)

uniform float min_val;
uniform float max_val;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;



void main() {
    vec3 norm = normalize(Normal);
    float range = max_val - min_val;
    float colorPercent =  (Position.y - min_val) / range;

    vec4 texColor;
    float normalPercent = dot(norm, vec3(0.0f, 1.0f, 0.0f));
    if(normalPercent > 0.5f) {
        texColor = texture(texture2, TexCoord);
    }
    else {
        texColor = texture(texture1, TexCoord);
    }
    vec3 objectTexColor = vec3(texColor.r, texColor.g, texColor.b);
    vec3 objectCColor = mix(GREEN, WHITE, colorPercent);
    vec3 objectColor = mix(objectCColor, objectTexColor, 0.5f);
    // vec3 objectColor = mix(GREEN, WHITE, percent);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;


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
