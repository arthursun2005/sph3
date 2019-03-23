layout (location = 0) out ivec2 A;

uniform isampler2D G;

uniform int j;
uniform int k;
uniform int n;
uniform int root;

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    
    int i = u.x + u.y * root;
    
    if(i >= n)
        discard;
    
    int ixj = i ^ j;
    
    ivec2 u2 = ivec2(ixj%root, ixj/root);
    
    ivec2 p1 = texelFetch(G, u, 0).xy;
    
    ivec2 p2 = texelFetch(G, u2, 0).xy;
    
    //int b0 = sign(i&k);
    
    //int b1 = sign(i - ixj);
    
    if(((i&k) == 0) ^^ (i > ixj)) {
        A = p1.x <= p2.x ? p1 : p2;
    }else{
        A = p1.x >= p2.x ? p1 : p2;
    }
}

