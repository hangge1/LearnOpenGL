
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Shader.h"

#include "Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/quaternion.hpp"

#include "Camera.h"

#include "Model.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "CubeTexture.h"

#include "FrameBuffer.h"

int screenWidth = 800;
int screenHeight = 600;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.MouseCB(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ScrollCB(yoffset);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        // 设置为线框模式
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        //切换成填充模式
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        return;
    }


    int mode = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mode |= (int)Camera::KEYMODE::KEY_W;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mode |= (int)Camera::KEYMODE::KEY_S;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mode |= (int)Camera::KEYMODE::KEY_A;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mode |= (int)Camera::KEYMODE::KEY_D;

    camera.KeyCB(deltaTime, (Camera::KEYMODE)mode);
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

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    //实际上也可将视口设置为比GLFW的窗口小
    //本质：屏幕空间变换
    glViewport(0, 0, width, height);

    //当用户改变窗口的大小的时候，视口也应该被调整
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //隐藏光标，捕捉
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    return window;
}


void display(Shader& shader, VertexArray& va,  float deltaTime)
{  
    static float rotateAngle = 0.0f;
    static float step = 2.0f;

    Renderer renderer;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //rotateAngle += step;

    for (size_t i = 0; i < 16; i++)
    {
        //1 欧拉旋转
        //glm::mat4 model = glm::rotate(glm::mat4(1.0f),glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 model_move = glm::translate(glm::mat4(1.0f), glm::vec3(sin(i), 10 * cos(0.5 * i), 20 * sin(10 * i)));

        glm::mat4 model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));

        //2 四元数旋转
        glm::mat4 model = model_move * model_scale * glm::toMat4(glm::angleAxis(glm::radians(rotateAngle), glm::vec3(glm::vec3(sin(i), cos(i), sin(5 * i)))));
        //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 project = glm::perspective(glm::radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 1000.0f);

        shader.Bind();
        shader.SetUniform4mat("model", model);
        shader.SetUniform4mat("view", camera.GetViewMatrix());
        shader.SetUniform4mat("project", project);

        renderer.Draw(va, shader, 36);
    }
}

int main()
{
    GLFWwindow* window = InitWindow(screenWidth, screenHeight, "Hello World");
    if (window == nullptr)
    {
        std::cout << "InitWindow Error!" << std::endl;
        return -1;
    }

    //启动深度测试
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);


    float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 
         1.0f,  1.0f, -1.0f, 
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f,  1.0f, 
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f, 
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, 
        -1.0f,  1.0f, -1.0f, 
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 
         1.0f, -1.0f,  1.0f, 
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 
         1.0f,  1.0f, -1.0f, 
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f, -1.0f
    };

    Shader shader("assets/shader/testShader.vs", "assets/shader/testShader.fs");


    VertexBuffer vb(vertexPositions, sizeof(vertexPositions));
    VertexBufferLayout layout;
    layout.Push<float>(3);

    VertexArray va;
    va.AddBuffer(vb, layout);
    va.Bind();

    {
        while (!glfwWindowShouldClose(window)) //渲染循环
        {
            processInput(window);

            display(shader, va, deltaTime);

            glfwPollEvents();
            glfwSwapBuffers(window);

            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
    }

    glfwTerminate();
    return 0;
}