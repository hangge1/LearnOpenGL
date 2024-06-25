
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"

#include "Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


GLFWwindow* InitWindow(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    //ʵ����Ҳ�ɽ��ӿ�����Ϊ��GLFW�Ĵ���С
    //���ʣ���Ļ�ռ�任
    glViewport(0, 0, width, height);

    //���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;

    GLFWwindow* window = InitWindow(screenWidth, screenHeight, "Hello World");
    if (window == nullptr)
    {
        std::cout << "InitWindow Error!" << std::endl;
        return -1;
    }

    

    float vertices[] = 
    {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
    };

    unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };


    //����VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //���ɲ���VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //����IBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���Ͷ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    Shader shader("assets/shader/default.vs", "assets/shader/default.fs");
    shader.Bind();
    shader.SetUniform1i("sample", 0);

    Texture texture("assets/textures/people.jpg");
    texture.Bind(0);



    while (!glfwWindowShouldClose(window)) //��Ⱦѭ��
    {
        processInput(window);

        //Clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //��ת90�㣬����һ��
        glm::mat4 model(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view(1.0f);
        // ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);

        glm::mat4 transform = projection * view * model;

        shader.SetUniform4mat("u_Transform", transform);

        //Draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}