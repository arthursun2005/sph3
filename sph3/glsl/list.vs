layout (location = 0) in vec2 uv;

uniform isampler2D G;
uniform int root;
uniform int list;
uniform int count;

out float a;

void main() {
    ivec2 coord = ivec2(uv * float(root) - 0.5f);
    int id = coord.x + coord.y * root;
    int k = texelFetch(G, coord, 0).x;
    vec2 c = (vec2(k%list, k/list) + vec2(0.5f)) / float(list);
    a = float(id);
    if(id > 0 && texelFetch(G, ivec2((id - 1)%root, (id - 1)/root), 0).x == k)
        a = -1.0f;
    gl_Position = vec4(c * 2.0f - 1.0f, 0.0f, 1.0f);
}

