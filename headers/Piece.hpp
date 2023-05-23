#include "utils.h"
#include <iostream>
#include <vector>
using namespace std;

#pragma once

class Piece {
  public:
    // constructeur par défaut
    Piece(Color c, string name, int pos, int score)
        : couleur(c), nom(name), position(pos), haveMoved(false), value(score) {
    }

    virtual Piece *Clone() const = 0;

    // methode virtuelle pour les coups possibles
    virtual bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                      int **numSquaresToEdge, int turn) = 0;

    virtual vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                                   int turn) = 0;

    virtual void set_turnOfLastEnPassantEnabling(int turn) { (void)turn; }

    virtual void set_sideOfEnPassant(int side) { (void)side; }

    void set_haveMoved();

    // méthode d'affichage
    void affiche(bool red) const;

    // getters

    Color get_couleur();

    string get_nom();

    int get_position();

    bool get_haveMoved();

    int get_value();

    // setters

    void set_position(int i) { position = i; }

    // --- destructeur
    virtual ~Piece() {}

  protected:
    // --- Attributs
    Color couleur;
    string nom;
    int position;
    bool haveMoved;
    int value;
}; // fin class Piece

class Rook : public Piece {
  public:
    // Constructeur
    Rook(Color c, string name, int pos) : Piece(c, name, pos, 5) {}

    virtual Piece *Clone() const { return new Rook(*this); }

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
};

class Knith : public Piece {
  public:
    // Constructeur
    Knith(Color c, string name, int pos) : Piece(c, name, pos, 3) {}

    virtual Piece *Clone() const { return new Knith(*this); }

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
};

class Bishop : public Piece {
  public:
    // Constructeur
    Bishop(Color c, string name, int pos) : Piece(c, name, pos, 3) {}

    virtual Piece *Clone() const { return new Bishop(*this); }

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
};

class Queen : public Piece {
  public:
    // Constructeur
    Queen(Color c, string name, int pos) : Piece(c, name, pos, 9) {}

    virtual Piece *Clone() const { return new Queen(*this); }

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
};

class King : public Piece {
  public:
    // Constructeur
    King(Color c, string name, int pos) : Piece(c, name, pos, 50) {}

    virtual Piece *Clone() const { return new King(*this); }

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
};

class Pawn : public Piece {
  public:
    // Constructeur
    Pawn(Color c, string name, int pos)
        : Piece(c, name, pos, 1), turnOfLastEnPassantEnabling(-2),
          sideOfEnPassant(0) {}

    virtual Piece *Clone() const { return new Pawn(*this); }

    // getters / setters

    void set_turnOfLastEnPassantEnabling(int turn);

    void set_sideOfEnPassant(int side);

    // methode pour les coups possibles
    bool is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                              int **numSquaresToEdge, int turn);

    vector<Move> move_list(Piece *echiquier[64], int **numSquaresToEdge,
                           int turn);

  private:
    int turnOfLastEnPassantEnabling;
    int sideOfEnPassant;
};