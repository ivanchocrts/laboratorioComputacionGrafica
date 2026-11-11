// Iluminacion.cpp
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

using namespace std;

// Dimensiones de ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Prototipos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void AnimationGhost();
void saveFrameGhost(void);
void resetElementsGhost(void);
void interpolationGhost(void);

// Cámara
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
bool keys[1024];
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Luz direccional (sol fijo)
glm::vec3 dirLightDir(-0.3f, -1.0f, -0.2f);

// DeltaTime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ============================================================
// VARIABLES DE ANIMACIÓN DEL FANTASMA (Keyframes)
// ============================================================
float ghostPosX = 0.0f, ghostPosY = 0.0f, ghostPosZ = 0.0f; // Posición inicial al ras del suelo
float rotGhostX = 0.0f, rotGhostZ = 0.0f; // Rotaciones en ejes X y Z
float leftArmGhost = 0.0f;      // Rotación brazo izquierdo (caminar)
float rightArmGhost = 0.0f;     // Rotación brazo derecho (caminar)
float veloGhost = 0.0f;         // Movimiento del velo

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frameGhost {
    float ghostPosX, ghostPosY, ghostPosZ;
    float incX, incY, incZ;
    float rotGhostX, rotGhostXInc;
    float rotGhostZ, rotGhostZInc;
    float leftArmGhost, leftArmGhostInc;
    float rightArmGhost, rightArmGhostInc;
    float veloGhost, veloGhostInc;
} FRAME_GHOST;

FRAME_GHOST KeyFrameGhost[MAX_FRAMES];
int FrameIndexGhost = 0;
bool playGhost = false;
int playIndexGhost = 0;

void saveFrameGhost(void)
{
    printf("Frame Ghost %d guardado | PosZ: %.3f | RotX: %.1f | RotZ: %.1f\n", FrameIndexGhost, ghostPosZ, rotGhostX, rotGhostZ);

    KeyFrameGhost[FrameIndexGhost].ghostPosX = ghostPosX;
    KeyFrameGhost[FrameIndexGhost].ghostPosY = ghostPosY;
    KeyFrameGhost[FrameIndexGhost].ghostPosZ = ghostPosZ;
    KeyFrameGhost[FrameIndexGhost].rotGhostX = rotGhostX;
    KeyFrameGhost[FrameIndexGhost].rotGhostZ = rotGhostZ;
    KeyFrameGhost[FrameIndexGhost].leftArmGhost = leftArmGhost;
    KeyFrameGhost[FrameIndexGhost].rightArmGhost = rightArmGhost;
    KeyFrameGhost[FrameIndexGhost].veloGhost = veloGhost;

    FrameIndexGhost++;
}

void resetElementsGhost(void)
{
    ghostPosX = KeyFrameGhost[0].ghostPosX;
    ghostPosY = KeyFrameGhost[0].ghostPosY;
    ghostPosZ = KeyFrameGhost[0].ghostPosZ;
    rotGhostX = KeyFrameGhost[0].rotGhostX;
    rotGhostZ = KeyFrameGhost[0].rotGhostZ;
    leftArmGhost = KeyFrameGhost[0].leftArmGhost;
    rightArmGhost = KeyFrameGhost[0].rightArmGhost;
    veloGhost = KeyFrameGhost[0].veloGhost;
}

void interpolationGhost(void)
{
    KeyFrameGhost[playIndexGhost].incX = (KeyFrameGhost[playIndexGhost + 1].ghostPosX - KeyFrameGhost[playIndexGhost].ghostPosX) / i_max_steps;
    KeyFrameGhost[playIndexGhost].incY = (KeyFrameGhost[playIndexGhost + 1].ghostPosY - KeyFrameGhost[playIndexGhost].ghostPosY) / i_max_steps;
    KeyFrameGhost[playIndexGhost].incZ = (KeyFrameGhost[playIndexGhost + 1].ghostPosZ - KeyFrameGhost[playIndexGhost].ghostPosZ) / i_max_steps;
    KeyFrameGhost[playIndexGhost].rotGhostXInc = (KeyFrameGhost[playIndexGhost + 1].rotGhostX - KeyFrameGhost[playIndexGhost].rotGhostX) / i_max_steps;
    KeyFrameGhost[playIndexGhost].rotGhostZInc = (KeyFrameGhost[playIndexGhost + 1].rotGhostZ - KeyFrameGhost[playIndexGhost].rotGhostZ) / i_max_steps;
    KeyFrameGhost[playIndexGhost].leftArmGhostInc = (KeyFrameGhost[playIndexGhost + 1].leftArmGhost - KeyFrameGhost[playIndexGhost].leftArmGhost) / i_max_steps;
    KeyFrameGhost[playIndexGhost].rightArmGhostInc = (KeyFrameGhost[playIndexGhost + 1].rightArmGhost - KeyFrameGhost[playIndexGhost].rightArmGhost) / i_max_steps;
    KeyFrameGhost[playIndexGhost].veloGhostInc = (KeyFrameGhost[playIndexGhost + 1].veloGhost - KeyFrameGhost[playIndexGhost].veloGhost) / i_max_steps;
}

