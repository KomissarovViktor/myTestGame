#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLfloat points[] = {
     0.0f, 50.f, 0.0f,
     50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoords[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};


glm::vec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pwindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}
void glfwKeyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pwindow, GL_TRUE);
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
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "MyGame", nullptr, nullptr);
    if (!pwindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(pwindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(pwindow);

	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	}
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
	glClearColor(1,1,0,1);
    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaderProgram("DefaultShaderProgram", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram->isCompiled())
        {
            std::cerr << "Can't create shader program: " << "DefaultShaderProgram" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaderProgram("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram->isCompiled())
        {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }
        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> subTexturesName = { "brickBlock","topLeftBrick","topRightBrick","topBrick","bottomLeftBrick","leftBrick",
                                                 "bottomLeft_topRightBrick","brickBlock_without_bottomRightBrick","bottomRightBrick",
                                                 "topLeft_bottomRightBrick","rightBrick","brickBlock_without_bottomLeftBrick",
                                                 "bottomBrick","brickBlock_without_topRightBrick","brickBlock_without_topLeftBrick"};

        auto pTextureAtlas = resourceManager.loadTextureAtlas("map_8x8", "res/textures/map_8x8.png", std::move(subTexturesName), 8, 8);
        auto pSpriteBrick = resourceManager.loadSprite("brickBlockSprite", "map_8x8", "SpriteShader", 100, 100, "bottomBrick");
        pSpriteBrick->setPosition(glm::vec2(300, 200));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoords_vbo = 0;
        glGenBuffers(1, &texCoords_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);
        
        glm::mat4 modelMatrix1(1.0f);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(100.f, 200.f, 0.f));

        glm::mat4 modelMatrix2(1.0f);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(500.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, g_windowSize.x, 0.f, g_windowSize.y, -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pwindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSpriteBrick->render();
            
            /* Swap front and back buffers */
            glfwSwapBuffers(pwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}