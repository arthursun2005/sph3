layout (location = 0) in vec2 p;

void main() {
    gl_Position = vec4(p, 0.0f, 1.0f);
}
