#include <iostream>
#include <vector>
#include <string>
#define N 97


int main() {
    int n, m, q;
    std::cin >> n >> m >> q;
    int matrix_in[n][m]; string matrix_out[n][m];
    for(int i = 0; i < n; i++)
        for(int c = 0; c < m; c++)
            std::cin >> matrix_in[i][c];

    for(int i = 0; i < n; i++)
        for(int c = 0; c < m; c++)
            cin >> matrix_out[i][c];

    std::cout << "digraph {" << endl << "    rankdir = LR" << endl << "    dummy [label = \"\", shape = none]" << endl;

    for(int i = 0; i < n; i++)
       std::cout << "    " << i << " [shape = circle]" << std::endl;

    std::cout << "    dummy -> " << q << endl;

    for(int i = 0; i < n; i++) {
       for(int c = 0; c < m; c++) {
           char str = N + c;
           std::cout << "    " << i << " -> " << matrix_in[i][c] << " [label = \"" << str  << "(" + matrix_out[i][c] + ")\"]" << std::endl;
       }
    }
    std::cout << "}" << std::endl;
    return 0;

}
