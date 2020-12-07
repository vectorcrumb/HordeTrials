#include <iostream>
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

H3DNode model = 0, cam = 0;

void initGame(int winWidth, int winHeight) {
    // Init engine
    h3dInit(H3DRenderDevice::OpenGL4);
    // Add a pipeline resource
    H3DRes pipeRes = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    // Add model resource
    H3DRes modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);
    // Add animation resource
    H3DRes animRes = h3dAddResource(H3DResTypes::Animation, "animations/man.anim", 0);
    // Load added resources
    h3dutLoadResourcesFromDisk("");

    // Add the model to scene
    model = h3dAddNodes(H3DRootNode, modelRes);
    // Apply animation to model
    h3dSetupModelAnimStage(model, 0, animRes, 0, "", false);

    // Add a light source
    H3DNode light = h3dAddLightNode(H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP");
    // Set light position and radius
    h3dSetNodeTransform(light, 0, 20, 0, 0, 0, 0, 1, 1, 1);
    h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 50.0f);

    // Add camera
    H3DNode cam = h3dAddCameraNode(H3DRootNode, "Camera", pipeRes);

    // Setup viewpoert and render target sizez
    h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, winWidth);
    h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, winHeight);
    h3dSetupCameraView(cam, 45.0f, (float)winWidth / winHeight, 0.5f, 2048.0f);
    h3dResizePipelineBuffers(pipeRes, winWidth, winHeight);
}

void gameLoop(float fps) {
    static float t = 0;
    // Increase animation time
    t += 10.0f * (1/fps);

    // Play animation
    h3dSetModelAnimParams(model, 0, t, 1.0f);
    h3dUpdateModel(model, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);

    // Set new model position
    h3dSetNodeTransform(model, t * 10, 0, 0,
                               0, 0, 0,
                               1, 1, 1);
    
    // Render scene
    h3dRender(cam);

    // Finish frame rendering
    h3dFinalizeFrame();
}

void releaseGame() {
    // Release engine
    h3dRelease();
}

void error_callback(int error, const char* desc) {
    fprintf(stderr, "Error: %s\n", desc);
}

int main(){

    int initStatus = glfwInit();

    if (!initStatus) {
        std::cerr << "Couldn't initialize glfw" << std::endl;
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Title", NULL, NULL);
    if (!window) {
        std::cerr << "Couldn't create window" << std::endl;
        return -2;
    }
    glfwMakeContextCurrent(window);

    initGame(WIN_WIDTH, WIN_HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        gameLoop(30);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}