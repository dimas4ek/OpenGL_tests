#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	//������������� GLFW
	glfwInit();

	//������� GLFW ����� ������ �� ����������
	//3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//������� GLFW ��� �� ���������� ������� CORE
	//��� ������, � ��� ���� ����������� �������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������� ���� GLFW
	GLFWwindow* window = glfwCreateWindow(800, 500, "OpenGL", nullptr, nullptr);
	//�������� �� ������, ���� ���� �� ���������
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	//������ �������� ������ ���� �������� � ������� ������
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//��������� GLAD, ����� ���������������� OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD\n";
		return -1;
	}

	//������ ������� ������� ����, ����� ������� ���� ����� ���������
	//�� ��������� ���������� � ����������� ��������
	glViewport(0, 0, 800, 800);
	//0, 0 - ���������� ������ ������� ���� ���������
	//800, 800 - ������ � ������ ��� ���������� ����


	//�������� ���� 
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//���� ������� ����
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//������� �������� ����� � ������ �� �����
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		//������� GLFW
		glfwPollEvents();
	}

	//������� ���� ����� ����������� ���������
	glfwDestroyWindow(window);
	//���������� GLFW ����� ����������� ���������
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}