//
//  main.cpp
//  sph3
//
//  Created by Arthur Sun on 3/17/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "World.hpp"

GLFWwindow *window;
const GLFWvidmode* mode;

double mouseX = 0.0f, mouseY = 0.0f;
double pmouseX = mouseX, pmouseY = mouseY;
float width, height;
float dt = 0.016f;
float windowScl = 0.75f;
float scl = 50.0f;
float h = 0.2f;

int framesPerSecond = 0;
float lastSecondTime = glfwGetTime();

World* world;

void addFromMouse() {
    float k = 2.0f;
    float w = 20.0f;
    float h = 20.0f;
    world->create_rect((mouseX * 2.0f - width) * k / scl, (mouseY * 2.0f - height) * -k / scl, w, h, 1.0f);
}

void addFromMouse2() {
    float k = 2.0f;
    float w = 2.0f;
    float h = 2.0f;
    world->create_rect((mouseX * 2.0f - width) * k / scl, (mouseY * 2.0f - height) * -k / scl, w, h, 1.0f, 0.0f, -60.0f);
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_RELEASE) {
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_C) {
            printf("%d\n", world->count);
        }
        if(key == GLFW_KEY_S) {
            printf("%f\n", scl);
        }
        if(key == GLFW_KEY_W) {
            world->exf *= -1.0f;
        }
        if(key == GLFW_KEY_A) {
            addFromMouse();
        }
    }
}

void initialize() {
    initBases();
    
    world = new World(640, h);
    
    world->exf.x = 0.0f;
    world->exf.y = 0.0f;
}

void free() {
    freeBases();
    
    delete world;
}

void draw() {
    world->solve(dt, 1);
    world->render(0, 0, 0, width * 2, height * 2, scl);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    scl *= exp(yoffset * 0.001f);
}

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    if(!glfwInit()) return 1;
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
    
    width = (float)mode->width * windowScl;
    height = (float)mode->height * windowScl;
    
    window = glfwCreateWindow(width, height, "Sim", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    glewInit();
    
    const unsigned char* t = glGetString(GL_VERSION);
    
    printf("%s\n", t);
    
    initialize();
    
    glfwSetMouseButtonCallback(window, mouseCallback);
    
    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    
    glfwSetCursor(window, cursor);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    
    world->create_rect(0.0f, 0.0f, 40.0f, 80.0f, 1.0f);
    
    do {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        float currentTime = glfwGetTime();
        bool press = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
        ++framesPerSecond;
        if(currentTime - lastSecondTime >= 1.0f) {
            printf("%f ms/frame \n", 1000.0f * (currentTime - lastSecondTime)/(float)framesPerSecond);
            framesPerSecond = 0;
            lastSecondTime = currentTime;
        }
        
        if(press == GLFW_PRESS) {
            addFromMouse2();
        }
        
        draw();
        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        pmouseX = mouseX;
        pmouseY = mouseY;
    } while (glfwWindowShouldClose(window) == GL_FALSE && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
    free();
    glfwDestroyCursor(cursor);
    glfwTerminate();
    return 0;
}
