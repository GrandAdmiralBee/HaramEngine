#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <ctime>

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
    srand(time(NULL));

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

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram) {
            std::cerr << "Cannot create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> subTexturesNames16 = { "eagle", "sign", "explosion1", "explosion2" , "explosion3", "explosion4", "impact1", "impact2" };
        std::vector<std::string> GlebTextureNames = { "gleb1", "gleb2" , "gleb3" , "gleb4" , "gleb5"};


        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames16), 16, 16);
        auto pTextureAtlasGleb = resourceManager.loadTextureAtlas("GlebTextureAtlas", "res/textures/Gleb100x65.png", std::move(GlebTextureNames), 100, 65);



        auto pSprite1 = resourceManager.loadSprite("NewSprite1", "GlebTextureAtlas", "SpriteShader", 100, 65, "gleb1");
        auto pSprite2 = resourceManager.loadSprite("NewSprite2", "GlebTextureAtlas", "SpriteShader", 100, 65, "gleb2");
        auto pSprite3 = resourceManager.loadSprite("NewSprite3", "GlebTextureAtlas", "SpriteShader", 100, 65, "gleb3");
        auto pSprite4 = resourceManager.loadSprite("NewSprite4", "GlebTextureAtlas", "SpriteShader", 100, 65, "gleb4");
        auto pSprite5 = resourceManager.loadSprite("NewSprite5", "GlebTextureAtlas", "SpriteShader", 100, 65, "gleb5");


        pSprite1->setPosition(glm::vec2(rand() % 540, rand() % 380));
        pSprite2->setPosition(glm::vec2(rand() % 540, rand() % 380));

        pSprite3->setPosition(glm::vec2(rand() % 540, rand() % 380));

        pSprite4->setPosition(glm::vec2(rand() % 540, rand() % 380));

        pSprite5->setPosition(glm::vec2(rand() % 540, rand() % 380));




       

    

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);


        pSpriteShaderProgram->use();
        //pSpriteShaderProgram->setInt("tex2", 0);
        pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

             pSprite1->render();

             pSprite2->render();
            //pSprite3->render();
            //pSprite4->render();
            //pSprite5->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}