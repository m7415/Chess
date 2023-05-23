#include "Piece.hpp"

#include <iostream>
using namespace std;

//=======================================================================================
// Affichage

void Piece::affiche(bool red) const {
    if (red) {
        cout << "\x1b[31m";
    }
    cout << nom << "\x1b[0m";
}

//========================================================================================
// Getters / Setters

Color Piece::get_couleur() { return couleur; }

string Piece::get_nom() { return nom; }

int Piece::get_position() { return position; }

int Piece::get_value() { return value; }

bool Piece::get_haveMoved() { return haveMoved; }

void Piece::set_haveMoved() { haveMoved = true; }

void Pawn::set_turnOfLastEnPassantEnabling(int turn) {
    turnOfLastEnPassantEnabling = turn;
}

void Pawn::set_sideOfEnPassant(int side) { sideOfEnPassant = side; }

//=============================================================================================
// Move checking

bool Rook::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                int **numSquaresToEdge, int turn) {
    (void)turn;
    coup->flag = Normal;
    int move = coup->end - coup->start;
    int signe = (move > 0 ? 1 : -1);
    move = signe * move;
    int move_geo = false;
    if ((move % 8 == 0) && (signe == 1) &&
        (numSquaresToEdge[position][0] >= (move / 8))) {
        move_geo = true;
    } else if ((move % 8 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][1] >= (move / 8))) {
        move_geo = true;
    } else if ((move / 8 == 0) && (signe == 1) &&
               (numSquaresToEdge[position][3] >= (move % 8))) {
        move_geo = true;
    } else if ((move / 8 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][2] >= (move % 8))) {
        move_geo = true;
    }
    if (move_geo) {
        Piece *pieceDep = echiquier[coup->start];
        Piece *pieceAr = echiquier[coup->end];
        int sens = (move % 8 == 0) ? 8 : 1;
        for (int i = coup->start + signe * sens; signe * i < signe * coup->end;
             i += signe * sens) {
            if (echiquier[i]) {
                coup->flag = Collision;
                return false;
            }
        }
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        return true;
    }
    coup->flag = UngeometricalMove;
    return false;
}

bool Bishop::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                  int **numSquaresToEdge, int turn) {
    (void)turn;
    coup->flag = Normal;
    int move = coup->end - coup->start;
    int signe = (move > 0 ? 1 : -1);
    move = signe * move;
    int move_geo = false;
    if ((move % 7 == 0) && (signe == 1) &&
        (numSquaresToEdge[position][4] >= (move / 7))) {
        move_geo = true;
    } else if ((move % 7 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][5] >= (move / 7))) {
        move_geo = true;
    } else if ((move % 9 == 0) && (signe == 1) &&
               (numSquaresToEdge[position][6] >= (move / 9))) {
        move_geo = true;
    } else if ((move % 9 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][7] >= (move / 9))) {
        move_geo = true;
    }
    if (move_geo) {
        Piece *pieceDep = echiquier[coup->start];
        Piece *pieceAr = echiquier[coup->end];
        int sens = (move % 7 == 0) ? 7 : 9;
        for (int i = coup->start + signe * sens; signe * i < signe * coup->end;
             i += signe * sens) {
            if (echiquier[i]) {
                coup->flag = Collision;
                return false;
            }
        }
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        return true;
    }
    coup->flag = UngeometricalMove;
    return false;
}

