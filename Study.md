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



**你好三角形**

先记住三个单词：

- VAO（顶点数组对象）
- VBO（顶点缓冲对象）
- IBO（索引缓冲对象） or EBO



图形渲染管线：

3D坐标转为2D坐标的处理过程是由OpenGL的图形渲染管线处理，它们可以被划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化，很容易并行执行！

GPU上为每一个（渲染管线）阶段运行各自的小程序，从而在快速处理你的数据。这些小程序叫做Shader！

OpenGL着色器是用OpenGL着色器语言（GLSL）编写。

渲染管线抽象如下图所示：

![image-20240624225948825](./Study.assets/image-20240624225948825.png)



蓝色表示可自定义阶段，灰色表示固定阶段！



现代OpenGL中，我们**必须**定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。

OpenGL仅当3D坐标在3个轴（x、y和z）上-1.0到1.0的范围内时才处理。

这个范围内的坐标叫NDC坐标！此范围内的坐标最终显示在屏幕上！



**VBO**

顶点缓冲对象是我们在[OpenGL](https://learnopengl-cn.github.io/01 Getting started/01 OpenGL/)教程中第一个出现的OpenGL对象，它会在GPU内存（通常被称为显存）中储存大量顶点。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次！



OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型！



**VAO**

绑定针对VBO的顶点数据的属性描述集合，如下图：

![image-20240624232514532](./Study.assets/image-20240624232514532.png)



最常见的渲染之前设置状态语句：

- glUseProgram(shaderProgram); 
- glBindVertexArray(VAO); 
- glDrawArrays(GL_TRIANGLES, 0, 3);



**EBO**

可以理解为一种特殊的VBO，它只是存储构成绘制图元的顶点索引序列而已！它和VAO、VBO关系如下：

![image-20240624233843496](./Study.assets/image-20240624233843496.png)



**着色器**

着色器(Shader)是运行在GPU上的小程序

典型的GLSL代码结构：

```glsl
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```



能声明的顶点属性是有上限的，由硬件决定。OpenGL确保至少有16个包含4分量的顶点属性可用！

通过一下Code片段可查询：

```glsl
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
```



**GLSL语言介绍：**

基础数据类型：

- int
- float
- double
- uint
- bool

向量类型：

- vecn（float）
- bvecn（bool）
- ivecn（int）
- uvecn（unsigned int）
- dvecn（double）



向量的分量访问：

- .x | .y | .z | .w
- .r | .g | .b | .a
- .s | .t | .p | .q

灵活的方式：重组（swizzling），如下：

- xyzz
- xxx
- xy





`layout (location = 0)`  表示：

顶点着色器为它的输入提供一个额外的`layout`标识，这样我们才能把它链接到顶点数据。





如果我们打算从一个着色器向另一个着色器发送数据：

- 发送方着色器中声明一个输出
- 接收方着色器中声明一个类似的输入
- 类型和名字都一样





**Uniform**

Uniform是另一种从我们的应用程序在 CPU 上传递数据到 GPU 上的着色器的方式。

它是全局的，它在每个着色器程序对象中独一无二，而且可以被着色器程序的任意着色器在任意阶段访问。















