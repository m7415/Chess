#include <limits>

#pragma once

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define INFINITY std::numeric_limits<int>::max()

enum Flag {
    UngeometricalMove,
    Collision,
    FriendlyFire,
    Normal,
    Taking,
    EnPassant
};

typedef struct Move {
    int start;
    int end;
    Flag flag;
    int score = 0;
} Move;

typedef struct Mask {
    bool isAttacked;
} Mask;

enum Mode { a, b, c };

enum Color { Black, White, None };

void compute_numSquaresToEdge(int **numSquaresToEdge);

void free_data(int **numSquaresToEdge);

void afficheCoup(Move coup);