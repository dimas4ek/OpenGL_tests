#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Раскомментируйте данную часть кода, если используете macOS
    /*
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    */

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Построение и компилирование нашей шейдерной программы

    // На этот раз мы пропустили проверку ошибок компилирования для удобства чтения (если у вас возникнут какие-либо проблемы, то добавьте код проверки ошибок компилирования (смотрите предыдущие примеры))
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // первый фрагментный шейдер, который выводит оранжевый цвет
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // второй фрагментный шейдер, который выводит желтый цвет
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram(); // вторая шейдерная программа
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);

    // Связываем первый программный объект
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    // Затем связываем второй программный объект, используя другой фрагментный шейдер (но при этом тот же вершинный шейдер).
    // Это разрешено, так как данные ввода и вывода как вершинного, так и фрагментного шейдеров совпадают
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // Указывание вершин (и буферов) и настройка вершинных атрибутов
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // слева
        -0.0f, -0.5f, 0.0f,  // справа
        -0.45f, 0.5f, 0.0f,  // вверху
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // слева
        0.9f, -0.5f, 0.0f,  // справа
        0.45f, 0.5f, 0.0f   // вверху
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // мы также можем генерировать несколько VAO или буферов одновременно
    glGenBuffers(2, VBOs);

    // Настройка первого треугольника
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // вершинные атрибуты остаются прежними
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // нет необходимости выполнять отвязку, так как мы напрямую связываем другой VAO в следующих нескольких строках кода

    // Настройка второго треугольника
    glBindVertexArray(VAOs[1]);	// обратите внимание, что мы выполняем привязку другого VAO сейчас
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // и другого VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // поскольку данные вершин плотно упакованы, то мы также можем указать 0 в качестве шага вершинного атрибута
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // это, на самом деле, не обязательно, но остерегайтесь вызовов, которые могут повлиять на VAO, пока он связан (как связывание объектов элементного буфера или подключение/отключение вершинных атрибутов)

    // Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Обработка ввода
        processInput(window);

        // Рендеринг
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Теперь, когда мы рисуем треугольник, нам сначала нужно использовать вершину и оранжевый фрагментный шейдер из первой программы
        glUseProgram(shaderProgramOrange);

        // Рисуем первый треугольник, используя данные из нашего первого VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // этот вызов должен вывести оранжевый треугольник

        // Затем рисуем второй треугольник, используя данные из второго VAO.
        // Когда мы рисуем второй треугольник, то мы хотим использовать другую шейдерную программу, поэтому мы переключаемся на шейдерную программу с желтым фрагментным шейдером
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // этот вызов должен нарисовать желтый треугольник

        // glfw: обмен содержимым front- и back-буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, высота окна на Retina-дисплеях будет значительно больше, чем указано в программе
    glViewport(0, 0, width, height);
}