#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "VoxelSet.hpp"
#include "VoxelWorld.hpp"
#include "LoadProgram.hpp"

int main() {

    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    // Open a window and create its OpenGL context
    GLFWwindow* window{glfwCreateWindow( 800, 600, "C'est le projet de votre année", NULL, NULL)};

    if(window == nullptr){

        std::cout << "Can't create a window, glfw error." << std::endl;
        const char* errorMessage;
        int code = glfwGetError(&errorMessage);
        std::cout << "GLFW error " << code << ": " << errorMessage << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cout << "Can't initialize GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    
    glClearColor(0.1f, 0.35f, 0.4f, 0.0f);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwPollEvents();

    glm::mat4 view{glm::lookAt(glm::vec3{0.f, 0.f, 4.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f})};
    glm::mat4 projection{glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f)};

    bool haveToStop{false};

    GLuint program{0};
    if(!createProgram(program, "Datas/Shaders/voxelVertex.vert", "Datas/Shaders/voxelFragment.frag")) { 

        std::cout << "Error with shaders load." << std::endl;
        return -1; 
    }

    VoxelWorld world{10, 10, 10};
    VoxelSet firstSet{world, program, 1, {200}};

    world.setColor(200, glm::vec4{0.f, 1.f, 1.f, 1.f});



    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID;
    createProgram(programID, "vertex_shader.glsl", "fragment_shader.glsl");

        static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };


    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindAttribLocation(programID, 0, "vertex");

    //Mettre les sommets dans un vertex buffer glBufferData()

    while (!haveToStop) {

        //Event
       
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { haveToStop = true; }


        //Update


        //Draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.f);

        firstSet.draw(view, projection);

        /*glUseProgram(programID);
        glBindVertexArray(VertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    } 

    glDeleteProgram(program);
    glfwTerminate();

    return 0;
}