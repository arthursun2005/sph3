layout (location = 0) out ivec2 A;

uniform sampler2D P;
uniform float h;
uniform int root;
uniform int count;
uniform int list;

void main() {
    ivec2 coord = ivec2(gl_FragCoord.xy);
    int q = coord.x + coord.y * root;
    if(q >= count) {
        A = ivec2(0x7fffffff, q);
    }else{
        ivec2 p = ivec2(texelFetch(P, coord, 0).xy/h);
        A = ivec2(mod(hash(p), list * list), q);
    }
}
