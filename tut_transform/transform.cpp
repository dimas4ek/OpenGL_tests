#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../source_files/shader_s.h"

#include <algorithm>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"//layout(location = 1) in vec2 aTexCoord;\n"
"//out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos, 2.f);\n"
"	ourColor = aColor;\n"
"	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"//in vec2 TexCoord;\n"
"in vec3 ourColor;\n"
"//uniform sampler2D texture1;\n"
"//uniform sampler2D texture2;\n"
"void main()\n"
"{\n"
"	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
"	FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int screen_w = 800;
const int screen_h = 500;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screen_w, screen_h, "transform", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init GLAD\n";
		return -1;
	}

	Shader shader(vertexShaderSource, fragmentShaderSource);

	// Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
	float vertices1[] = {
		// координаты        // текстурные координаты
		0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, //1.0f, 1.0f, // верхняя правая вершина
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, //1.0f, 0.0f, // нижняя правая вершина
	   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, //0.0f, 0.0f, // нижняя левая вершина
	   -0.5f,  0.5f, 0.0f,	1.f, .5f, 0.f,    //0.0f, 1.0f  // верхняя левая вершина
	};
	unsigned int indices1[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};


	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	
	shader.use();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 transform = glm::mat4(1.f); //инициализация единичной матрицы
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); //перенос в правый нижний угол
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(.0f, .0f, 1.f)); //

		
		//тупое вращение
		/*transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3((float)std::cos(glfwGetTime()-1), (float)std::sin(1), .0f));
		transform = glm::translate(transform, glm::vec3(.3f, .3f, 0.f));
		transform = glm::scale(transform, glm::vec3((float)std::sin(90), 1.f, (float)(glfwGetTime())));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3((float)std::cos(glfwGetTime()*2), (float)std::sin(1), .0f));
		transform = glm::translate(transform, glm::vec3(-.3f, .3f, 0.f));*/

		unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			//1 - местонахождение uniform-переменной
			//2 - сколько матриц надо отправить
			//3 - транспортирование матрицы
			//4 - фактические данные матрицы (лучше применять glm::value_ptr() - он лучше)

		shader.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = glm::mat4(1.f);
		transform = glm::translate(transform, glm::vec3( - 0.5f, 0.5f, 0.f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.f, 0.f, -1.f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = glm::mat4(1.f);
		transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.f));
		transform = glm::scale(transform, glm::vec3(1.f, 0.5f, 0.f));
		transform = glm::rotate(transform, (float)sin(glfwGetTime()), glm::vec3(0.f, 0.f, -1.f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}