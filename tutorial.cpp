#include <iostream>
#include <cmath>
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;
const int KEYS = 320;

H3DNode model = 0, cam = 0;

double deg2rad(double deg) {
    return deg * M_PI / 180.0f;
}

// Simple key state holder
bool gKeys[KEYS];
void setKeyState( int key, bool state ) {
    gKeys[key] = state;
}

float gCamRotation;

// GLFW callbacks
void error_callback(int error, const char* desc) {
    fprintf(stderr, "Error: %s\n", desc);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// In charge of initializing Horde3D and resources
void initGame(int winWidth, int winHeight) {
    // Init Horde3D engine
    h3dInit(H3DRenderDevice::OpenGL4);
    // Horde 3D configurations
    h3dSetOption(H3DOptions::LoadTextures, 1);
    h3dSetOption(H3DOptions::TexCompression, 0);
    h3dSetOption(H3DOptions::MaxAnisotropy, 4);
    h3dSetOption(H3DOptions::ShadowMapSize, 2048);
    // Add resources
    H3DRes forwardPipeRes = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    H3DRes modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);
    H3DRes animRes = h3dAddResource(H3DResTypes::Animation, "animations/man.anim", 0);
    H3DRes skyBoxRes = h3dAddResource(H3DResTypes::SceneGraph, "models/skybox/skybox.scene.xml", 0);
    // Load resources
    if(!h3dutLoadResourcesFromDisk("../Content/")) {
        std::cout << "Failed to load resources" << std::endl;
        return;
    }
    // Add the model to scene with animation
    model = h3dAddNodes(H3DRootNode, modelRes);
    h3dSetupModelAnimStage(model, 0, animRes, 0, "", false);
    // Add skybox to scene
    H3DNode sky = h3dAddNodes(H3DRootNode, skyBoxRes);
    h3dSetNodeTransform(sky, 0, 0, 0, 0, 0, 0, 210, 50, 210);
    h3dSetNodeFlags(sky, H3DNodeFlags::NoCastShadow, true);
    // Add a light source, with position, radius and color
    H3DNode light = h3dAddLightNode(H3DRootNode, "Light1",  0, "LIGHTING","SHADOWMAP" );
    h3dSetNodeTransform(light, 0, 20, 0, 0, 0, 0, 1, 1, 1);
    h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 50.0f);
    h3dSetNodeParamF(light, H3DLight::ColorF3, 0.0f, 2.0f);
    // Add camera
    cam = h3dAddCameraNode(H3DRootNode, "Camera", forwardPipeRes);
    // Setup viewport for camera
    h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, winWidth);
    h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, winHeight);
    h3dSetupCameraView(cam, 45.0f, (float) winWidth / winHeight, 0.5f, 2048.0f);
    h3dResizePipelineBuffers(forwardPipeRes, winWidth, winHeight);
    // Reposition camera
    h3dSetNodeTransform(cam, 0, 0, 3.0f, 0, 0, 0, 1, 1, 1);
}

void gameLoop(double deltat) {
    static float t = 0;
    // Increase animation time
    t += 10.0 * deltat;
    // Play animation
    h3dSetModelAnimParams(model, 0, t, 1.0f);
    h3dUpdateModel(model, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
    // Set new model position
    h3dSetNodeTransform(model, t * 10, 0, 0,
                               0, 0, 0,
                               1, 1, 1);
    // Camera rotation and position
    if(gKeys[GLFW_KEY_LEFT]) gCamRotation -= 50.0 * deltat;
    if(gKeys[GLFW_KEY_RIGHT]) gCamRotation += 50.0 * deltat;
    float zCamPos = std::cos(deg2rad(gCamRotation)) * 3.0f;
    float xCamPos = std::sin(deg2rad(gCamRotation)) * 3.0f;
    // Set new camera rotation and position
    h3dSetNodeTransform(cam,
                        xCamPos, 0, zCamPos,
                        0, gCamRotation, 0,
                        1, 1, 1);
}

void releaseGame(GLFWwindow* window) {
    // Release engine
    h3dRelease();
    // GLFW cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
}



int main(){
    // Window handler
    GLFWwindow* window;

    // Initialize GLFW. Set callbacks, version, buffer swap delay and window context (after creation)
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Couldn't initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Title", NULL, NULL);
    // Window creation sanity check. Remember to terminate GLFW!
    if (!window) {
        std::cerr << "Couldn't create window" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize Horde3D render engine and resources
    initGame(WIN_WIDTH, WIN_HEIGHT);

    // Time variables
    double prevTime = glfwGetTime();
    double currTime = glfwGetTime();
    double dt = 0.0, lag = 0.0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate current delta time
        currTime = glfwGetTime();
        dt = currTime - prevTime;
        prevTime = currTime;
        lag += dt;

        // Update key states
        for( int i = 32; i < KEYS; ++i ) {
            setKeyState( i, glfwGetKey(window, i) == GLFW_PRESS );
        }
        // Run game loop logic
        gameLoop(dt);

        // Render scene
        h3dRender(cam);
        h3dFinalizeFrame();
        // After render logic, swap buffers and poll events
        glfwSwapBuffers(window);
        // Process input
        glfwPollEvents();
    }

    // When exiting, cleanup Horde3D and GLFW
    releaseGame(window);

    return 0;
}