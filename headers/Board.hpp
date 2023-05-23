#include "Piece.hpp"
#include "utils.h"
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

#pragma once

class Board {
  public:
    // constructeurs
    Board();

    Board(Board const *echiquier);

    // getters
    Piece *get_piece(int num);

    int get_wKingPos() const;

    int get_bKingPos() const;

    Piece **get_chessboard();

    vector<Piece *> *get_pieces_prises();

    // méthode publique
    Piece *make_move(Move coup, bool vraiCoup, int turn, bool autoQueen);

    void change_piece(int position, Piece *piece);

    void make_Qcastling(bool wTurn);

    void make_Kcastling(bool wTurn);

    void free();

    int eval();

    // méthode d'affichage
    void affiche(int where) const;

    void canonical_position();

  private:
    // --- Attributs
    Piece *chessboard[64];
    int wKingPos;
    int bKingPos;
    vector<Piece *> piecesPrises;
}; // fin class Board