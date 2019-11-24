#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "VoxelSet.hpp"
#include "VoxelWorld.hpp"
#include "LoadProgram.hpp"

#include "SceneObject.hpp"
#include "Camera.hpp"

#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"

bool initOpenGL(GLFWwindow **window) {

    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

        // Open a window and create its OpenGL context
    *window = glfwCreateWindow( 800, 600, "C'est le projet de votre année", NULL, NULL);

    if(*window == nullptr){

        std::cout << "ERROR: Can't create a window, glfw error." << std::endl;
        const char* errorMessage;
        int code = glfwGetError(&errorMessage);
        std::cout << "GLFW error " << code << ": " << errorMessage << std::endl;
        return false;
    }

    glfwMakeContextCurrent(*window);

    // Initialize GLEW
    glewExperimental = true; 
    if (glewInit() != GLEW_OK) {
        std::cout << "Can't initialize GLEW." << std::endl;
        glfwTerminate();
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    return true;
}

int main() {

    GLFWwindow* window;

    if(!initOpenGL(&window)) {

        glfwTerminate();
        return -1;
    }

    Gg::GulgEngine engine;

    if(!engine.loadSignatures("Datas/Signatures")) {

        std::cout << "Gulg error: Can't open signatures file." << std::endl;
        return -1;
    }


    bool haveToStop{false};

    if(!engine.loadProgram("Datas/Shaders/voxelVertex.vert", "Datas/Shaders/voxelFragment.frag", "MainProgram")) {

        std::cout << "Error with shaders load." << std::endl;
        return -1;
    }

    GLuint program{engine.getProgram("MainProgram")};
    Gg::Component::Mesh mesh{program};

    mesh.m_vertexPosition.resize(4);
    mesh.m_vertexPosition[0] = glm::vec3{0.f, 0.f, 0.f};
    mesh.m_vertexPosition[1] = glm::vec3{0.f, 1.f, 0.f};
    mesh.m_vertexPosition[2] = glm::vec3{1.f, 0.f, 0.f};
    mesh.m_vertexPosition[3] = glm::vec3{1.f, 1.f, 0.f};

    for(unsigned int i{0}; i < 4; i++) {

        mesh.m_vertexColor.emplace_back(glm::vec3{0.55f, 0.45f, 0.1f});
    }

    for(unsigned int i{0}; i < 4; i++) {

        mesh.m_vertexNormal.emplace_back(glm::vec3{1.f, 0.f, 0.f});
    }

    mesh.m_vertexIndice.emplace_back(0);
    mesh.m_vertexIndice.emplace_back(1);
    mesh.m_vertexIndice.emplace_back(2);

    mesh.m_vertexIndice.emplace_back(1);
    mesh.m_vertexIndice.emplace_back(3);
    mesh.m_vertexIndice.emplace_back(2);

    mesh.reshape();






    while (!haveToStop) {

        //Event

        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { haveToStop = true; }


        //Update

        //Draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.45f, 0.55f, 1.0f);

        mesh.draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
