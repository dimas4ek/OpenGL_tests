#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//исх. код вершинного шейдера
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//исх. код фрагментного шейдера
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

	//вершинный шейдер 
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	  //1 - объект шейдера для компиляции
	  //2 - кол-во строк, которые мы передаем в качестве исходного кода
	  //3 - фактический исх. код вершинного шейдера
	  //4 - пока что nullptr
	glCompileShader(vertexShader);

	//проверка на наличие ошибок компилирования вершинного шейдера
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	//фрагментный шейдер
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	//проверка на наличие ошибок фрагментного шейдера
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	//связывание шейдеров
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//проверка на наличие ошибок связвания шейдеров
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << '\n';
	}

	//удаление шейдеров, т.к. они уже связались в программе
	glCompileShader(vertexShader);
	glDeleteShader(fragmentShader);

	//указывание вершин (и буферов) и настройка вершинных атрибутов
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,   //левая вершина
		 0.5f, -0.5f, 0.0f,   //правая вершина
		 0.0f,  0.5f, 0.0f,    //верхняя вершина
	};

	//создаем Vertex Buffer Object и Vertex Array Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//сначала связываем объект вершинного массива
	glBindVertexArray(VAO);

	//свзяваем и устанавливаем вершинный буфер
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	  //1 - тип буфера
	  //2 - размер данных в байтах, которые надо передать в буфер
	  //3 - данные, которые надо отправить
	  //4 - определяем то, как мы хотим, чтобы видяха управляла переданными ей данными
	  // GL_STREAM_DRAW — данные указываются только один раз и используются графическим процессором не более нескольких раз.
	  // GL_STATIC_DRAW — данные указываются только один раз и используются много раз.
	  // GL_DYNAMIC_DRAW — данные часто изменяются и используются много раз.

	//конфигурируем веришнный атрибут
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	  //1 - указываем, какой атрибут вершины надо использовать - layout (location = 0) => 0
	  //2 - определяем размер вершинного атрибута - vec3 => 3
	  //3 - указываем тип данных GL_FLOAT (т.к. vec* в GLSL состоит из значений float)
	  //4 - указываем, хотим ли мы, чтобы наши данные были нормализованы
	  //5 - ШАГ (stride) - т.к. следующий набор данных о местоположении вершины находится на расстоянии 3 * sizeof(float)
	  //6 - СМЕЩЕНИЕ того места, где начинаются данные в буфере => 0
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//режим каркаса
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		//обраюотка ввода
		processInput(window);

		//рендеринг
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//рисуем треугольник
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//обмен содержимым front- и back-буферов
		glfwSwapBuffers(window);
		
		//отслеживание событий
		glfwPollEvents();
	}

	//освобождение всех ресурсов
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//освобождение GLFW ресурсов
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