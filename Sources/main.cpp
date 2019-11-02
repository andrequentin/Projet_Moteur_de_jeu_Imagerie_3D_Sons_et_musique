#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glm::mat4 view{glm::lookAt(glm::vec3{0.f, 0.f, 10.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f})};
    glm::mat4 projection{glm::perspective(glm::radians(45.0f), 800.f / 600.f, 1.f, 30.f)};

    bool haveToStop{false};

    GLuint program{0};
    if(!createProgram(program, "Datas/Shaders/voxelVertex.vert", "Datas/Shaders/voxelFragment.frag")) {

        std::cout << "Error with shaders load." << std::endl;
        return -1;
    }

    // VoxelWorld world{10, 10, 10};
    // world.setColor(0, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(1, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(2, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(10, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(11, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(20, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(100, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(101, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(110, glm::vec4{0.5f, 1.f, 0.5f, 1.f});
    // world.setColor(200, glm::vec4{0.5f, 1.f, 0.5f, 1.f});

    VoxelWorld world{10,10,10};

    world.perlinGen((unsigned int)2);

    std::vector<unsigned int> transparencyTest;

    for(unsigned int i{0}; i < 1'000; i++) {

        transparencyTest.emplace_back(i);
    }

    VoxelSet firstSet{world, program, 1, transparencyTest};

    const float rotateSpeed{1.f};

    while (!haveToStop) {

        //Event

        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { haveToStop = true; }

        if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) { view = glm::rotate(view, glm::radians(rotateSpeed), glm::vec3{0.f, 1.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS) { view = glm::rotate(view, glm::radians(-rotateSpeed), glm::vec3{0.f, 1.f, 0.f}); }

        if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS) { view = glm::rotate(view, glm::radians(rotateSpeed), glm::vec3{1.f, 0.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) { view = glm::rotate(view, glm::radians(-rotateSpeed), glm::vec3{1.f, 0.f, 0.f}); }


        //Update


        //Draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.45f, 0.55f, 1.0f);

        firstSet.draw(view, projection);

        glfwSwapBuffers(window);

    }

    glDeleteProgram(program);
    glfwTerminate();

    return 0;
}
