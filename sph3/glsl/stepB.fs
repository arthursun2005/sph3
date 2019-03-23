layout (location = 0) out vec2 A;

uniform sampler2D P;
uniform sampler2D V;
uniform float dt;

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    vec2 p0 = texelFetch(P, u, 0).xy;
    vec2 v = texelFetch(V, u, 0).xy;
    p0 += v * dt;
    
    //p0 = clamp(p0, vec2(0.0f), vec2(30.0f));
    
    A = p0;
}

