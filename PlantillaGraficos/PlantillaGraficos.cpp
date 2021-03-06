// PlantillaGraficos.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Vertice.h"
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

using namespace std;
using namespace glm;

//Cada elemeto que queramos renderear necesita un vertex array y un buffer
vector<Vertice> triangulo;
mat4 transformacionesTriangulo;
GLuint vertexArrayTrianguloID;
GLuint bufferTrianguloID;

vector<Vertice> cuadrado;
GLuint vertexArrayCuadradoID;
GLuint bufferCuadradoID;

//Instancia del shader
Shader *shader;
//Identificadores para mapeo de atributos de entradea del vertex shader
GLuint posicionID;
GLuint colorID;
GLuint transformacionesID;

void inicializarCuadrado() {
	Vertice v1 = {
		vec4(-0.2f,0.2f,0.0f,1.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};

	Vertice v2 = {
		vec4(0.2f,0.2f,0.0f,1.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};

	Vertice v3 = {
		vec4(0.2f,-0.2f,0.0f,1.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};

	Vertice v4 = {
		vec4(-0.2f,-0.2f,0.0f,1.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};
	cuadrado.push_back(v1);
	cuadrado.push_back(v2);
	cuadrado.push_back(v3);
	cuadrado.push_back(v4);
}

void inicializarTriangulo() {
	Vertice v1 = {
		vec4(0.0f, 0.3f, 0.0f,1.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	Vertice v2 = {
		vec4(-0.3f, -0.3f, 0.0f,1.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	Vertice v3 = {
		vec4(0.3, -0.3, 0.0f,1.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	triangulo.push_back(v1);
	triangulo.push_back(v2);
	triangulo.push_back(v3);
	transformacionesTriangulo = mat4(1.0f);
}

void dibujar() {
	//Elegir shader
	shader->enlazar();
	//Elegir el vertex array
	glBindVertexArray(vertexArrayTrianguloID);
	//Dibujar
	glDrawArrays(GL_TRIANGLES, 0, triangulo.size());

	//Proceso dibujo de cuadrado
	glBindVertexArray(vertexArrayCuadradoID);
	glDrawArrays(GL_QUADS, 0, cuadrado.size());

	//Soltar vertex array
	glBindVertexArray(0);
	//Desenlazar shader
	shader->desenlazar();
}

int main()
{
	//declarar una ventana
	GLFWwindow* window;

	//Si no se pudo iniciar glfw
	//terminamos ejecucion
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo inicial glfw
	//inicializamos la ventana
	window = glfwCreateWindow(600, 600, "Ventana", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Establecemos la ventana como contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido el contexto se activan las funciones "modernas" (GPU)
	glewExperimental = true;

	GLenum errores = glewInit();
	if (errores != GLEW_OK)
	{
		glewGetErrorString(errores);
	}

	const GLubyte* versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;

	inicializarTriangulo();
	inicializarCuadrado();

	const char* rutaVertexShader = "VertexShader.Shader";
	const char* rutaFragmentShader = "FragmentShader.Shader";
	shader = new Shader(rutaVertexShader, rutaFragmentShader);

	//Mapeo de atributos
	posicionID = glGetAttribLocation(shader->getID(), "posicion");
	colorID = glGetAttribLocation(shader->getID(), "color");
	transformacionesID = glGetUniformLocation(shader->getID, "transformaciones");

	shader->desenlazar();

	//Crear el vertex array del triangulo
	glGenVertexArrays(1, &vertexArrayTrianguloID);
	glBindVertexArray(vertexArrayTrianguloID);

	//Vertex buffer
	glGenBuffers(1, &bufferTrianguloID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferTrianguloID);

	//Asociar datos al bufer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * triangulo.size(), triangulo.data(), GL_STATIC_DRAW);

	//Habilitar atributo de shader
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);

	//Especificar a OpenGL como comunicarse
	glVertexAttribPointer(posicionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)sizeof(vec3));

	//Proceso de inicializar vertex array para el cuadrado
	glGenVertexArrays(1, &vertexArrayCuadradoID);
	glBindVertexArray(vertexArrayCuadradoID);
	glGenBuffers(1, &bufferCuadradoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCuadradoID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * cuadrado.size(), cuadrado.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(posicionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)sizeof(vec3));

	//Soltar el vertex array y el buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Establecer region de dibujo
		glViewport(0, 0, 600, 600);
		//Establecemos el color de borrado
		//Valores RGBA
		glClearColor(0.5, 0.2, 0.5, 1);
		//Borrar!
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Actualizar valores y dibujar
		dibujar();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	//Despues del ciclo de dibujo
	//Eliminamos ventana y procesos de glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}