bool Queen::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                 int **numSquaresToEdge, int turn) {
    (void)turn;
    coup->flag = Normal;
    int move = coup->end - coup->start;
    int signe = (move > 0 ? 1 : -1);
    move = signe * move;
    int move_geo = false;
    if ((move % 8 == 0) && (signe == 1) &&
        (numSquaresToEdge[position][0] >= (move / 8))) {
        move_geo = true;
    } else if ((move % 8 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][1] >= (move / 8))) {
        move_geo = true;
    } else if ((move / 8 == 0) && (signe == 1) &&
               (numSquaresToEdge[position][3] >= (move % 8))) {
        move_geo = true;
    } else if ((move / 8 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][2] >= (move % 8))) {
        move_geo = true;
    } else if ((move % 7 == 0) && (signe == 1) &&
               (numSquaresToEdge[position][4] >= (move / 7))) {
        move_geo = true;
    } else if ((move % 7 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][5] >= (move / 7))) {
        move_geo = true;
    } else if ((move % 9 == 0) && (signe == 1) &&
               (numSquaresToEdge[position][6] >= (move / 9))) {
        move_geo = true;
    } else if ((move % 9 == 0) && (signe == -1) &&
               (numSquaresToEdge[position][7] >= (move / 9))) {
        move_geo = true;
    }
    if (move_geo) {
        Piece *pieceDep = echiquier[coup->start];
        Piece *pieceAr = echiquier[coup->end];
        int sens = (move % 8 == 0)
                       ? 8
                       : ((move % 7 == 0) ? 7 : ((move % 9 == 0) ? 9 : 1));
        for (int i = coup->start + signe * sens; signe * i < signe * coup->end;
             i += signe * sens) {
            if (echiquier[i]) {
                coup->flag = Collision;
                return false;
            }
        }
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        return true;
    }
    coup->flag = UngeometricalMove;
    return false;
}

bool King::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                int **numSquaresToEdge, int turn) {
    (void)turn;
    coup->flag = Normal;
    int move = coup->end - coup->start;
    int move_geo = false;
    if (move == 8 && (numSquaresToEdge[position][0] > 0)) {
        move_geo = true;
    } else if (move == -8 && (numSquaresToEdge[position][1] > 0)) {
        move_geo = true;
    } else if (move == 1 && (numSquaresToEdge[position][3] > 0)) {
        move_geo = true;
    } else if (move == -1 && (numSquaresToEdge[position][2] > 0)) {
        move_geo = true;
    } else if (move == 7 && (numSquaresToEdge[position][4] > 0)) {
        move_geo = true;
    } else if (move == -7 && (numSquaresToEdge[position][5] > 0)) {
        move_geo = true;
    } else if (move == 9 && (numSquaresToEdge[position][6] > 0)) {
        move_geo = true;
    } else if (move == -9 && (numSquaresToEdge[position][7] > 0)) {
        move_geo = true;
    }
    if (move_geo) {
        Piece *pieceDep = echiquier[coup->start];
        Piece *pieceAr = echiquier[coup->end];
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        return true;
    }
    coup->flag = UngeometricalMove;
    return false;
}

bool Knith::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                 int **numSquaresToEdge, int turn) {
    (void)turn;
    coup->flag = Normal;
    int move = coup->end - coup->start;
    bool move_geo = false;

    if (move == 17 && numSquaresToEdge[position][0] > 1 &&
        numSquaresToEdge[position][3] > 0) {
        move_geo = true;
    } else if (move == 15 && numSquaresToEdge[position][0] > 1 &&
               numSquaresToEdge[position][2] > 0) {
        move_geo = true;
    } else if (move == 10 && numSquaresToEdge[position][3] > 1 &&
               numSquaresToEdge[position][0] > 0) {
        move_geo = true;
    } else if (move == -6 && numSquaresToEdge[position][3] > 1 &&
               numSquaresToEdge[position][1] > 0) {
        move_geo = true;
    } else if (move == 6 && numSquaresToEdge[position][2] > 1 &&
               numSquaresToEdge[position][0] > 0) {
        move_geo = true;
    } else if (move == -10 && numSquaresToEdge[position][2] > 1 &&
               numSquaresToEdge[position][1] > 0) {
        move_geo = true;
    } else if (move == -17 && numSquaresToEdge[position][1] > 1 &&
               numSquaresToEdge[position][2] > 0) {
        move_geo = true;
    } else if (move == -15 && numSquaresToEdge[position][1] > 1 &&
               numSquaresToEdge[position][3] > 0) {
        move_geo = true;
    }

    if (move_geo) {
        Piece *pieceDep = echiquier[coup->start];
        Piece *pieceAr = echiquier[coup->end];
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        return true;
    }
    coup->flag = UngeometricalMove;
    return false;
}

