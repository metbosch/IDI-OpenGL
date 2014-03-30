#ifndef __AUX_FUNCT__
#define __AUX_FUNCT__
int min (int a, int b) {
    return a - ((a - b) & -(b < a));
}

double max (double a, double b) {
    if (a > b) return a;
    return b;
}

double max (double a, double b, double c) {
    if (max(a, b) > max(b, c)) return a;
    else if (max(a, b) < max(b, c)) return c;
    else return b;
}

#define PI (3.14159)
#define TO_RADIANTS (PI/180.0)

#endif