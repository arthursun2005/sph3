layout (location = 0) out vec2 A;

uniform sampler2D P;
uniform sampler2D V;
uniform sampler2D U;
uniform float dt;
uniform float w0;
uniform float pressure;
uniform float h;
uniform float e;
uniform vec2 exf;
uniform int count;
uniform int root;

uniform int list;

uniform isampler2D G;
uniform isampler2D L;

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    int aq = u.x + u.y * root;
    
    if(aq >= count) discard;
    
    vec2 p = texelFetch(P, u, 0).xy;
    vec2 v0 = texelFetch(V, u, 0).xy;
    
    float Ad = max(texelFetch(U, u, 0).x, w0);
    float Ap = (Ad - w0) * pressure;
    
    vec2 accel = vec2(0.0f);
    
    ivec2 p0 = ivec2(p/h);
    
    float h2 = h * h;
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            
            ivec2 i = p0 + ivec2(x, y);
            
            int hh = int(mod(hash(i), list * list));
            
            int j = texelFetch(L, ivec2(hh%list, hh/list), 0).x;
            
            while(j < count) {
                
                ivec2 cell = texelFetch(G, ivec2(j%root, j/root), 0).xy;
                
                if(cell.x != hh)  {
                    break;
                }
                
                if(cell.y == aq) {
                    ++j;
                    continue;
                }
                
                ivec2 joord = ivec2(cell.y%root, cell.y/root);
                vec2 p1 = texelFetch(P, joord, 0).xy;
                
                float Bd = max(texelFetch(U, joord, 0).x, w0);
                float Bp = (Bd - w0) * pressure;
                
                vec2 dp = p1 - p;
                
                float r2 = dot(dp, dp);
                
                vec2 n = normalize(dp);
                
                float k = sign(h2 - r2) * 0.5f + 0.5f;
                
                float w = 1.0f - sqrt(r2) / h;
                
                vec2 vd = texelFetch(V, joord, 0).xy - v0;
                
                //accel -= (((Ap + Bp) / (2.0f * Ad * Bd)) * w * w * h / Ad * 45.0f / pi * k) * n;
                accel -= (w * k * 80.0f) * n;
                accel += (e / (Ad * Bd) * 45.0f / pi * k) * vd;
                
                ++j;
            }
        }
    }
    
    accel += exf;
    
    v0 += accel * dt;
    
    p += v0 * dt;
    
    /*
     
    float dmp = 0.0f;
    
    float a = 1.0f + dmp;
    
    vec2 bound = vec2(30.0f);
    
    v0.x *= sign(p.x) * a - dmp;
    v0.x *= sign(bound.x - p.x) * a - dmp;
    
    v0.y *= sign(p.y) * a - dmp;
    v0.y *= sign(bound.y - p.y) * a - dmp;
    
    */
    
    A = v0;
}


