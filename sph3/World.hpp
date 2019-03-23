//
//  World.hpp
//  sph3
//
//  Created by Arthur Sun on 3/17/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include "setup.hpp"

class World
{
    
protected:
    
    float h;
    
    float p0 = 1.0f;
    float u = 500.0f;
    float e = 0.25f;
    
    int root;
    int list = 4096;
    glm::vec2 size;
    
    DoubleTexture positions;
    DoubleTexture velocities;
    DoubleTexture hashGrid;
    Texture offsetList;
    Texture pressures;
    
    Shader sorter;
    Shader putRect;
    Shader stepper;
    Shader solver;
    Shader drawer;
    Shader weight;
    Shader hasher;
    Shader toList;
    Shader setter;
    Shader setterf;
    
    void sort();
    
    void weight_particles();
    
    void solve_pressure(float dt);
    
    void sort_step(int j, int k, int n);
    
    void step_once(float dt);
    
    void map_to_grid();
    
    void map_to_list();
    
    void solve_once(float dt);
    
    GLuint vao;
    GLuint vbo;
    
    float _dt;
    
    void blit(Texture& tex, int start, int count) {
        glBindFramebuffer(GL_FRAMEBUFFER, tex.target->fbo);
        glBindVertexArray(vao);
        glViewport(0, 0, root, root);
        glDrawArrays(GL_POINTS, start, count);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
public:
    
    int count;
    int capacity;
    glm::vec2 exf;
    
    World(int r, float h) : root(r), h(h), count(0), size(r), exf(0.0f), capacity(r * r)
    {
        positions.init(GL_NEAREST);
        positions.image(GL_RG32F, GL_RG, root, root, GL_FLOAT, 0);
        
        velocities.init(GL_NEAREST);
        velocities.image(GL_RG32F, GL_RG, root, root, GL_FLOAT, 0);
        
        hashGrid.init(GL_NEAREST);
        hashGrid.image(GL_RG32I, GL_RG_INTEGER, root, root, GL_INT, 0);
        
        pressures.init(GL_NEAREST);
        pressures.image(GL_R32F, GL_RED, root, root, GL_FLOAT, 0);
        
        offsetList.init(GL_NEAREST);
        offsetList.image(GL_R32I, GL_RED_INTEGER, list, list, GL_INT, 0);
        
        sorter.init("glsl/pass.vs", "glsl/sorter.fs", "glsl/shared.glsl");
        putRect.init("glsl/point.vs", "glsl/rect.fs", "glsl/shared.glsl");
        stepper.init("glsl/pass.vs", "glsl/stepB.fs", "glsl/shared.glsl");
        solver.init("glsl/pass.vs", "glsl/solve.fs", "glsl/shared.glsl");
        drawer.init("glsl/texture.vs", "glsl/drawer.fs", "glsl/shared.glsl");
        weight.init("glsl/pass.vs", "glsl/weight.fs", "glsl/shared.glsl");
        hasher.init("glsl/pass.vs", "glsl/hash.fs", "glsl/shared.glsl");
        toList.init("glsl/list.vs", "glsl/list.fs", "glsl/shared.glsl");
        setter.init("glsl/pass.vs", "glsl/set.fs", "glsl/shared.glsl");
        setterf.init("glsl/point.vs", "glsl/setf.fs", "glsl/shared.glsl");
        
        DoubleTexture* dts[] = {
            &positions,
            &velocities,
        };
        
        int n = sizeof(dts)/sizeof(*dts);
        for(int i = 0; i < n; ++i)
            dts[i]->clear();
            
        
        
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        std::vector<float> texels(capacity * 2);
        
        for(int py = 0; py < root; ++py) {
            for(int px = 0; px < root; ++px) {
                int i = (px + py * root) * 2;
                texels[i + 0] = (px + 0.5f) / (float)root;
                texels[i + 1] = (py + 0.5f) / (float)root;
            }
        }
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        glBufferData(GL_ARRAY_BUFFER, 2 * capacity * sizeof(float), texels.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    int create_rect(float x, float y, float w, float h, float sp);
    
    int create_rect(float x, float y, float w, float h, float sp, float vs, float vy);
    
    void solve(float, int);
    
    void render(GLuint, GLuint, GLuint, GLuint, GLuint, float);
};

#endif /* World_hpp */
