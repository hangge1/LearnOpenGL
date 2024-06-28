
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Shader.h"

#include "Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Camera.h"

#include "Model.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

Camera camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
        glfwSetWindowShouldClose(window, true);

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

    //启动深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //设置面剔除(默认是逆时针正向，剔除背面)
    /*glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);*/

    float cubeVertices[] = 
    {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float planeVertices[] = 
    {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    {
        // cube VAO
        VertexArray cubeVAO;
        VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
        VertexBufferLayout cubeLayout;
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(2);
        cubeVAO.AddBuffer(cubeVBO, cubeLayout);

        // plane VAO
        VertexArray planeVAO;
        VertexBuffer planeVBO(planeVertices, sizeof(planeVertices));
        VertexBufferLayout planeLayout;
        planeLayout.Push<float>(3);
        planeLayout.Push<float>(2);
        planeVAO.AddBuffer(planeVBO, planeLayout);


        Texture cubeTexture("assets/textures/diffuseMap.png");
        Texture floorTexture("assets/textures/wall.jpg");

        cubeTexture.Bind(0);
        floorTexture.Bind(1);

        Shader shader("assets/shader/stencil_testing.vs", "assets/shader/stencil_testing.fs");

        Renderer renderer;

        while (!glfwWindowShouldClose(window)) //渲染循环
        {
            processInput(window);

            //Clear
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //======================================================Draw
            glm::mat4 model(1.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);



            //========绘制轮廓========
            //1、绘制地板
            floorTexture.Bind();
            shader.Bind();
            shader.SetUniform4mat("view", view);
            shader.SetUniform4mat("projection", projection);
            shader.SetUniform4mat("model", glm::mat4(1.0f));
            renderer.Draw(planeVAO, shader, 6);


            //2、绘制立方体
            cubeTexture.Bind();
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            shader.SetUniform4mat("model", model);
            renderer.Draw(cubeVAO, shader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            shader.SetUniform4mat("model", model);
            renderer.Draw(cubeVAO, shader, 36);



            //End======================================================Draw
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