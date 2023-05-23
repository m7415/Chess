#include "Board.hpp"
#include "Position.hpp"
#include "utils.h"
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

#pragma once

class Game {
  public:
    // constructeur par défaut
    Game();

    // méthode de déplacement
    bool move(Board *echiquier);

    // méthode d'affichage
    void affiche(Board echiquier);

    // --- destructeur
    virtual ~Game() {
        free_data(numSquaresToEdge);
        delete[] numSquaresToEdge;
    }

    int compter_coups(Board *position, int depth, bool wTurn, int offset);

    int compter_coups_2(Board *position, int depth, bool wTurn, int offset);

    int minimax(Board *position, int depth, int alpha, int beta, bool wTurn,
                Move *bestMove, int offset);

  private:
    // methodes internes
    bool saisie_correcte(string const &cmd);

    bool saisie_correcte_petitroque(string const &cmd);

    bool saisie_correcte_grandroque(string const &cmd);

    bool is_move_legal(Move coup, Board *echiquier, Color c);

    bool is_check(Color c, Board echiquier);

    bool is_checkmate(Color c, Board *echiquier);

    bool is_Qcastling_legal(Board echiquier);

    bool is_Kcastling_legal(Board echiquier);

    vector<Move> generer_coups(Board *position, bool wTurn, int turn_a);

    // --- Attributs
    bool wTurn;
    bool check;
    int turn;
    int mode;
    int **numSquaresToEdge;
}; // fin class Game