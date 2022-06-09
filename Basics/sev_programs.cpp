#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���������
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
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ���������������� ������ ����� ����, ���� ����������� macOS
    /*
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    */

    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: �������� ���� ���������� �� OpenGL-�������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ���������� � �������������� ����� ��������� ���������

    // �� ���� ��� �� ���������� �������� ������ �������������� ��� �������� ������ (���� � ��� ��������� �����-���� ��������, �� �������� ��� �������� ������ �������������� (�������� ���������� �������))
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // ������ ����������� ������, ������� ������� ��������� ����
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // ������ ����������� ������, ������� ������� ������ ����
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram(); // ������ ��������� ���������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);

    // ��������� ������ ����������� ������
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    // ����� ��������� ������ ����������� ������, ��������� ������ ����������� ������ (�� ��� ���� ��� �� ��������� ������).
    // ��� ���������, ��� ��� ������ ����� � ������ ��� ����������, ��� � ������������ �������� ���������
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // ���������� ������ (� �������) � ��������� ��������� ���������
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // �����
        -0.0f, -0.5f, 0.0f,  // ������
        -0.45f, 0.5f, 0.0f,  // ������
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // �����
        0.9f, -0.5f, 0.0f,  // ������
        0.45f, 0.5f, 0.0f   // ������
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // �� ����� ����� ������������ ��������� VAO ��� ������� ������������
    glGenBuffers(2, VBOs);

    // ��������� ������� ������������
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ��������� �������� �������� ��������
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // ��� ������������� ��������� �������, ��� ��� �� �������� ��������� ������ VAO � ��������� ���������� ������� ����

    // ��������� ������� ������������
    glBindVertexArray(VAOs[1]);	// �������� ��������, ��� �� ��������� �������� ������� VAO ������
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // � ������� VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // ��������� ������ ������ ������ ���������, �� �� ����� ����� ������� 0 � �������� ���� ���������� ��������
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // ���, �� ����� ����, �� �����������, �� ������������� �������, ������� ����� �������� �� VAO, ���� �� ������ (��� ���������� �������� ����������� ������ ��� �����������/���������� ��������� ���������)

    // ���������������� ��������� ������ ��� ��������� ��������� � ������ �������
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        // ��������� �����
        processInput(window);

        // ���������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ������, ����� �� ������ �����������, ��� ������� ����� ������������ ������� � ��������� ����������� ������ �� ������ ���������
        glUseProgram(shaderProgramOrange);

        // ������ ������ �����������, ��������� ������ �� ������ ������� VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // ���� ����� ������ ������� ��������� �����������

        // ����� ������ ������ �����������, ��������� ������ �� ������� VAO.
        // ����� �� ������ ������ �����������, �� �� ����� ������������ ������ ��������� ���������, ������� �� ������������� �� ��������� ��������� � ������ ����������� ��������
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // ���� ����� ������ ���������� ������ �����������

        // glfw: ����� ���������� front- � back-�������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ ���� �� Retina-�������� ����� ����������� ������, ��� ������� � ���������
    glViewport(0, 0, width, height);
}