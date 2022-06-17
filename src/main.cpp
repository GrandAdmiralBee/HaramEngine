#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "renderer/ShaderProgram.h"
#include "resources/ResourceManager.h"
#include "renderer/Texture2D.h"
#include "renderer/Sprite.h"




glm::ivec2 g_windowSize(640, 480);


GLfloat points[] = {
        0.0f,  50.f,
        50.f, -50.f,
       -50.f, -50.f,
};

GLfloat colors[]{
    0.55f, 0.0f, 1.0f,
    1.0f, 0.52f, 0.0f,
    0.0f, 0.78f, 1.0f
};

GLfloat texCoordinates[]{
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};


void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int heigth) 
{
    g_windowSize.x = width;
    g_windowSize.y = heigth;
    glViewport(0, 0, width, heigth);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{


    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "HaramEngine", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "GLFWwindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);


    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL())
	{
		std::cout << "Cannot load GLAD!" << std::endl;
		return -1;
	}

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0, 0.0, 0.0, 1);
    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Cannot create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Cannot create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("ButterflyTexture", "res/textures/butterfly.png");
        auto tex2 = resourceManager.loadTexture("GlebTexture", "res/textures/Gleb.jpg");

        auto pButterflySprite = resourceManager.loadSprite("ButterflySprite", "ButterflyTexture", "SpriteShader", 92, 68);
        pButterflySprite->setPosition(glm::vec2(0, 0));

        auto pGlebSprite = resourceManager.loadSprite("GlebSprite", "GlebTexture", "SpriteShader", 160, 106);
        pGlebSprite->setPosition(glm::vec2(0, 0));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoordinates_vbo = 0;
        glGenBuffers(1, &texCoordinates_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordinates_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordinates), texCoordinates, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordinates_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(270.f, 200.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(370.f, 200.f, 0.f));

        glm::mat4 modelMatrix_3 = glm::mat4(1.f);
        modelMatrix_3 = glm::translate(modelMatrix_3, glm::vec3(320.f, 300.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex2", 0);
        pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);


            
            pButterflySprite->render();
            pGlebSprite->render();


            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}