bool Pawn::is_move_pseudo_legal(Move *coup, Piece *echiquier[64],
                                int **numSquaresToEdge, int turn) {
    Piece *pieceDep = echiquier[coup->start];
    Piece *pieceAr = echiquier[coup->end];
    int signe = (pieceDep->get_couleur() == White) ? 1 : -1;
    int move = signe * (coup->end - coup->start);
    coup->flag = Normal;

    if (move == 8 && numSquaresToEdge[position][(signe == 1) ? 0 : 1] > 0) {
        if (pieceAr) {
            coup->flag = Collision;
            return false;
        }
        return true;
    } else if (move == 16 && !haveMoved &&
               numSquaresToEdge[position][(signe == 1) ? 0 : 1] > 1) {
        if (echiquier[coup->start + signe * 8] || pieceAr) {
            coup->flag = Collision;
            return false;
        }
        return true;
    }

    if ((move == 7 && numSquaresToEdge[position][((signe == 1) ? 4 : 5)] > 0) ||
        (move == 9 && numSquaresToEdge[position][((signe == 1) ? 6 : 7)] > 0)) {
        if (pieceAr) {
            if (pieceDep->get_couleur() == pieceAr->get_couleur()) {
                coup->flag = FriendlyFire;
                return false;
            }
            coup->flag = Taking;
            return true;
        }
        if ((turn - turnOfLastEnPassantEnabling) == 1 &&
            move == (8 + sideOfEnPassant)) {
            coup->flag = EnPassant;
            return true;
        }
    }
    coup->flag = UngeometricalMove;
    return false;
}

//======================================================================================
// Pseudo_legal move generation

vector<Move> Rook::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                             int turn) {
    (void)turn;
    vector<Move> list;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;
    int directions[4] = {8, -8, -1, 1};
    for (int k = 0; k < 4; k++) {
        int dir = directions[k];
        int limite = numSquaresToEdge[position][k];
        for (int i = 1; i <= limite; i++) {
            curMov.end = position + i * dir;
            if (echiquier[curMov.end]) {
                if (echiquier[curMov.end]->get_couleur() !=
                    echiquier[position]->get_couleur()) {
                    list.push_back(curMov);
                    break;
                } else {
                    break;
                }
            } else {
                list.push_back(curMov);
            }
        }
    }
    return list;
}

vector<Move> Knith::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                              int turn) {
    (void)turn;
    vector<Move> list;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;
    int directions[4] = {8, -8, -1, 1};
    int curPos;
    int ind;
    for (int k = 0; k < 4; k++) {
        curPos = position;
        if (numSquaresToEdge[position][k] > 1) {
            curPos += 2 * directions[k];
            int dir = ((abs(directions[k]) == 8) ? 1 : 8);

            ind = ((abs(directions[k]) == 8) ? 3 : 0);
            if (numSquaresToEdge[curPos][ind] > 0) {
                curMov.end = curPos + dir;
                if (!echiquier[curMov.end] ||
                    (echiquier[curMov.end]->get_couleur() !=
                     echiquier[position]->get_couleur())) {
                    list.push_back(curMov);
                }
            }

            ind = ((abs(directions[k]) == 8) ? 2 : 1);
            if (numSquaresToEdge[curPos][ind] > 0) {
                curMov.end = curPos - dir;
                if (!echiquier[curMov.end] ||
                    (echiquier[curMov.end]->get_couleur() !=
                     echiquier[position]->get_couleur())) {
                    list.push_back(curMov);
                }
            }
        }
    }
    return list;
}

