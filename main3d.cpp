#include <iostream>
#include <filesystem>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.cpp"
#include "rubicscube.cpp"

// Global variables that hold the state of the game
RubicsCube game;
RotationManager rotation_manager(&game);

// Camera state
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// Key states for the game
bool keyMajPressed = false;
bool keyFPressed = false;
bool keyRPressed = false;
bool keyUPressed = false;

unsigned int yellow, red, white, blue, orange, green, none;

unsigned int color_to_code(Color c) {
    switch (c)
    {
    case Color::YELLOW: return yellow;
    case Color::GREEN: return green;
    case Color::RED: return red;
    case Color::BLUE: return blue;
    case Color::ORANGE: return orange;
    case Color::WHITE: return white;
    case Color::NONE: return none;
    }
    return 0;
}

/**
 * Process all inputs for the rubicscube solver
 */
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.15f; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= 0.1f * cameraSpeed * cameraPos;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += 0.1f * cameraSpeed * cameraPos;
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraPos, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraPos, cameraUp)) * cameraSpeed;

    // ups and downs
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;


    // Detect if maj is pressed (to do backward motion)

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (!keyMajPressed) keyMajPressed = true;
    } else if (keyMajPressed) {
        keyMajPressed = false;
    }

    // Game actions: F, R, U

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!keyFPressed) {
            keyFPressed = true;
            if (rotation_manager.is_free()) {
                rotation_manager.start_motion(Motion::F, !keyMajPressed);
            }
        }
    } else if (keyFPressed) {
        keyFPressed = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!keyRPressed) {
            keyRPressed = true;
            if (rotation_manager.is_free()) {
                rotation_manager.start_motion(Motion::R, !keyMajPressed);
            }
        }
    } else if (keyRPressed) {
        keyRPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        if (!keyUPressed) {
            keyUPressed = true;
            if (rotation_manager.is_free()) {
                rotation_manager.start_motion(Motion::U, !keyMajPressed);
            }
        }
    } else if (keyUPressed) {
        keyUPressed = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/**
 * A function that loads a texture from a given path
 */
void load_gl_texture(unsigned int& id, const char* path, bool rgba = false) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (rgba)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using std::cout;
using std::endl;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    // Shader ourShader("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");
    Shader ourShader("/home/arthur/dev/cpp/tuto1/shader_vert.glsl", "/home/arthur/dev/cpp/tuto1/shader_frag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    /*
     * Faces are each assigned to a number as defined below

            Back   = 0
            Front  = 1
            Left   = 2
            Right  = 3
            Bottom = 4
            Top    = 5 

     */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 2.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 2.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 2.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 2.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 2.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 2.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 3.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 3.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 3.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 3.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 3.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 3.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 4.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 4.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 4.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 4.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 4.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 4.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 5.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 5.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 5.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 5.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 5.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 5.0f
        };

    // Setup VBO, VAO and EBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    load_gl_texture(yellow, "/home/arthur/dev/cpp/tuto1/resources/yellow.png");
    load_gl_texture(red, "/home/arthur/dev/cpp/tuto1/resources/red.png");
    load_gl_texture(white, "/home/arthur/dev/cpp/tuto1/resources/white.png");
    load_gl_texture(blue, "/home/arthur/dev/cpp/tuto1/resources/blue.png");
    load_gl_texture(orange, "/home/arthur/dev/cpp/tuto1/resources/orange.png");
    load_gl_texture(green, "/home/arthur/dev/cpp/tuto1/resources/green.png");
    load_gl_texture(none, "/home/arthur/dev/cpp/tuto1/resources/selected.png", true);

    ourShader.use();

    ourShader.setInt("texture0", 0);
    ourShader.setInt("texture1", 1);
    ourShader.setInt("texture2", 2);
    ourShader.setInt("texture3", 3);
    ourShader.setInt("texture4", 4);
    ourShader.setInt("texture5", 5);
    ourShader.setInt("textureNone", 6);

    // Activate depth buffer
    glEnable(GL_DEPTH_TEST);

    game.set_main_color(Color::BLUE);

    // Wireframe mode ?
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    Color colors[3] = {Color::NONE, Color::NONE, Color::NONE};
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Makes every rotation 
        rotation_manager.step();

        // activate shader
        ourShader.use();

        // create transformations 
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Setup the camera
        view = glm::lookAt(cameraPos, vec3(0., 0., 0.), cameraUp);
        projection = glm::perspective(glm::radians(70.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, none);

        glBindVertexArray(VAO);
        for (const auto& cube: game.cubes) {
            // Get the colors of the cube
            cube.fillColors(colors);

            // FRONT 
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, color_to_code(colors[0]));
            // RIGHT
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, color_to_code(colors[1]));
            // TOP
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, color_to_code(colors[2]));
            // also set the BOTTOM color to allow for swapping axis
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, color_to_code(colors[2]));

            // Is this cube on the main face ? 
            ourShader.setBool("onCurrentFace", game.is_cube_on_selected_face(cube));

            // Set the model matrix to the transform of the cube and then render
            ourShader.setMat4("model", cube.transform);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}