#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"
#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Light.hpp"

#include "Systems/UpdateScene.hpp"
#include "Systems/DrawScene.hpp"
#include "Systems/Lightning.hpp"

#include "NewMap.hpp"

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


    Gg::Entity gameID{engine.getNewEntity()},
               worldID{engine.getNewEntity()},
               cameraID{engine.getNewEntity()},
               cameraTranslateID{engine.getNewEntity()};

    newMap(engine,worldID,program);

    std::shared_ptr<Gg::Component::SceneObject> gameScene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::SceneObject> cameraScene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::SceneObject> cameraTranslateScene{std::make_shared<Gg::Component::SceneObject>()};

    std::shared_ptr<Gg::Component::Transformation> gameTransformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Transformation> cameraTransformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Transformation> cameraTranslateTransformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Transformation> worldTransformation{std::make_shared<Gg::Component::Transformation>()};


    engine.addComponentToEntity(gameID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(gameScene));
    engine.addComponentToEntity(cameraID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraScene));
    engine.addComponentToEntity(cameraTranslateID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraTranslateScene));

    engine.addComponentToEntity(gameID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(gameTransformation));
    engine.addComponentToEntity(cameraID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraTransformation));
    engine.addComponentToEntity(cameraTranslateID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraTranslateTransformation));


    Gg::Entity light1ID{engine.getNewEntity()};

    std::shared_ptr<Gg::Component::SceneObject> light1Scene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::Transformation> light1Transformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Light> light1Light{std::make_shared<Gg::Component::Light>()};

    engine.addComponentToEntity(light1ID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Scene));
    engine.addComponentToEntity(light1ID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Transformation));
    engine.addComponentToEntity(light1ID, "Light", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Light));

    light1Light->m_ambient = glm::vec3{1.f, 1.f, 1.f};
    light1Light->m_diffuse = glm::vec3{1.f, 1.f, 1.f};
    light1Light->m_specular = glm::vec3{1.f, 1.f, 1.f};

    light1Light->m_constant = 1.f;
    light1Light->m_linear = 0.007f;
    light1Light->m_quadratic = 0.0002f;

    light1Light->m_lightType = Gg::Component::LightType::Point;

    light1Transformation->translate(glm::vec3{40.f, 40.f, 40.f});


    gameScene->addChild(worldID);
    cameraTranslateScene->addChild(cameraID);
    gameScene->addChild(cameraTranslateID);
    gameScene->addChild(light1ID);


    UpdateScene sceneUpdate{engine};
    sceneUpdate.addEntity(gameID);

    DrawScene sceneDraw{engine};
    sceneDraw.addEntity(worldID);
    sceneDraw.setCameraEntity(cameraID);

    Lightning lightning{engine, program};
    lightning.addEntity(light1ID);

    glm::mat4 projection{glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 2000.f)};
    cameraTransformation->setSpecificTransformation(glm::lookAt(glm::vec3{0.f, 0.f, 5.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f}));


    while (!haveToStop) {

        //Event

        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { haveToStop = true; }

        if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(-1.f), glm::vec3{0.f, 1.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(1.f), glm::vec3{0.f, 1.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(-1.f), glm::vec3{1.f, 0.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(1.f), glm::vec3{1.f, 0.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(-1.f), glm::vec3{0.f, 0.f, 1.f}); }
        if(glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS) { cameraTransformation->rotate(glm::radians(1.f), glm::vec3{0.f, 0.f, 1.f}); }

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{0.f, -0.3f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{0.f, 0.3f, 0.f}); }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{0.f, 0.f, 0.3f}); }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{0.f, 0.f, -0.3f}); }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{0.3f, 0.f, 0.f}); }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cameraTranslateTransformation->translate(glm::vec3{-0.3f, 0.f, 0.f}); }


        //Update

        sceneUpdate.applyAlgorithms();
        sceneDraw.setProjection(projection);

        lightning.applyAlgorithms();

        //Draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.45f, 0.55f, 1.0f);

        sceneDraw.applyAlgorithms();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
