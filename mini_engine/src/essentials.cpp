#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Window size
const int WIN_W = 1280;
const int WIN_H = 720;

// Canvas size
const int CANVAS_W = 480;
const int CANVAS_H = 360;

// Vertex shader (simple passthrough)
const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

// Fragment shader with rounded corners
const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform float radius;
uniform vec2 resolution;

void main() {
    vec2 uv = gl_FragCoord.xy;
    vec2 br = resolution;

    vec2 d = min(uv, br - uv);
    float dist = length(d);
    if(dist < radius) {
        FragColor = vec4(0.2, 0.6, 0.9, 1.0);
    } else if(d.x >= radius || d.y >= radius) {
        FragColor = vec4(0.2, 0.6, 0.9, 1.0);
    } else {
        discard;
    }
}
)";

int main() {
    if(!glfwInit()){ std::cerr << "GLFW failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Top-Right GPU Canvas", nullptr,nullptr);
    if(!window){ glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glewInit();

    // Create shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Quad covering -1..1 NDC
    float quad[] = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,
        -1.f,  1.f
    };
    unsigned int indices[] = {0,1,2, 2,3,0};
    GLuint VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quad),quad,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Main loop
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f,0.1f,0.12f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set viewport to top-right corner
        glViewport(WIN_W - CANVAS_W - 20, WIN_H - CANVAS_H - 20, CANVAS_W, CANVAS_H);

        // Draw the rounded rectangle
        glUseProgram(shaderProgram);
        int resLoc = glGetUniformLocation(shaderProgram,"resolution");
        glUniform2f(resLoc,(float)CANVAS_W,(float)CANVAS_H);
        int radiusLoc = glGetUniformLocation(shaderProgram,"radius");
        glUniform1f(radiusLoc,20.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // Reset viewport
        glViewport(0,0,WIN_W,WIN_H);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
