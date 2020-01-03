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
#include "Components/Collider.hpp"
#include "Components/Explosive.hpp"
#include "Components/Timer.hpp"

#include "Systems/UpdateScene.hpp"
#include "Systems/Collisions.hpp"
#include "Systems/Physics.hpp"
#include "Systems/DrawScene.hpp"
#include "Systems/Lightning.hpp"
#include "Systems/Time.hpp"

#include "LoadAnimation.hpp"
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

    if(!engine.loadProgram("Datas/Shaders/animationVertex.vert", "Datas/Shaders/animationFragment.frag", "AnimationProgram")) {

        std::cout << "Error with shaders load." << std::endl;
        return -1;
    }

    if(!engine.loadTexture("Datas/Animated/rambo.png", "RamboTexture")) {

        std::cout << "Error with shaders load." << std::endl;
        return -1;
    }

    GLuint program{engine.getProgram("MainProgram")};


    Gg::Entity gameID{engine.getNewEntity()},
               worldID{engine.getNewEntity()},
               cameraID{engine.getNewEntity()},
               playerID{engine.getNewEntity()};

    newMap(engine,worldID,program);

    std::shared_ptr<Gg::Component::SceneObject> gameScene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::SceneObject> cameraScene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::SceneObject> playerScene{std::make_shared<Gg::Component::SceneObject>()};

    std::shared_ptr<Gg::Component::Transformation> gameTransformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Transformation> cameraTransformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Transformation> playerTransformation{std::make_shared<Gg::Component::Transformation>()};

    std::shared_ptr<Gg::Component::Collider> playerCollider{std::make_shared<Gg::Component::Collider>
      (glm::vec3{0.f,0.f,0.f},glm::vec3{0.f,0.f,0.f},1.f)};
    std::shared_ptr<Gg::Component::Forces> playerForces{std::make_shared<Gg::Component::Forces>()};


    engine.addComponentToEntity(gameID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(gameScene));
    engine.addComponentToEntity(cameraID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraScene));
    engine.addComponentToEntity(playerID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerScene));

    engine.addComponentToEntity(gameID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(gameTransformation));
    engine.addComponentToEntity(cameraID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(cameraTransformation));
    engine.addComponentToEntity(playerID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerTransformation));

    engine.addComponentToEntity(playerID, "Collider", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerCollider));
    engine.addComponentToEntity(playerID, "Forces", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerForces));

    // loadAnimation(engine, playerID, "Datas/Animated/rambo.dae");
    // playerTransformation->translate(glm::vec3{0.f, 0.f, -20.f});
    // playerTransformation->scale(2);

    // std::shared_ptr<Gg::Component::Mesh> playerMesh{std::make_shared<Gg::Component::Mesh>(program)};
    //Cube(playerMesh);
     //    engine.addComponentToEntity(playerID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerMesh));
    //loadAnimation(engine, playerID, "Datas/Animated/rb.dae");

    // loadAnimation(engine, playerID, "Datas/Animated/rb.dae");
    // playerTransformation->translate(glm::vec3{0.f, 0.f, -20.f});
    // playerTransformation->scale(2);

     std::shared_ptr<Gg::Component::Mesh> playerMesh{std::make_shared<Gg::Component::Mesh>(program)};
    Cube(playerMesh,1.f,glm::vec3{0.f,0.3f,1.0f});
         engine.addComponentToEntity(playerID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(playerMesh));
    // loadAnimation(engine, playerID, "Datas/Animated/rambo.dae");

    //Directional

    Gg::Entity light1ID{engine.getNewEntity()};

    std::shared_ptr<Gg::Component::SceneObject> light1Scene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::Transformation> light1Transformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Light> light1Light{std::make_shared<Gg::Component::Light>()};

    engine.addComponentToEntity(light1ID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Scene));
    engine.addComponentToEntity(light1ID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Transformation));
    engine.addComponentToEntity(light1ID, "Light", std::static_pointer_cast<Gg::Component::AbstractComponent>(light1Light));

    light1Light->m_ambient = glm::vec3{0.1f, 0.1f, 0.1f};
    light1Light->m_diffuse = glm::vec3{1.f, 1.f, 1.f};
    light1Light->m_specular = glm::vec3{1.f, 1.f, 1.f};

    light1Light->m_direction = glm::vec3{0.f, 0.f, -1.f};
    light1Light->m_lightType = Gg::Component::LightType::Directional;

    //Point

    Gg::Entity light2ID{engine.getNewEntity()};

    std::shared_ptr<Gg::Component::SceneObject> light2Scene{std::make_shared<Gg::Component::SceneObject>()};
    std::shared_ptr<Gg::Component::Transformation> light2Transformation{std::make_shared<Gg::Component::Transformation>()};
    std::shared_ptr<Gg::Component::Light> light2Light{std::make_shared<Gg::Component::Light>()};

    engine.addComponentToEntity(light2ID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(light2Scene));
    engine.addComponentToEntity(light2ID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(light2Transformation));
    engine.addComponentToEntity(light2ID, "Light", std::static_pointer_cast<Gg::Component::AbstractComponent>(light2Light));

    light2Light->m_ambient = glm::vec3{0.0f, 0.0f, 0.0f};
    light2Light->m_diffuse = glm::vec3{0.85f, 0.15f, 0.17f};
    light2Light->m_specular = glm::vec3{1.f, 1.f, 1.f};

    light2Light->m_constant = 1.f;
    light2Light->m_linear = 0.027f;
    light2Light->m_quadratic = 0.0028f;

    light2Transformation->translate(glm::vec3{100.f, 300.f, -100.f});

    light2Light->m_lightType = Gg::Component::LightType::Point;




    gameScene->addChild(worldID);
    gameScene->addChild(playerID);
    playerScene->addChild(cameraID);


    gameScene->addChild(light1ID);
    gameScene->addChild(light2ID);


    UpdateScene sceneUpdate{engine};
    sceneUpdate.addEntity(gameID);

    DrawScene sceneDraw{engine};
    sceneDraw.addEntity(worldID);
    sceneDraw.addEntity(playerID);
    sceneDraw.setCameraEntity(cameraID);

    Physics physics{engine};
    physics.addEntity(playerID);

    Collisions collisions{engine,worldID};
    collisions.addEntity(playerID);

    Time time{engine,worldID};

    Lightning lightning{engine, program};
    lightning.addEntity(light1ID);
    //lightning.addEntity(light2ID);

    glm::mat4 projection{glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 2000.f)};
    //cameraTransformation->setSpecificTransformation(glm::lookAt(glm::vec3{0.f, 0.f, 10.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f}));
    cameraTransformation->translate(glm::vec3{0.f, 0.f, -50.f});
     playerTransformation->translate(glm::vec3{0.f, 0.f, -20.f} * cameraTransformation->m_rotation);

    sceneDraw.setProjection(projection);

    float P_acc = 0.1f;
    int gOldState = GLFW_RELEASE;
    int gNewState = GLFW_RELEASE;
    int rOldState = GLFW_RELEASE;
    int rNewState = GLFW_RELEASE;
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

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { playerForces->addForce(glm::vec3{0.f,  0.f,-(2.f*P_acc)} ); }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { playerForces->addForce(glm::vec3{0.f, 0.f, P_acc} ); }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {  glm::vec3 toadd{glm::vec3{0.f, 0.f, P_acc} * cameraTransformation->m_rotation};        toadd[2]=0.f;    playerForces->addForce(toadd);   }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {glm::vec3 toadd{glm::vec3{0.f, 0.f, -P_acc} * cameraTransformation->m_rotation};        toadd[2]=0.f;    playerForces->addForce(toadd); }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { glm::vec3 toadd{glm::vec3{P_acc,0.f, 0.f} * cameraTransformation->m_rotation};        toadd[2]=0.f;    playerForces->addForce(toadd);  }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {glm::vec3 toadd{glm::vec3{-P_acc,0.f, 0.f } * cameraTransformation->m_rotation};        toadd[2]=0.f;    playerForces->addForce(toadd);  }
        gOldState = gNewState;
        gNewState = glfwGetKey(window, GLFW_KEY_G) ;
        if(gOldState == GLFW_PRESS && gNewState == GLFW_RELEASE ) {
          Gg::Entity newG{engine.getNewEntity()};
          std::shared_ptr<Gg::Component::SceneObject> newGScene{std::make_shared<Gg::Component::SceneObject>()};
          std::shared_ptr<Gg::Component::Transformation> newGTransformation{std::make_shared<Gg::Component::Transformation>()};
          std::shared_ptr<Gg::Component::Collider> newGCollider{std::make_shared<Gg::Component::Collider>
            (glm::vec3{0.f,0.f,0.f},glm::vec3{0.f,0.f,0.f},0.5f)};
          std::shared_ptr<Gg::Component::Forces> newGForces{std::make_shared<Gg::Component::Forces>()};
          std::shared_ptr<Gg::Component::Mesh> newGMesh{std::make_shared<Gg::Component::Mesh>(program)};
          Cube(newGMesh,0.5f,glm::vec3{1.f,0.f,0.f});
          std::shared_ptr<Gg::Component::Explosive> newGExp{std::make_shared<Gg::Component::Explosive>(5,TIMER)};
          std::shared_ptr<Gg::Component::Timer> newGTimer{std::make_shared<Gg::Component::Timer>(5000)};

          newGTransformation->setSpecificTransformation(playerScene->m_globalTransformations);
          glm::vec3 f {(glm::vec3{0.f, 0.f, 10.f} * cameraTransformation->m_rotation)};
          f[2] += -5.f;
          newGForces->addForce(playerForces->velocity + f);
          engine.addComponentToEntity(newG, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGScene));
          engine.addComponentToEntity(newG, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGTransformation));
          engine.addComponentToEntity(newG, "Collider", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGCollider));
          engine.addComponentToEntity(newG, "Forces", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGForces));
          engine.addComponentToEntity(newG, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGMesh));
          engine.addComponentToEntity(newG, "Explosive", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGExp));
          engine.addComponentToEntity(newG, "Timer", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGTimer));
          gameScene->addChild(newG);
          sceneDraw.addEntity(newG);
          physics.addEntity(newG);
          collisions.addEntity(newG);
          time.addEntity(newG);
          sceneUpdate.applyAlgorithms();
        }
        rOldState = rNewState;
        rNewState = glfwGetKey(window, GLFW_KEY_R) ;
        if(rOldState == GLFW_PRESS && rNewState == GLFW_RELEASE ) {
          Gg::Entity newG{engine.getNewEntity()};
          std::shared_ptr<Gg::Component::SceneObject> newGScene{std::make_shared<Gg::Component::SceneObject>()};
          std::shared_ptr<Gg::Component::Transformation> newGTransformation{std::make_shared<Gg::Component::Transformation>()};
          std::shared_ptr<Gg::Component::Collider> newGCollider{std::make_shared<Gg::Component::Collider>
            (glm::vec3{0.f,0.f,0.f},glm::vec3{0.f,0.f,0.f},0.5f)};
          std::shared_ptr<Gg::Component::Forces> newGForces{std::make_shared<Gg::Component::Forces>(glm::vec3{0.f},0.f,1.f)};
          std::shared_ptr<Gg::Component::Mesh> newGMesh{std::make_shared<Gg::Component::Mesh>(program)};
          Cube(newGMesh,0.5f,glm::vec3{1.f,0.f,0.f});
          std::shared_ptr<Gg::Component::Explosive> newGExp{std::make_shared<Gg::Component::Explosive>(5,ON_COLLISION)};

          newGTransformation->setSpecificTransformation(playerScene->m_globalTransformations);
          glm::vec3 f {(glm::vec3{0.f, 0.f, 10.f} * cameraTransformation->m_rotation)};
          f[2] += -5.f;
          newGForces->addForce(playerForces->velocity + f);
          engine.addComponentToEntity(newG, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGScene));
          engine.addComponentToEntity(newG, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGTransformation));
          engine.addComponentToEntity(newG, "Collider", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGCollider));
          engine.addComponentToEntity(newG, "Forces", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGForces));
          engine.addComponentToEntity(newG, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGMesh));
          engine.addComponentToEntity(newG, "Explosive", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGExp));
          gameScene->addChild(newG);
          sceneDraw.addEntity(newG);
          physics.addEntity(newG);
          collisions.addEntity(newG);
          sceneUpdate.applyAlgorithms();
        }
        //Update
        collisions.applyAlgorithms();
        time.applyAlgorithms();
        physics.applyAlgorithms();
        sceneUpdate.applyAlgorithms();
        lightning.applyAlgorithms();


        //Draw

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        sceneDraw.applyAlgorithms();
        for(Gg::Entity toD : time.toDelete){
          collisions.deleteEntity(toD);
          physics.deleteEntity(toD);
          sceneUpdate.deleteEntity(toD);
          sceneDraw.deleteEntity(toD);
          engine.deleteEntity(toD);
          time.deleteEntity(toD);
          gameScene->deleteChild(toD);
        }
        for(Gg::Entity toD : time.toAdd){
          collisions.addEntity(toD);
          physics.addEntity(toD);
          sceneUpdate.addEntity(toD);
          sceneDraw.addEntity(toD);
          gameScene->addChild(toD);
        }

        for(Gg::Entity toD : collisions.toDelete){
          collisions.deleteEntity(toD);
          physics.deleteEntity(toD);
          sceneUpdate.deleteEntity(toD);
          sceneDraw.deleteEntity(toD);
          engine.deleteEntity(toD);
          time.deleteEntity(toD);
          gameScene->deleteChild(toD);
        }
        for(Gg::Entity toD : collisions.toAdd){
          collisions.addEntity(toD);
          physics.addEntity(toD);
          sceneUpdate.addEntity(toD);
          sceneDraw.addEntity(toD);
          gameScene->addChild(toD);
        }
        if(collisions.toAdd.size()>0 || time.toAdd.size()>0)sceneUpdate.applyAlgorithms();
        time.toAdd.clear();
        collisions.toAdd.clear();
        time.toDelete.clear();
        collisions.toDelete.clear();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
