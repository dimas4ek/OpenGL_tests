#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//���. ��� ���������� �������
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//���. ��� ������������ �������
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "triangle", nullptr, nullptr);
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
		std::cout << "failed to initialize GLAD\n";
		return -1;
	}

	#pragma region Triangle
/////////////////////////////////////////////////////////////////////////////////////////////

	//��������� ������ 
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	  //1 - ������ ������� ��� ����������
	  //2 - ���-�� �����, ������� �� �������� � �������� ��������� ����
	  //3 - ����������� ���. ��� ���������� �������
	  //4 - ���� ��� nullptr
	glCompileShader(vertexShader);

	//�������� �� ������� ������ �������������� ���������� �������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	//����������� ������
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	//�������� �� ������� ������ ������������ �������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	//���������� ��������
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//�������� �� ������� ������ ��������� ��������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << '\n';
	}

	//�������� ��������, �.�. ��� ��� ��������� � ���������
	glCompileShader(vertexShader);
	glDeleteShader(fragmentShader);

	//���������� ������ (� �������) � ��������� ��������� ���������
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,   //����� �������
		 0.5f, -0.5f, 0.0f,   //������ �������
		 0.0f,  0.5f, 0.0f,    //������� �������
	};

	//������� Vertex Buffer Object � Vertex Array Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//������� ��������� ������ ���������� �������
	glBindVertexArray(VAO);

	//�������� � ������������� ��������� �����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	  //1 - ��� ������
	  //2 - ������ ������ � ������, ������� ���� �������� � �����
	  //3 - ������, ������� ���� ���������
	  //4 - ���������� ��, ��� �� �����, ����� ������ ��������� ����������� �� �������
	  // GL_STREAM_DRAW � ������ ����������� ������ ���� ��� � ������������ ����������� ����������� �� ����� ���������� ���.
	  // GL_STATIC_DRAW � ������ ����������� ������ ���� ��� � ������������ ����� ���.
	  // GL_DYNAMIC_DRAW � ������ ����� ���������� � ������������ ����� ���.

	//������������� ��������� �������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	  //1 - ���������, ����� ������� ������� ���� ������������ - layout (location = 0) => 0
	  //2 - ���������� ������ ���������� �������� - vec3 => 3
	  //3 - ��������� ��� ������ GL_FLOAT (�.�. vec* � GLSL ������� �� �������� float)
	  //4 - ���������, ����� �� ��, ����� ���� ������ ���� �������������
	  //5 - ��� (stride) - �.�. ��������� ����� ������ � �������������� ������� ��������� �� ���������� 3 * sizeof(float)
	  //6 - �������� ���� �����, ��� ���������� ������ � ������ => 0
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//����� �������
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		//��������� �����
		processInput(window);

		//���������
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//������ �����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//����� ���������� front- � back-�������
		glfwSwapBuffers(window);
		
		//������������ �������
		glfwPollEvents();
	}

	//������������ ���� ��������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//������������ GLFW ��������
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}