//ACTIVIDAD NOMBRE
//ENTREGA //NUMERO CUENTA 
// Std. Includes
#include <string>

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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
// Con valores de 0 la camara se situa como si se estuviera dentro del objeto (coordenadas cero para todo)
//Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
Camera camera( glm::vec3( 0.0f, 2.0f, 7.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 6. Carga de modelos y camara sintetica. Ivan Daniel.", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    //Entre parentesis la ruta de donde se encuentra el archivo en 3D
    //mesaJardin
    Model mesaJardin((char*)"Models/garden_table.obj");

    //arbol
    Model arbol((char*)"Models/Gledista_Triacanthos_6.obj");

    //manzana
    Model manzana((char*)"Models/Green_Apple_OBJ.obj");
    
    //calabaza1
    Model calabaza1((char*)"Models/pumpkin.obj");
    
    //perrito1
    Model perrito1((char*)"Models/RedDog.obj");
    
    //silla1
    Model silla1((char*)"Models/silla.obj");

    //perrito2
    Model perrito2((char*)"Models/RedDog.obj");

    //pasto
    Model pasto((char*)"Models/grass.obj");
    
    //calabaza2
    Model calabaza2((char*)"Models/pumpkin.obj");

    //silla2
    Model silla2((char*)"Models/silla.obj");

    //vallaBlanca
    Model valla((char*)"Models/valla.obj");

    //calabaza3
    Model calabaza3((char*)"Models/pumpkin.obj");

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    

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
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        // Objeto: mesaJardin
        glm::mat4 model_mesajardin(1.0f);
        model_mesajardin = glm::translate(model_mesajardin, glm::vec3(0.0214f, -0.0004f, 2.5f));
        model_mesajardin = glm::scale(model_mesajardin, glm::vec3(1.000000f, 1.000000f, 1.000000f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_mesajardin));
        mesaJardin.Draw(shader);

        // Objeto: arbol
        glm::mat4 model_arbol(1.0f);
        model_arbol = glm::translate(model_arbol, glm::vec3(0.4715f, -0.0496f, -0.4152f));
        model_arbol = glm::scale(model_arbol, glm::vec3(0.0333f, 0.0186f, 0.0391f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_arbol));
        arbol.Draw(shader);

        // Objeto: manzana
        glm::mat4 model_manzana(1.0f);
        model_manzana = glm::translate(model_manzana, glm::vec3(2.8176f, 0.0090f, 2.6121f));
        model_manzana = glm::scale(model_manzana, glm::vec3(0.000727f, 0.000727f, 0.000727f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_manzana));
        manzana.Draw(shader);

        // Objeto: calabaza1
        glm::mat4 model_calabaza(1.0f);
        model_calabaza = glm::translate(model_calabaza, glm::vec3(0.3006f, -0.0037f, -0.7141f));
        model_calabaza = glm::scale(model_calabaza, glm::vec3(0.0067f, 0.0100f, 0.0068f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_calabaza));
        calabaza1.Draw(shader);

        // Objeto: perrito1
        glm::mat4 model_perrito1(1.0f);
        model_perrito1 = glm::translate(model_perrito1, glm::vec3(-0.2472f, 0.3703f, 0.7365f));
        model_perrito1 = glm::scale(model_perrito1, glm::vec3(1.000000f, 1.000000f, 1.000000f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_perrito1));
        perrito1.Draw(shader);

        // Objeto: silla1
        glm::mat4 model_silla1(1.0f);
        model_silla1 = glm::translate(model_silla1, glm::vec3(1.4788f, 0.0157f, -1.0240f));
        model_silla1 = glm::scale(model_silla1, glm::vec3(0.0495f, 0.0339f, 0.0361f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_silla1));
        silla1.Draw(shader);

        // Objeto: perrito2
        glm::mat4 model_perrito2(1.0f);
        model_perrito2 = glm::translate(model_perrito2, glm::vec3(1.6896f, 0.3592f, 1.9271f));
        model_perrito2 = glm::scale(model_perrito2, glm::vec3(1.000000f, 1.000000f, 1.000000f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_perrito2));
        perrito2.Draw(shader);

        // Objeto: pasto
        glm::mat4 model_pasto(1.0f);
        model_pasto = glm::translate(model_pasto, glm::vec3(0.0000f, 0.0000f, -0.0000f));
        model_pasto = glm::scale(model_pasto, glm::vec3(0.010829f, 0.001376f, 0.010579f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_pasto));
        pasto.Draw(shader);

        // Objeto: calabaza2
        glm::mat4 model_calabaza_001(1.0f);
        model_calabaza_001 = glm::translate(model_calabaza_001, glm::vec3(1.0121f, -0.0017f, -0.1457f));
        model_calabaza_001 = glm::scale(model_calabaza_001, glm::vec3(0.0067f, 0.0100f, 0.0068f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_calabaza_001));
        calabaza2.Draw(shader);

        // Objeto: silla2
        glm::mat4 model_silla2(1.0f);
        model_silla2 = glm::translate(model_silla2, glm::vec3(2.5288f, 0.0145f, -0.5500f));
        model_silla2 = glm::scale(model_silla2, glm::vec3(0.0495f, 0.0339f, 0.0361f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_silla2));
        silla2.Draw(shader);

        // Objeto: vallaBlanca
        glm::mat4 model_vallablanca(1.0f);
        model_vallablanca = glm::translate(model_vallablanca, glm::vec3(0.2667f, 0.001f, 0.1f));
        model_vallablanca = glm::scale(model_vallablanca, glm::vec3(1.000000f, 1.000000f, 1.000000f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_vallablanca));
        valla.Draw(shader);

        // Objeto: calabaza3
        glm::mat4 model_calabaza_002(1.0f);
        model_calabaza_002 = glm::translate(model_calabaza_002, glm::vec3(4.7605f, -0.0019f, -1.4147f));
        model_calabaza_002 = glm::scale(model_calabaza_002, glm::vec3(0.0067f, 0.0100f, 0.0068f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_calabaza_002));
        calabaza3.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }




}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

