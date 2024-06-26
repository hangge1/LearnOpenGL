
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

#include "Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

//void loadModel(const std::string& path)
//{
//    Assimp::Importer import;
//    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//    {
//        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
//        return;
//    }
//    //directory = path.substr(0, path.find_last_of('/'));
//
//    //processNode(scene->mRootNode, scene);
//}

int main()
{
    //loadModel("assets/models/nanosuit/nanosuit.obj");
    //return 0;


    int screenWidth = 800;
    int screenHeight = 600;

    GLFWwindow* window = InitWindow(screenWidth, screenHeight, "Hello World");
    if (window == nullptr)
    {
        std::cout << "InitWindow Error!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    float vertices[] = 
    {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    //生成VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   /* Texture texture("assets/textures/wall.jpg");
    texture.Bind(0);*/

    glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

    //生成表示物体的立方体
    unsigned int objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //生成表示灯光的立方体
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    Shader lightshader("assets/shader/lightShader.vs", "assets/shader/lightShader.fs");
    Shader objectshader("assets/shader/objectShader.vs", "assets/shader/objectShader.fs");

    Texture diffuse_texture("assets/textures/diffuseMap.png");
    Texture specular_texture("assets/textures/specularMap.png");
    diffuse_texture.Bind(0);
    specular_texture.Bind(1);

    glm::vec3 cubePositions[] = 
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = 
    {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    while (!glfwWindowShouldClose(window)) //渲染循环
    {
        processInput(window);

        //Clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)screenWidth / screenHeight, 0.1f, 100.0f);


        //渲染灯
        lightshader.Bind();
        lightshader.SetUniform4mat("u_View", view);
        lightshader.SetUniform4mat("u_Project", projection);
        for (size_t i = 0; i < 4; i++)
        {
            glm::mat4 lightmodel(1.0f);
            lightmodel = glm::translate(lightmodel, pointLightPositions[i]);
            lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
            lightshader.SetUniform4mat("u_Model", lightmodel);
            
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        //渲染物体
        objectshader.Bind();
        objectshader.SetUniform3f("u_ViewPos", camera.GetPos());

        //材质颜色
        objectshader.SetUniform1i("u_Material.diffuse", 0);
        objectshader.SetUniform1i("u_Material.specular", 1);
        objectshader.SetUniform1f("u_Material.shininess", 32);

        //光照属性
        //1、定向光源
        objectshader.SetUniform3f("u_DirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        objectshader.SetUniform3f("u_DirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        objectshader.SetUniform3f("u_DirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        objectshader.SetUniform3f("u_DirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        //2、点光源
        //(0)
        objectshader.SetUniform3f("u_PointLights[0].position", pointLightPositions[0]);
        objectshader.SetUniform3f("u_PointLights[0].ambient",   glm::vec3(0.05f, 0.05f, 0.05f));
        objectshader.SetUniform3f("u_PointLights[0].diffuse",   glm::vec3(0.8f, 0.8f, 0.8f));
        objectshader.SetUniform3f("u_PointLights[0].specular",  glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform1f("u_PointLights[0].constant",  1.0f);
        objectshader.SetUniform1f("u_PointLights[0].linear", 0.09f);
        objectshader.SetUniform1f("u_PointLights[0].quadratic", 0.032f);
        //(1)
        objectshader.SetUniform3f("u_PointLights[1].position", pointLightPositions[1]);
        objectshader.SetUniform3f("u_PointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        objectshader.SetUniform3f("u_PointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        objectshader.SetUniform3f("u_PointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform1f("u_PointLights[1].constant", 1.0f);
        objectshader.SetUniform1f("u_PointLights[1].linear", 0.09f);
        objectshader.SetUniform1f("u_PointLights[1].quadratic", 0.032f);
        //(2)
        objectshader.SetUniform3f("u_PointLights[2].position", pointLightPositions[2]);
        objectshader.SetUniform3f("u_PointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        objectshader.SetUniform3f("u_PointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        objectshader.SetUniform3f("u_PointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform1f("u_PointLights[2].constant", 1.0f);
        objectshader.SetUniform1f("u_PointLights[2].linear", 0.09f);
        objectshader.SetUniform1f("u_PointLights[2].quadratic", 0.032f);
        //(3)
        objectshader.SetUniform3f("u_PointLights[3].position", pointLightPositions[3]);
        objectshader.SetUniform3f("u_PointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        objectshader.SetUniform3f("u_PointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        objectshader.SetUniform3f("u_PointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform1f("u_PointLights[3].constant", 1.0f);
        objectshader.SetUniform1f("u_PointLights[3].linear", 0.09f);
        objectshader.SetUniform1f("u_PointLights[3].quadratic", 0.032f);

        //3、聚光灯
        objectshader.SetUniform3f("u_SpotLight.position", camera.GetPos());
        objectshader.SetUniform3f("u_SpotLight.direction", camera.GetFront());
        objectshader.SetUniform1f("u_SpotLight.cutOff", glm::cos(glm::radians(5.0f)));
        objectshader.SetUniform1f("u_SpotLight.outerCutOff", glm::cos(glm::radians(7.0f)));
        objectshader.SetUniform1f("u_SpotLight.constant", 1.0f);
        objectshader.SetUniform1f("u_SpotLight.linear", 0.09f);
        objectshader.SetUniform1f("u_SpotLight.quadratic", 0.032f);
        objectshader.SetUniform3f("u_SpotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        objectshader.SetUniform3f("u_SpotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform3f("u_SpotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        objectshader.SetUniform4mat("u_View", view);
        objectshader.SetUniform4mat("u_Project", projection);

        for (size_t i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            objectshader.SetUniform4mat("u_Model", model); 
            glBindVertexArray(objectVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glfwPollEvents();
        glfwSwapBuffers(window);

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objectVAO);

    glfwTerminate();
    return 0;
}