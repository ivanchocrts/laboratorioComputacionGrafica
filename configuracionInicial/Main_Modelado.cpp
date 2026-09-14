#include<iostream>
//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;

//Estas variables permiten manipular la vista de manera directa y así manipular el entorno sin tener que 
//cerrar y compilar el código una y otra vez.
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	//Set all the required options for GLFW
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4. Modelado geometrico. Ivan Daniel", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	//Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	//Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	//Enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	//Set up vertex data (and buffer(s)) and attribute pointers
	//Use with Perspective Projection
	float vertices[] = {
		//Cada cara del cubo está formada por dos triángulos y cada triangulo tiene 3 vértices,
		//es por eso que para cada cara se definen 6 vértices
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, //Front
		 0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,-0.5f,  0.0f, 1.0f, 0.0f, //Back
		 0.5f, -0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,-0.5f,  0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		//En comparación con códigos anteriores aqui no se definen valores, se ocupan los valores que se hayan asignado a las variables (línea 24)
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		//ACTIVIDADES DEL PREVIO
		////SUPERFICIE DE LA MESA
		//model = glm::mat4(1.0f);
		////Aqui se inicia la construcción de la mesa (actividad del vídeo)
		//model = glm::scale(model, glm::vec3(3.0f, 0.1f, 2.0f));//Ancho, grosor y produndidad (Tamaño de la superficie de la mesa)
		//model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Para crear las patas de la mesa, será necesario inicializar nuevamente la matriz de modelo (esto se hace para cada pata? Sí!)
		////Posteriormente se usarán las herramientas de transformación (escala y transformación)

		////PATA 1
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		//model = glm::translate(model, glm::vec3(2.9f, -0.6f, 1.9f));//Posiciona la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////PATA 2
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		//model = glm::translate(model, glm::vec3(-2.9f, -0.6f, 1.9f));//Posiciona la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////PATA 3
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		//model = glm::translate(model, glm::vec3(-2.9f, -0.6f, -1.9f));//Posiciona la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		////PATA 4
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		//model = glm::translate(model, glm::vec3(2.9f, -0.6f, -1.9f));//Posiciona la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(0);
		//// Swap the screen buffers
		//glfwSwapBuffers(window);

		//ACTIVIDADES DEL LABORATORIO
		//Agregue la línea que permite llevar a cabo la rotación, es correcto?
		//Cubo 1: pata izquierda
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));// Rota 90 grados alrededor del eje X
		model = glm::scale(model, glm::vec3(0.28f, 0.1f, 0.4f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-0.6f, -0.6f, 0.25f));//Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cubo 3: pierna izquierda
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));
		model = glm::translate(model, glm::vec3(-1.6f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cubo 2: pata derecha
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota 90 grados alrededor del eje X
		model = glm::scale(model, glm::vec3(0.28f, 0.1f, 0.4f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(0.6f, -0.6f, 0.25f));//Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cubo 4: pierna derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));
		model = glm::translate(model, glm::vec3(1.6f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerpo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.01f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cabeza  
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-0.01f, 1.6f, 0.21f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cresta
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.8f, 0.5f));
		model = glm::translate(model, glm::vec3(-0.15f, 1.6f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pico
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		model = glm::translate(model, glm::vec3(0.1f, 11.0f, 1.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Papada
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(0.1f, 10.0f, 5.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cola
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.1f));
		model = glm::translate(model, glm::vec3(-0.01f, 1.0f, -5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ala izquierda
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.6f));
		model = glm::translate(model, glm::vec3(-3.5f, 2.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ala derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.6f));
		model = glm::translate(model, glm::vec3(3.5f, 2.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		//Swap the screen buffers
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

//Función que permite interactuar con el dispositivo de entrada que son las teclas del teclado del equipo
//El cubo no se mueve, lo que se mueve es la matriz de vista es decir el punto de vusta desde el que se observa el cubo 
void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	//En mi compu la tecla page up es con la que doy siguiente a las canciones
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	//En mi compu la tecla page down es con la que doy atrás a las canciones
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
}