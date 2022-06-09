#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../source_files/shader_s.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const int screen_w = 800;
const int screen_h = 500;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourPosition;\n"
"out vec3 ourColor;\n"
"uniform float xOffset;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);\n"
"   //ourColor = aColor;\n"
"	ourPosition = aPos;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec3 ourPosition;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourPosition, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screen_w, screen_h, "shaders", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window\n";
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	//создание шейдера при помощи класса Shader
	Shader ourShader(vertexShaderSource, fragmentShaderSource); //путь к файлам

	float vertices[] = {
		 0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 
		-0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
		 0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//координатный атрибут
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//цветовой атрибут
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		
		float offset = 0.5f;
		ourShader.setFloat("xOffset", offset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	/*if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		offset += 0.1f;
		shader.setFloat("xOffset", offset);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		offset -= 0.1f;
		shader.setFloat("xOffset", offset);
	}*/
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}