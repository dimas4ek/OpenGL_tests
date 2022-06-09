#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../source_files/shader_s.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int screen_w = 800;
const int screen_h = 500;

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"}\n\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screen_w, screen_h, "coord1", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to create glfw window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init glad\n";
		return -1;
	}

	Shader shader(vertexShaderSource, fragmentShaderSource);

	// �������� ������ (� ������(��)) � ��������� ��������� ���������    
	float vertices[] = {
		// ����������        // �������� ����������
		0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
	   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 
	   -0.5f,  0.5f, 0.0f,	1.f, .5f, 0.f,   
	};
	unsigned int indices[] = {
		0, 1, 3, // ������ �����������
		1, 2, 3  // ������ �����������
	};

	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader.use();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		//������� ������ 
		glm::mat4 model = glm::mat4(1.f); //��������� �������
		model = glm::rotate(model, glm::radians(-70.f), glm::vec3(1.f, 0.f, 0.f)); //������� �� ����������� ����� �� ��� Z
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.f, 0.f, 0.f)); //�������� ������� �� ��� Z

		//������� ����
		glm::mat4 view = glm::mat4(1.f); //��������� �������
		view = glm::translate(view, glm::vec3(0.f, 0.f, -2.f)); //����������� ������� ����� ��� Z

		//������� ��������
		glm::mat4 projection = glm::mat4(1.f); //��������� �������
		projection = glm::perspective(glm::radians(45.f), (float)screen_w / screen_h, .1f, 100.f); //����������� (������� ��������� ��������)
		  //1 - FoV (Filed of View) - ���� ������ (������ ������ 45 ��������)
		  //2 - ����������� ������ (������ / ������)
		  //3 - ���������� �� ��������� ��������� �������� (������ ������ 0.1)
		  //4 - ���������� �� ������� ��������� �������� (������ ������ 100)

		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		shader.setMat4("projection", projection);

		glBindVertexArray(VAO);
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}