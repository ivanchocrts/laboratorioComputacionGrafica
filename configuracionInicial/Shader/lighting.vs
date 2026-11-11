
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Parametros de animacion del agua
uniform int isWater;
uniform float uTime;
uniform float waveAmplitude;
uniform float waveFrequency;
uniform float waveSpeed;
uniform vec2 waterSpeed;

void main()
{
    vec3 currentPosition = position;
    vec3 currentNormal = normal;
    vec2 currentTexCoords = texCoords;

    if (isWater == 1)
    {
        // ==========================================
        // 1. ONDAS GEOMETRICAS MULTIDIRECCIONALES
        // ==========================================

        float phase1 =
            position.x * waveFrequency
            + uTime * waveSpeed;

        float phase2 =
            position.z * waveFrequency * 0.8
            - uTime * waveSpeed * 0.7;

        float phase3 =
            (position.x + position.z)
            * waveFrequency * 0.5
            + uTime * waveSpeed * 0.5;

        float wave1 = sin(phase1);
        float wave2 = cos(phase2);
        float wave3 = sin(phase3);

        float wave = (wave1 + wave2 + wave3) / 3.0;

        // Desplazamiento vertical del agua
        currentPosition.y += wave * waveAmplitude;


        // ==========================================
        // 2. NORMALES DINAMICAS
        // ==========================================

        float derivativeX =
            waveAmplitude / 3.0 *
            (
                waveFrequency * cos(phase1)
                + waveFrequency * 0.5 * cos(phase3)
            );

        float derivativeZ =
            waveAmplitude / 3.0 *
            (
                -waveFrequency * 0.8 * sin(phase2)
                + waveFrequency * 0.5 * cos(phase3)
            );

        currentNormal = normalize(
            vec3(-derivativeX, 1.0, -derivativeZ)
        );


        // ==========================================
        // 3. ONDULACIONES VISUALES RAPIDAS
        // ==========================================

        // Primera capa de ondulaciones
        float rippleX = sin(
            texCoords.y * 25.0 + uTime * 3.0
        ) * 0.010;

        float rippleY = cos(
            texCoords.x * 22.0 - uTime * 2.7
        ) * 0.010;

        // Segunda capa de ondulaciones
        rippleX += cos(
            (texCoords.x + texCoords.y) * 35.0
            - uTime * 2.0
        ) * 0.005;

        rippleY += sin(
            (texCoords.x - texCoords.y) * 30.0
            + uTime * 2.3
        ) * 0.005;

        // Distorsion oscilatoria sin desplazamiento
        // continuo de toda la textura
        currentTexCoords =
            texCoords + vec2(rippleX, rippleY);
    }

    // ==============================================
    // 4. TRANSFORMACIONES FINALES
    // ==============================================

    vec4 worldPosition =
        model * vec4(currentPosition, 1.0);

    gl_Position =
        projection * view * worldPosition;

    FragPos = vec3(worldPosition);

    Normal = normalize(
        mat3(transpose(inverse(model))) * currentNormal
    );

    TexCoords = currentTexCoords;
}
