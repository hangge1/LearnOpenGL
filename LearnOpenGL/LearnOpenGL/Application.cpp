
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

#include "CubeTexture.h"

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


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
    const int screenWidth = 800;
    const int screenHeight = 600;

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

    float screenVertices[] =
    {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    std::vector<std::string> faces
    {
        "assets/textures/cubeMap/skybox/right.jpg",
        "assets/textures/cubeMap/skybox/left.jpg",
        "assets/textures/cubeMap/skybox/top.jpg",
        "assets/textures/cubeMap/skybox/bottom.jpg",
        "assets/textures/cubeMap/skybox/front.jpg",
        "assets/textures/cubeMap/skybox/back.jpg"
    };
    
    float skyboxVertices[] = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    {
        //帧缓冲
        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        //绑定颜色纹理附件
        unsigned int colorTexture;
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
        //绑定深度和模板缓冲附件
        //法1: 纹理附件
        /*unsigned int depthStencilTexture;
        glGenTextures(1, &depthStencilTexture);
        glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture, 0);*/
        //法2: 渲染缓冲对象附件
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            return -1;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);



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

        // screen VAO
        VertexArray screenVAO;
        VertexBuffer screenVBO(screenVertices, sizeof(screenVertices));
        VertexBufferLayout screenLayout;
        screenLayout.Push<float>(2);
        screenLayout.Push<float>(2);
        screenVAO.AddBuffer(screenVBO, screenLayout);

        // skyBox VAO
        VertexArray skyboxVAO;
        VertexBuffer skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
        VertexBufferLayout skyboxLayout;
        skyboxLayout.Push<float>(3);
        skyboxVAO.AddBuffer(skyboxVBO, skyboxLayout);

        Texture cubeTexture("assets/textures/diffuseMap.png");
        Texture floorTexture("assets/textures/wall.jpg");
        CubeTexture skyBoxTexture(faces);

        cubeTexture.Bind(0);
        floorTexture.Bind(1);

        Shader shader("assets/shader/stencil_testing.vs", "assets/shader/stencil_testing.fs");
        Shader screenShader("assets/shader/fboRenderShader.vs", "assets/shader/fboRenderShader.fs");
        Shader skyboxShader("assets/shader/skybox.vs", "assets/shader/skybox.fs");

        Renderer renderer;

        while (!glfwWindowShouldClose(window)) //渲染循环
        {
            processInput(window);

            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

            

            //===================第一阶段 离线渲染=====================
            //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            //glEnable(GL_DEPTH_TEST);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //======================================================Draw
            glm::mat4 model(1.0f);

            //========绘制轮廓========
            ////1、绘制地板
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

            //===================渲染天空盒=========================
            glDepthMask(GL_FALSE);
            skyBoxTexture.Bind();
            skyboxShader.Bind();
            glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
            skyboxShader.SetUniform4mat("view", skyboxView);
            skyboxShader.SetUniform4mat("projection", projection);
            renderer.Draw(skyboxVAO, skyboxShader, 36);
            glDepthMask(GL_TRUE);
            //===================End 渲染天空盒=========================
            //===================End 第一阶段 离线渲染=====================
             

            //===================第二阶段 渲染到屏幕=====================
            /*glBindFramebuffer(GL_FRAMEBUFFER, 0);  
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            glBindTexture(GL_TEXTURE_2D, colorTexture);
            renderer.Draw(screenVAO, screenShader, 6);*/
            //===================End 第二阶段 渲染到屏幕=====================


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