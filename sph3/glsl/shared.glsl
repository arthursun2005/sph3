#version 410 core

#define pi 3.141592654f

precision mediump float;

int hash(ivec2 p) {
    return ((p.x << 8) * 31) ^ p.y;
}
