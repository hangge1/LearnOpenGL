# Day1

**前置知识**

OpenGL一般被认为是一个图形API，然而，OpenGL本身并不是一个API，它仅仅是一个由[Khronos组织](http://www.khronos.org/)制定并维护的规范。实际的OpenGL库的开发者通常是显卡的生产商，这也意味着任何时候OpenGL库表现的行为与规范规定的不一致时，基本都是库的开发者留下的bug。



**核心模式和立即渲染模式**

早期的OpenGL使用立即渲染模式，即固定渲染管线。它确实容易使用和理解，但是效率太低！

OpenGL3.2开始，规范文档开始废弃立即渲染模式，鼓励开发者在OpenGL核心模式下开发。

当使用OpenGL的核心模式时，OpenGL迫使我们使用现代的函数，现代函数要求使用者真正理解OpenGL和图形编程，它有一些难度，然而提供了更多的灵活性，更高的效率，更重要的是可以更深入的理解图形编程。



**状态机**

OpenGL自身是一个巨大的状态机，一系列的变量描述OpenGL此刻应当如何运行，这些状态集合称为OpenGL上下文（Context）。



**创建窗口**

渲染出内容，第一步：创建一个OpenGL上下文和一个用于显示的窗口。

这些操作在每个系统上都是不一样的，OpenGL有意将这些操作抽象(Abstract)出去。所以我们需要利用现成的一些库来完成这些最基本的平台相关代码，节省时间！

流行的有：GLUT、SDL、SFML、GLFW

对比一下：

**GLUT**: 适合学习和简单的 OpenGL 应用，功能有限，已停止更新。

**SDL**: 功能全面，适合游戏和多媒体应用开发，支持多种平台。

**SFML**: API 简洁，功能丰富，适用于 2D 图形、多媒体和游戏开发。

**GLFW**: 轻量级，专注于 OpenGL 窗口和输入管理，适合 OpenGL 应用。







**GLAD**

因为OpenGL只是一个标准，具体的实现是由驱动开发商实现。

由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。

所以任务就落在了开发者身上，开发者需要在运行时获取函数地址并将其保存在一个函数指针中供以后使用。

类似如下：

```
// 定义函数原型
typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
// 找到正确的函数并赋值给函数指针
GL_GENBUFFERS glGenBuffers  = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
// 现在函数可以被正常调用了
GLuint buffer;
glGenBuffers(1, &buffer);
```

幸运的是，GLAD就是简化此过程的库，也是最流畅最新的！





**你好窗口**





