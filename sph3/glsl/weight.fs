layout (location = 0) out float A;

uniform sampler2D P;
uniform float h;
uniform int count;
uniform int root;

uniform int list;

uniform isampler2D G;
uniform isampler2D L;

void main() {
    ivec2 q = ivec2(gl_FragCoord.xy);
    int aq = q.x + q.y * root;
    
    if(aq >= count) discard;
    
    vec2 p = texelFetch(P, q, 0).xy;
    
    float weight = 0.0f;
    
    ivec2 ph = ivec2(p/h);
    
    float h2 = h * h;
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            
            ivec2 i = ph + ivec2(x, y);
            
            int hh = int(mod(hash(i), list * list));
            
            int j = texelFetch(L, ivec2(hh%list, hh/list), 0).x;
            
            while(j < count) {
                
                ivec2 cell = texelFetch(G, ivec2(j%root, j/root), 0).xy;
                
                if(cell.x != hh) {
                    break;
                }
                
                if(cell.y == aq) {
                    ++j;
                    continue;
                }
                
                ivec2 joord = ivec2(cell.y%root, cell.y/root);
                vec2 p1 = texelFetch(P, joord, 0).xy;
                
                vec2 dp = p1 - p;
                
                float r2 = dot(dp, dp);
                
                float k = sign(h2 - r2) * 0.5f + 0.5f;
                
                float w = 1.0f - r2/h2;
                
                weight += w * w * w * k;
                
                ++j;
            }
        }
    }
    
    A = weight;
}