void AnimationGhost(void)
{
    if (playGhost)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndexGhost++;
            if (playIndexGhost > FrameIndexGhost - 2)
            {
                printf("Animacion del fantasma terminada\n");
                playIndexGhost = 0;
                playGhost = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolationGhost();
            }
        }
        else
        {
            ghostPosX += KeyFrameGhost[playIndexGhost].incX;
            ghostPosY += KeyFrameGhost[playIndexGhost].incY;
            ghostPosZ += KeyFrameGhost[playIndexGhost].incZ;
            rotGhostX += KeyFrameGhost[playIndexGhost].rotGhostXInc;
            rotGhostZ += KeyFrameGhost[playIndexGhost].rotGhostZInc;
            leftArmGhost += KeyFrameGhost[playIndexGhost].leftArmGhostInc;
            rightArmGhost += KeyFrameGhost[playIndexGhost].rightArmGhostInc;
            veloGhost += KeyFrameGhost[playIndexGhost].veloGhostInc;

            i_curr_steps++;
        }
    }
}

// Cubo unitario para las lámparas
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final. Computacion grafica", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Opciones globales OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

    // Inicialización de Keyframes del Fantasma modificada
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrameGhost[i].ghostPosX = 0.0f;
        KeyFrameGhost[i].ghostPosY = 0.0f; // Inicializado al nivel del suelo
        KeyFrameGhost[i].ghostPosZ = 0.0f;
        KeyFrameGhost[i].incX = 0.0f;
        KeyFrameGhost[i].incY = 0.0f;
        KeyFrameGhost[i].incZ = 0.0f;
        KeyFrameGhost[i].rotGhostX = 0.0f;
        KeyFrameGhost[i].rotGhostXInc = 0.0f;
        KeyFrameGhost[i].rotGhostZ = 0.0f;
        KeyFrameGhost[i].rotGhostZInc = 0.0f;
        KeyFrameGhost[i].leftArmGhost = 0.0f;
        KeyFrameGhost[i].leftArmGhostInc = 0.0f;
        KeyFrameGhost[i].rightArmGhost = 0.0f;
        KeyFrameGhost[i].rightArmGhostInc = 0.0f;
        KeyFrameGhost[i].veloGhost = 0.0f;
        KeyFrameGhost[i].veloGhostInc = 0.0f;
    }

    // ============================================================
    // SKYBOX: vértices del cubo
    // ============================================================
    GLfloat skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    // ============================================================
    // SKYBOX: carga del cubemap
    // ============================================================
    vector<const GLchar*> faces;
    faces.push_back("SkyBox/derecha.jpg");
    faces.push_back("SkyBox/izquierda.jpg");
    faces.push_back("SkyBox/arriba.jpg");
    faces.push_back("SkyBox/abajo.jpg");
    faces.push_back("SkyBox/frente.jpg");
    faces.push_back("SkyBox/atras.jpg");

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

    // Modelos Ambientales
    Model calaverasArboles((char*)"Models/calaverasArboles.obj");
    Model castilloIsla((char*)"Models/castilloIsla.obj");
    Model carpasMesas((char*)"Models/carpasMesas.obj");
    Model postesPapelPicado((char*)"Models/postesCamellon.obj");
    Model flores((char*)"Models/flores.obj");
    Model ofrendas((char*)"Models/ofrendas.obj");
    Model agua((char*)"Models/aguaReescalada.obj");

    // Modelos del Fantasma
    Model torsoGhost((char*)"Models/torso.obj");
    Model cabezaGhost((char*)"Models/cabeza.obj");
    Model veloGhost((char*)"Models/velo.obj");
    Model brazoIzquierdoGhost((char*)"Models/brazoIzquierdo.obj");
    Model brazoDerechoGhost((char*)"Models/brazoDerecho.obj");

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    skyboxShader.Use();
    glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

    // ============================================================
    // BUCLE PRINCIPAL
    // ============================================================
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        AnimationGhost();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Iluminación
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), dirLightDir.x, dirLightDir.y, dirLightDir.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.55f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.2f, 1.15f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0002f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");

        // Dibujo de escenario
        glUniform1i(glGetUniformLocation(lightingShader.Program, "isWater"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);

        glm::mat4 modelCalaveras(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCalaveras));
        calaverasArboles.Draw(lightingShader);

        glm::mat4 modelCastillo(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCastillo));
        castilloIsla.Draw(lightingShader);

        glm::mat4 modelCarpas(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCarpas));
        carpasMesas.Draw(lightingShader);

        glm::mat4 modelPostesPapelPicado(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPostesPapelPicado));
        postesPapelPicado.Draw(lightingShader);

        glm::mat4 modelFlores(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFlores));
        flores.Draw(lightingShader);

        glm::mat4 modelOfrendas(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOfrendas));
        ofrendas.Draw(lightingShader);

        // Agua
        glUniform1i(glGetUniformLocation(lightingShader.Program, "isWater"), 1);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "uTime"), (float)glfwGetTime());
        glUniform1f(glGetUniformLocation(lightingShader.Program, "waveAmplitude"), 0.08f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "waveFrequency"), 2.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "waveSpeed"), 1.2f);
        glUniform2f(glGetUniformLocation(lightingShader.Program, "waterSpeed"), 0.0f, 0.0f);

        glm::mat4 modelAgua(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAgua));
        agua.Draw(lightingShader);

        // ========================================================
        // DIBUJADO DEL FANTASMA (Estructura Jerárquica Modificada)
        // ========================================================
        float floatOffset = sin((float)glfwGetTime() * 3.0f) * 0.15f;
        float currentPosY = ghostPosY + floatOffset;
        float s = 0.0625f;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);

        // 1. MATRIZ BASE (Posición global + Flotación + Rotaciones X y Z + ESCALA GENERAL)
        glm::mat4 modelGhost = glm::mat4(1.0f);
        modelGhost = glm::translate(modelGhost, glm::vec3(ghostPosX, currentPosY, ghostPosZ));
        modelGhost = glm::rotate(modelGhost, glm::radians(rotGhostX), glm::vec3(1.0f, 0.0f, 0.0f)); // Giro X
        modelGhost = glm::rotate(modelGhost, glm::radians(rotGhostZ), glm::vec3(0.0f, 0.0f, 1.0f)); // Giro Z
        modelGhost = glm::scale(modelGhost, glm::vec3(s, s, s));

        // 2. TORSO (Raíz)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGhost));
        torsoGhost.Draw(lightingShader);

        // 3. CABEZA (Fija, atada al movimiento del torso sin separarse)
        glm::mat4 modelHead = modelGhost;
        modelHead = glm::translate(modelHead, glm::vec3(0.0f, 0.20f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHead));
        cabezaGhost.Draw(lightingShader);

        // 4. VELO
        glm::mat4 modelVelo = modelGhost;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVelo));
        veloGhost.Draw(lightingShader);

        // 5. BRAZO IZQUIERDO (Rotación hacia adelante para simular caminar)
        glm::mat4 modelArmLeft = modelGhost;
        modelArmLeft = glm::translate(modelArmLeft, glm::vec3(0.35f, 0.15f, 0.0f));
        modelArmLeft = glm::rotate(modelArmLeft, glm::radians(leftArmGhost), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArmLeft));
        brazoIzquierdoGhost.Draw(lightingShader);

        // 6. BRAZO DERECHO (Rotación hacia adelante para simular caminar)
        glm::mat4 modelArmRight = modelGhost;
        modelArmRight = glm::translate(modelArmRight, glm::vec3(-0.35f, 0.15f, 0.0f));
        modelArmRight = glm::rotate(modelArmRight, glm::radians(rightArmGhost), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArmRight));
        brazoDerechoGhost.Draw(lightingShader);

        glDisable(GL_BLEND);

        // --------------------------------------------------------
        // SKYBOX
        // --------------------------------------------------------
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 1);

        glm::mat4 viewSky = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewSky));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
    }

    // Limpieza
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteTextures(1, &cubemapTexture);

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    // Movimiento vertical (Arriba / Abajo)
    if (keys[GLFW_KEY_F]) ghostPosY += 0.005f;
    if (keys[GLFW_KEY_V]) ghostPosY -= 0.005f;

    // Movimiento Adelante / Atrás (Eje Z)
    if (keys[GLFW_KEY_T]) ghostPosZ -= 0.005f; // Hacia adelante
    if (keys[GLFW_KEY_G]) ghostPosZ += 0.005f; // Hacia atrás

    // Rotaciones generales en X y Z
    if (keys[GLFW_KEY_Z]) rotGhostX -= 1.0f;    // Girar sobre X
    if (keys[GLFW_KEY_X]) rotGhostX += 1.0f;
    if (keys[GLFW_KEY_C]) rotGhostZ -= 1.0f;    // Girar sobre Z
    if (keys[GLFW_KEY_B]) rotGhostZ += 1.0f;

    // Articulaciones de los brazos (Como caminar, sobre el eje X)
    if (keys[GLFW_KEY_H]) leftArmGhost += 1.0f;
    if (keys[GLFW_KEY_J]) leftArmGhost -= 1.0f;
    if (keys[GLFW_KEY_Y]) rightArmGhost += 1.0f;
    if (keys[GLFW_KEY_U]) rightArmGhost -= 1.0f;

    // Controles de Cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)          keys[key] = true;
        else if (action == GLFW_RELEASE)   keys[key] = false;
    }

    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_I)
        {
            if (FrameIndexGhost < MAX_FRAMES)
            {
                saveFrameGhost();
            }
            else
            {
                printf("Limite de KeyFrames de fantasma alcanzado (%d)\n", MAX_FRAMES);
            }
        }

        if (key == GLFW_KEY_O)
        {
            if (playGhost == false && (FrameIndexGhost > 1))
            {
                playIndexGhost = 0;
                i_curr_steps = 0;
                resetElementsGhost();
                interpolationGhost();
                playGhost = true;
                printf("Iniciando reproduccion de animacion del fantasma (%d frames)\n", FrameIndexGhost);
            }
            else
            {
                playGhost = false;
                printf("Animacion de fantasma pausada/detenida\n");
            }
        }

        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}