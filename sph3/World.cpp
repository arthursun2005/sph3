//
//  World.cpp
//  sph3
//
//  Created by Arthur Sun on 3/17/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "World.hpp"

void World::solve_once(float dt) {
    map_to_grid();
    sort();
    map_to_list();
    
    //weight_particles();
    solve_pressure(dt);
    step_once(dt);
}

void World::sort() {
    int n = nextPowerOfTwo(count);
    
    for(int k = 2; k <= n; k <<= 1) {
        for(int j = k >> 1; j > 0; j >>= 1) {
            sort_step(j, k, n);
        }
    }
}

void World::step_once(float dt) {
    positions[0].bind();
    positions[1].bind();
    velocities[1].bind();
    
    stepper.bind();
    stepper.uniform1f("dt", dt);
    stepper.uniform1i("P", positions[1].id);
    stepper.uniform1i("V", velocities[1].id);
    
    ::blit(positions[0], 0, 0, root, count/root + 1);
    
    positions.swap();
    
    reset_texture_count;
}

void World::solve_pressure(float dt) {
    velocities[0].bind();
    velocities[1].bind();
    positions[1].bind();
    
    hashGrid[1].bind();
    offsetList.bind();
    
    //pressures.bind();
    
    solver.bind();
    solver.uniform1f("dt", dt);
    solver.uniform1i("P", positions[1].id);
    solver.uniform1i("V", velocities[1].id);
    solver.uniform1i("U", pressures.id);
    solver.uniform1i("count", count);
    solver.uniform1i("root", root);
    solver.uniform1f("h", h);
    solver.uniform1f("e", e);
    solver.uniform1f("w0", p0);
    solver.uniform1f("pressure", u);
    solver.uniform1f("time", glfwGetTime());
    solver.uniform2f("exf", exf);
    
    solver.uniform1i("G", hashGrid[1].id);
    solver.uniform1i("L", offsetList.id);
    solver.uniform1i("list", list);
    
    ::blit(velocities[0], 0, 0, root, count/root + 1);
    
    velocities.swap();
    
    reset_texture_count;
}

void World::weight_particles() {
    positions[1].bind();
    pressures.bind();
    
    hashGrid[1].bind();
    offsetList.bind();
    
    weight.bind();
    weight.uniform1i("count", count);
    weight.uniform1i("root", root);
    weight.uniform1f("h", h);
    weight.uniform1i("P", positions[1].id);
    
    weight.uniform1i("G", hashGrid[1].id);
    weight.uniform1i("L", offsetList.id);
    weight.uniform1i("list", list);
    
    ::blit(pressures, 0, 0, root, count/root + 1);
    
    reset_texture_count;
}

void World::sort_step(int j, int k, int n) {
    hashGrid[0].bind();
    hashGrid[1].bind();
    
    sorter.bind();
    sorter.uniform1i("k", k);
    sorter.uniform1i("j", j);
    sorter.uniform1i("G", hashGrid[1].id);
    sorter.uniform1i("root", root);
    sorter.uniform1i("n", n);
    
    ::blit(hashGrid[0], 0, 0, root, n/root + 1);
    
    hashGrid.swap();
    
    reset_texture_count;
}

void World::map_to_grid() {
    hashGrid[1].bind();
    positions[1].bind();
    
    hasher.bind();
    hasher.uniform1f("h", h);
    hasher.uniform1i("root", root);
    hasher.uniform1i("P", positions[1].id);
    hasher.uniform1i("count", count);
    hasher.uniform1i("list", list);
    
    ::blit(hashGrid[1], 0, 0, root, root);
    
    reset_texture_count;
}

void World::map_to_list() {
    offsetList.bind();
    
    setter.bind();
    setter.uniform4i("a", capacity, 0, 0, 0);
    
    ::blit(offsetList, 0, 0, list, list);
    
    reset_texture_count;
    
    
    hashGrid[1].bind();
    offsetList.bind();
    
    toList.bind();
    toList.uniform1i("list", list);
    toList.uniform1i("root", root);
    toList.uniform1i("G", hashGrid[1].id);
    
    glBindFramebuffer(GL_FRAMEBUFFER, offsetList.target->fbo);
    glBindVertexArray(vao);
    glViewport(0, 0, list, list);
    glDrawArrays(GL_POINTS, 0, count);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    reset_texture_count;
}

void World::solve(float dt, int sub) {
    _dt = dt;
    dt /= (float) sub;
    for(int i = 0; i < sub; ++i)
        solve_once(dt);
}

int World::create_rect(float x, float y, float w, float h, float sp) {
    sp *= this->h;
    
    w /= sp;
    h /= sp;
    
    int iw = roundf(w);
    int ih = roundf(h);
    
    int i = (iw * ih);
    
    assert(count + i <= capacity);
    
    positions[1].bind();
    
    putRect.bind();
    putRect.uniform1f("sp", sp);
    putRect.uniform2f("pos", x, y);
    putRect.uniform1i("hx", iw);
    putRect.uniform1i("hy", ih);
    putRect.uniform1i("count", count);
    putRect.uniform1i("root", root);
    
    blit(positions[1], count, i);
    
    count += i;
    
    reset_texture_count;
    
    return i;
}

int World::create_rect(float x, float y, float w, float h, float sp, float vx, float vy) {
    int i = create_rect(x, y, w, h, sp);
    
    velocities[1].bind();
    
    setterf.bind();
    setterf.uniform4f("a", vx, vy, 0.0f, 0.0f);
    
    blit(velocities[1], count - i, i);
    
    reset_texture_count;
    
    return i;
}

void World::render(GLuint t, GLuint x, GLuint y, GLuint w, GLuint h, float scl) {
    positions[1].bind();
    
    drawer.bind();
    drawer.uniform2f("scl", scl/(float)w, scl/(float)h);
    drawer.uniform1f("size", 0.25f * this->h * scl);
    drawer.uniform1i("T", positions[1].id);
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, t);
    glBindVertexArray(vao);
    glViewport(x, y, w, h);
    glDrawArrays(GL_POINTS, 0, count);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    reset_texture_count;
}
