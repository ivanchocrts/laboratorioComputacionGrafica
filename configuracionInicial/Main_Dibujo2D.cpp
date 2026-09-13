#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificacion de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	//Nombre que tendrá la ventana. *Recuerda actualizarlo con tu nombre.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D. Ivan Daniel", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificacion de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificacion de errores de inicializacion de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacion de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	//Aqui se está cragando la información de los archivos que se tienen por separado: el core .frag y el .vs
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	//Definición de los vértices que se van a ocupar 
	//La primera parte hace referencia a los vérices X, Y, Z, como se trata de un dibujo en 2D
	//los cuatro renglones tienen en la última componente el valor de 0. 
	//
	//La segunda parte hace referencia al color que tendrá cada vértice. Están en RGB.
	float vertices[] = {
		//Este "espacio de dubujo" va de -1 a 1 tanto para el eje x como para el y
		//Un vértice es un punto, en este caso con coordenadas en x y y 
		 0.5f,  0.5f, 0.5f,   0.0f,1.0f,0.0f,  // top right     //Elemento 0 del arreglo
		 0.5f, -0.5f, 0.0f,   1.0f,1.0f,0.0f,  // bottom right  //Elemento 1 
		-0.5f, -0.5f, 0.0f,   1.0f,0.0f,1.0f,  // bottom left   //Elemento 2
		-0.5f,  0.5f, 0.0f,   1.0f,1.0f,0.0f,  // top left      //Elemento 3

		//Para dibujar un cangrejo
		 0.4f,  0.4f, 0.0f,   1.0f,0.0f,0.0f,  // top right   //Elemento 4
		 0.4f, -0.4f, 0.0f,   1.0f,0.5f,0.0f,  // bottom right  //Elemento 5 
		-0.4f, -0.4f, 0.0f,   0.0f,0.0f,1.0f,  // bottom left   //Elemento 6
		-0.4f,  0.4f, 0.0f,   0.0f,1.0f,1.0f,  // top left      //Elemento 7... Vértices que forman un cuadrado

		-0.4f,  0.4f, 0.0f,  1.0f,1.0f,0.0f,
		-0.7f,  0.0f, 0.0f,  1.0f,1.0f,0.5f,
		-0.4f, -0.4f, 0.0f,  0.0f,1.0f,0.0f,                                     //Vértices que forman el triángulo izquierdo

		 0.4f,  0.4f, 0.0f,  1.0f,1.0f,0.0f,
		 0.7f,  0.0f, 0.0f,  1.0f,0.5f,1.0f,
		 0.4f, -0.4f, 0.0f,  1.0f,0.5f,0.5f,                                     //Vértices que forman el triángulo derecho

		-0.7f,  0.0f, 0.0f,  0.5f,0.5f,0.0f,
		-0.4,  -0.6f, 0.0f,  0.5f,0.0f,0.5f,
		-0.4,  -0.4f, 0.0f,  1.0f,0.5f,0.0f,                                     //Pata izquierda

		 0.7f,  0.0f, 0.0f,  0.5f,0.5f,0.0f,
		 0.4f, -0.6f, 0.0f,  0.5f,0.0f,0.5f,
		 0.4f,  0.4f, 0.0f,  1.0f,0.5f,0.0f,                                     //Pata derecha 

		-0.9f,  0.4f, 0.0f,  1.0f,0.0f,0.5f,
		-0.55f, 0.2f, 0.0f,  1.0f,0.5f,0.5f,
		-0.4f,  0.4f, 0.0f,  1.0f,1.0f,0.0f,                                     //Tenaza izquierda parte de abajo

		 0.9f,  0.4f, 0.0f,  1.0f,0.0f,0.5f,
		 0.55f, 0.2f, 0.0f,  1.0f,0.5f,0.5f,
		 0.4f,  0.4f, 0.0f,  1.0f,1.0f,0.0f,                                     //Tenaza derecha parte de abajo

		-0.4f,  0.6f, 0.0f,  0.0f,1.0f,0.0f,
		-0.9f,  0.4f, 0.0f,  1.0f,0.5f,0.0f,
		-0.6f,  0.4f, 0.0f,  0.5f,1.0f,0.0f,                                     //Tenaza izquierda parte de arriba

		 0.4f,  0.6f, 0.0f,  0.0f,1.0f,0.0f,
		 0.9f,  0.4f, 0.0f,  1.0f,0.5f,0.0f,
		 0.6f,  0.4f, 0.0f,  0.5f,1.0f,0.0f,                                     //Tenaza derecha parte de arriba             
	};

	//Sección de índices de cada vértice
	//Estos índices se utilizan cuando no deseo seguir el orden con el que fueron creados en float vertices[]
	unsigned int indices[] = {   //note that we start from 0!
		3,2,1,// second Triangle //Elemento 0 del arreglo
		0,1,3,                   //Elemento 1 del arreglo
		2,3,0,					 //Elemento 2 del arreglo...
		0,1,2,
	};



	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		//Actividades iniciales (para el previo)
		//1. PARA DIBUJAR PUNTOS
		//glPointSize(10);              //Para modificar el tamaño del punto
		//glDrawArrays(GL_POINTS,0,4);  //A partir de qué elemento del arreglo se va a dibujar, cuántos elementos del arreglo se van a ocupar.

		//2. PARA DIBUJAR LÍNEAS
		//glDrawArrays(GL_LINES,0,4);      //A partir de qué elemento del arreglo se va a dibujar, cuántos elementos del arreglo se van a ocupar.
		//¿qué pasa con (GL_LINES,3,2), no se tendría que dibujar la línea superior, por qué no lo hace?
		// glDrawsArray(GL_LINES,0,2);
		// La función anterior sólo lleva a acabo la unión de 2 vértices, la siguiente permite dibujar el rectángulo completo. Permite cerrar los
		// vértices.
		//glDrawArrays(GL_LINE_LOOP,0,4);
		//¿Realmente existe la línea superior o solo se está cerrando el rectángulo?
		//Recuerda que los argumentos hacen referencia a vértices o puntos NO a las líneas!!!

		//3. PARA DIBUJAR TRIÁNGULOS
		//glDrawArrays(GL_TRIANGLES,0,3);

		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,0);
		//Estas definiciones permiten elegir que triangulo dibujar
		//glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)(sizeof(unsigned int)*3));
		//glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)(sizeof(unsigned int)*6));
		//glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)(sizeof(unsigned int)*9));

		//Instrucciones para dibujar al cangrejo (únicamente con líneas)

		//glDrawArrays(GL_LINE_LOOP,4, 4);

		//glDrawArrays(GL_LINE_LOOP,8, 3);
		//glDrawArrays(GL_LINE_LOOP,11,3);
		//
		//glDrawArrays(GL_LINE_LOOP,14,3);
		//glDrawArrays(GL_LINE_LOOP,17,3);
		//
		//glDrawArrays(GL_LINE_LOOP,20, 3);
		//glDrawArrays(GL_LINE_LOOP,23, 3);
		//
		//glDrawArrays(GL_LINE_LOOP, 26, 3);
		//glDrawArrays(GL_LINE_LOOP, 29, 3);


		//Acá el cangrejo se dibuja con color
		//Se utilizo esta función para dibujar el cuadrado con relleno
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

		glDrawArrays(GL_TRIANGLES, 8, 4);
		glDrawArrays(GL_TRIANGLES, 11, 3);

		glDrawArrays(GL_TRIANGLES, 14, 3);
		glDrawArrays(GL_TRIANGLES, 17, 3);

		glDrawArrays(GL_TRIANGLES, 20, 3);
		glDrawArrays(GL_TRIANGLES, 23, 3);

		glDrawArrays(GL_TRIANGLES, 26, 3);
		glDrawArrays(GL_TRIANGLES, 29, 3);


		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}