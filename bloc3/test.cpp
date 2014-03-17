#include<iostream>
using namespace std;

int min (int a, int b) {
    return a - ((a - b) & -(b < a));
}

int main () {
    cout << min(50, 1) << endl;
}
