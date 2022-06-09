#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	//инициализация GLFW
	glfwInit();

	//говорим GLFW какую версию мы используем
	//3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//говорим GLFW что мы используем профиль CORE
	//это значит, у нас есть современные функции
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//создание окна GLFW
	GLFWwindow* window = glfwCreateWindow(800, 500, "OpenGL", nullptr, nullptr);
	//проверка на ошибку, если окно не создалось
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	//делаем контекст нашего окна основным в текущем потоке
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//загружаем GLAD, чтобы сконфигурировать OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD\n";
		return -1;
	}

	//размер видимой области окна, через которую юзер может наблюдать
	//за процессом рендеринга и отображения картинки
	glViewport(0, 0, 800, 800);
	//0, 0 - координаты левого нижнего угла просмотра
	//800, 800 - ширина и высота для рендеринга окна


	//основной цикл 
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//цвет заднего фона
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//очищаем цветовой буфер и меняем на новый
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		//события GLFW
		glfwPollEvents();
	}

	//удаляем окно перед завершением программы
	glfwDestroyWindow(window);
	//прекращаем GLFW перед завершением программы
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