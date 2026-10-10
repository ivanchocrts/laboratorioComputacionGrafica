//Previo 8. Materiales e iluminación        Nombre: Iván Daniel Cortés Alvarado
//Fecha de entrega: 9 de octubre del 2026   Número de cuenta: 3165028563
// Std. Includes
#include <string>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes (Se actualizarán dinámicamente con la órbita vertical)
glm::vec3 lightPos1(0.0f, 4.5f, 0.0f);
glm::vec3 lightPos2(0.0f, -4.5f, 0.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8. Materiales e iluminacion. Ivan Daniel", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // === ACTIVAR BLENDING / TRANSPARENCIA EN OPENGL ===
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model papel_picado((char*)"Models/papelpicado.obj");
    Model sol((char*)"Models/modeloSol.obj");
    Model luna((char*)"Models/modeloLuna.obj");
    Model calabaza1((char*)"Models/pumpkin.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Movimiento de las fuentes de luz 
        float radius = 4.5f;               // Radio de la orbita
        float speed  = 0.2f;               // Velocidad con la que las fuentes de luz iluminaran la escena 
        float angle  = (float)glfwGetTime() * speed;

        // Para que el movimiento de la orbita sea vertical
        float solY = radius * sin(angle);
        float solZ = radius * cos(angle);

        // Para que el sol y la luna queden en lados opuestos
        float lunaY = -solY;
        float lunaZ = -solZ;

        // Actualizacion de posiciones de luces para coincidir con el sol y la luna
        lightPos1 = glm::vec3(0.0f, solY, solZ);
        lightPos2 = glm::vec3(0.0f, lunaY, lunaZ);

        // DIBUJO DE LOS MODELOS
        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Caracteristicas del sol
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos1.x, lightPos1.y, lightPos1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.95f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.6f, 0.6f, 0.6f);

        // Caracterisiticas de la luna
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), lightPos2.x, lightPos2.y, lightPos2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.2f, 0.2f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.5f, 0.6f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.5f, 0.5f, 0.5f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Propiedades del material
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.8f);

        // Perrito
        glm::mat4 modelDog(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.0f, 0.0f, 0.0f));
        modelDog = glm::scale(modelDog, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        red_dog.Draw(lightingShader);

        // Papel picado 
        glm::mat4 modelPapel(1.0f);
        modelPapel = glm::translate(modelPapel, glm::vec3(-0.65f, 1.35f, 0.0f));
        modelPapel = glm::scale(modelPapel, glm::vec3(0.9f, 0.9f, 0.9f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPapel));
        papel_picado.Draw(lightingShader);

        shader.Use();

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Objetos adicionales: montones de calabzas, cambiar su posicion para que coincidan con las patas del perrito
        // 1. Calabaza izquierda 1
        glm::mat4 model_izq1(1.0f);
        model_izq1 = glm::translate(model_izq1, glm::vec3(0.3006f - 0.85f, -0.0037f - 0.18f, -0.7141f + 0.1f));
        model_izq1 = glm::rotate(model_izq1, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_izq1 = glm::scale(model_izq1, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_izq1));
        calabaza1.Draw(shader);

        // 2. Calabaza izquierda 2
        glm::mat4 model_izq2(1.0f);
        model_izq2 = glm::translate(model_izq2, glm::vec3(0.3006f - 1.10f, -0.0037f - 0.18f, -0.7141f - 0.1f));
        model_izq2 = glm::rotate(model_izq2, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_izq2 = glm::scale(model_izq2, glm::vec3(0.01f, 0.01f, 0.01f) * 0.85f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_izq2));
        calabaza1.Draw(shader);

        // 3. Calabaza derecha 1
        glm::mat4 model_der1(1.0f);
        model_der1 = glm::translate(model_der1, glm::vec3(0.3006f + 0.90f, -0.0037f - 0.18f, -0.7141f));
        model_der1 = glm::rotate(model_der1, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_der1 = glm::scale(model_der1, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_der1));
        calabaza1.Draw(shader);

        // 4. Calabaza derecha 2
        glm::mat4 model_der2(1.0f);
        model_der2 = glm::translate(model_der2, glm::vec3(0.3006f + 1.20f, -0.0037f - 0.18f, -0.7141f - 0.2f));
        model_der2 = glm::rotate(model_der2, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_der2 = glm::scale(model_der2, glm::vec3(0.01f, 0.01f, 0.01f) * 0.9f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_der2));
        calabaza1.Draw(shader);

        // 5. Calabaza derecha 3
        glm::mat4 model_der3(1.0f);
        model_der3 = glm::translate(model_der3, glm::vec3(0.3006f + 1.05f, -0.0037f - 0.18f, -0.7141f + 0.25f));
        model_der3 = glm::rotate(model_der3, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_der3 = glm::scale(model_der3, glm::vec3(0.01f, 0.01f, 0.01f) * 0.75f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_der3));
        calabaza1.Draw(shader);

        // Para el dibujo del sol y la luna como fuentes de luz
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Sol
        glm::mat4 modeloSol(1.0f);
        modeloSol = glm::translate(modeloSol, lightPos1);
        modeloSol = glm::rotate(modeloSol, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        modeloSol = glm::scale(modeloSol, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeloSol));

        // Para definir su color
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 1.0f, 0.7f, 0.0f);
        sol.Draw(lampshader);


        // Luna
        glm::mat4 modeloLuna(1.0f);
        modeloLuna = glm::translate(modeloLuna, lightPos2);
        modeloLuna = glm::rotate(modeloLuna, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        modeloLuna = glm::scale(modeloLuna, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeloLuna));

        // Para definir su color 
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 0.8f, 0.8f, 0.85f);
        luna.Draw(lampshader);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera and light positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_UP]    || keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_DOWN]  || keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_LEFT]  || keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // CONTROL MANUAL DE LUZ 1 (Teclas I, K, J, L)
    if (keys[GLFW_KEY_I]) lightPos1.y += 0.01f;
    if (keys[GLFW_KEY_K]) lightPos1.y -= 0.01f;
    if (keys[GLFW_KEY_J]) lightPos1.x -= 0.01f;
    if (keys[GLFW_KEY_L]) lightPos1.x += 0.01f;

    // CONTROL MANUAL DE LUZ 2 (Teclas T, G, F, H) 
    if (keys[GLFW_KEY_T]) lightPos2.y += 0.01f;
    if (keys[GLFW_KEY_G]) lightPos2.y -= 0.01f;
    if (keys[GLFW_KEY_F]) lightPos2.x -= 0.01f;
    if (keys[GLFW_KEY_H]) lightPos2.x += 0.01f;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}