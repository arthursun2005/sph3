layout (location = 0) out int A;

in float a;

void main() {
    if(a < 0.0f)
        discard;
    A = int(a);
}


