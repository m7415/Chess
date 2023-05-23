#include "utils.h"
#include <iostream>

using namespace std;

void compute_numSquaresToEdge(int **numSquaresToEdge) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            numSquaresToEdge[8 * j + i] = new int[8];
            numSquaresToEdge[8 * j + i][0] = 7 - j;             // Nord
            numSquaresToEdge[8 * j + i][1] = j;                 // Sud
            numSquaresToEdge[8 * j + i][2] = i;                 // Ouest
            numSquaresToEdge[8 * j + i][3] = 7 - i;             // Est
            numSquaresToEdge[8 * j + i][4] = MIN(7 - j, i);     // Nord Ouest
            numSquaresToEdge[8 * j + i][5] = MIN(j, 7 - i);     // Sud Est
            numSquaresToEdge[8 * j + i][6] = MIN(7 - j, 7 - i); // Nord Est
            numSquaresToEdge[8 * j + i][7] = MIN(j, i);         // Sud Ouest
        }
    }
}

void free_data(int **numSquaresToEdge) {
    for (int i = 0; i < 64; i++) {
        delete[] numSquaresToEdge[i];
    }
}

void afficheCoup(Move coup) {
    char coupNom[4];
    coupNom[0] = char(coup.start % 8 + 97);
    coupNom[1] = char(coup.start / 8 + 49);
    coupNom[2] = char(coup.end % 8 + 97);
    coupNom[3] = char(coup.end / 8 + 49);
    cout << coupNom << endl;
}