vector<Move> Bishop::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                               int turn) {
    (void)turn;
    vector<Move> list;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;
    int directions[4] = {7, -7, 9, -9};
    for (int k = 0; k < 4; k++) {
        int dir = directions[k];
        int limite = numSquaresToEdge[position][k + 4];
        for (int i = 1; i <= limite; i++) {
            curMov.end = position + i * dir;
            if (echiquier[curMov.end]) {
                if (echiquier[curMov.end]->get_couleur() !=
                    echiquier[position]->get_couleur()) {
                    list.push_back(curMov);
                    break;
                } else {
                    break;
                }
            } else {
                list.push_back(curMov);
            }
        }
    }
    return list;
}

vector<Move> Queen::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                              int turn) {
    (void)turn;
    vector<Move> list;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;
    int directions[8] = {8, -8, -1, 1, 7, -7, 9, -9};
    for (int k = 0; k < 8; k++) {
        int dir = directions[k];
        int limite = numSquaresToEdge[position][k];
        for (int i = 1; i <= limite; i++) {
            curMov.end = position + i * dir;
            if (echiquier[curMov.end]) {
                if (echiquier[curMov.end]->get_couleur() !=
                    echiquier[position]->get_couleur()) {
                    list.push_back(curMov);
                    break;
                } else {
                    break;
                }
            } else {
                list.push_back(curMov);
            }
        }
    }
    return list;
}

vector<Move> King::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                             int turn) {
    (void)turn;
    vector<Move> list;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;
    int directions[8] = {8, -8, -1, 1, 7, -7, 9, -9};
    for (size_t i = 0; i < 8; i++) {
        int dir = directions[i];
        if (numSquaresToEdge[position][i] > 0) {
            curMov.end = position + dir;
            if (echiquier[curMov.end]) {
                if (echiquier[curMov.end]->get_couleur() !=
                    echiquier[position]->get_couleur()) {
                    list.push_back(curMov);
                }
            } else {
                list.push_back(curMov);
            }
        }
    }
    return list;
}

vector<Move> Pawn::move_list(Piece *echiquier[64], int **numSquaresToEdge,
                             int turn) {
    vector<Move> list;
    int signe = (echiquier[position]->get_couleur() == White) ? 1 : -1;
    Move curMov;
    curMov.flag = Normal;
    curMov.start = position;

    curMov.end = position + signe * 8;
    if ((numSquaresToEdge[position][((signe == 1) ? 0 : 1)] > 0) &&
        !echiquier[curMov.end]) {
        list.push_back(curMov);
    }

    curMov.end = position + signe * 16;
    if ((numSquaresToEdge[position][((signe == 1) ? 0 : 1)] > 1) &&
        !haveMoved && !echiquier[position + signe * 8] &&
        !echiquier[curMov.end]) {
        list.push_back(curMov);
    }

    curMov.end = position + signe * 9;
    if ((numSquaresToEdge[position][((signe == 1) ? 6 : 7)] > 0)) {
        if (echiquier[curMov.end] && (echiquier[curMov.end]->get_couleur() !=
                                      echiquier[position]->get_couleur())) {
            curMov.flag = Taking;
            list.push_back(curMov);
        } else if ((turn - turnOfLastEnPassantEnabling) == 1 &&
                   (8 + sideOfEnPassant) == 9) {
            curMov.flag = EnPassant;
            list.push_back(curMov);
        }
    }

    curMov.end = position + signe * 7;
    if ((numSquaresToEdge[position][((signe == 1) ? 4 : 5)] > 0)) {
        if (echiquier[curMov.end] && (echiquier[curMov.end]->get_couleur() !=
                                      echiquier[position]->get_couleur())) {
            curMov.flag = Taking;
            list.push_back(curMov);
        } else if ((turn - turnOfLastEnPassantEnabling) == 1 &&
                   (8 + sideOfEnPassant) == 7) {
            curMov.flag = EnPassant;
            list.push_back(curMov);
        }
    }
    return list;
}