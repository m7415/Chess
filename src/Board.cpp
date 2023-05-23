#include "Board.hpp"

#include <iostream>
using namespace std;

// constructeur
Board::Board() : wKingPos(4), bKingPos(60) {
    for (int i = 0; i < 64; i++) {
        chessboard[i] = nullptr;
    }
    // Constructeur (Couleur,nom_affiché, case)
    chessboard[0] = new Rook(White, "\u2656", 0);
    chessboard[1] = new Knith(White, "\u2658", 1);
    chessboard[2] = new Bishop(White, "\u2657", 2);
    chessboard[3] = new Queen(White, "\u2655", 3);
    chessboard[4] = new King(White, "\u2654", 4);
    chessboard[5] = new Bishop(White, "\u2657", 5);
    chessboard[6] = new Knith(White, "\u2658", 6);
    chessboard[7] = new Rook(White, "\u2656", 7);
    chessboard[56] = new Rook(Black, "\u265C", 56);
    chessboard[57] = new Knith(Black, "\u265E", 57);
    chessboard[58] = new Bishop(Black, "\u265D", 58);
    chessboard[59] = new Queen(Black, "\u265B", 59);
    chessboard[60] = new King(Black, "\u265A", 60);
    chessboard[61] = new Bishop(Black, "\u265D", 61);
    chessboard[62] = new Knith(Black, "\u265E", 62);
    chessboard[63] = new Rook(Black, "\u265C", 63);

    // allocation des pions
    for (int i = 0; i < 8; i++) {
        chessboard[i + 8] = new Pawn(White, "\u2659", i + 8);
        chessboard[i + 48] = new Pawn(Black, "\u265F", i + 48);
    }
}
// constructeur de copie
Board::Board(Board const *echiquier)
    : wKingPos(echiquier->wKingPos), bKingPos(echiquier->bKingPos) {
    for (int i = 0; i < 64; i++) {
        if (echiquier->chessboard[i]) {
            chessboard[i] = echiquier->chessboard[i]->Clone();
        } else {
            chessboard[i] = nullptr;
        }
    }
}

// deplacement

Piece *Board::make_move(Move coup, bool vraiCoup, int turn, bool autoQueen) {
    Piece *p = chessboard[coup.end];
    if (coup.flag == EnPassant) {
        chessboard[coup.end - 8] = nullptr;
    }

    chessboard[coup.end] = chessboard[coup.start];
    chessboard[coup.start] = nullptr;

    Color c = chessboard[coup.end]->get_couleur();
    string nom = (c == White) ? "\u2659" : "\u265F";
    int side = (c == White) ? 7 : 0;
    int signe = (c == White) ? 1 : -1;
    int move = signe * (coup.end - coup.start);
    string piece;
    bool entreeValide = false;

    if (vraiCoup) {
        chessboard[coup.end]->set_position(coup.end);
        chessboard[coup.end]->set_haveMoved();
    }

    if (vraiCoup && (chessboard[coup.end]->get_nom() == nom)) {
        if (coup.end / 8 == side) {
            vector<Piece *> none;
            if (!autoQueen) {
                cout
                    << "En quelle piece (Q,R,B,K) voulez-vous promouvoir votre "
                       "pion ?";
                cin >> piece;
                regex mouvmtpattern("(Q|R|B|K)");
                while (!entreeValide) {
                    entreeValide = regex_match(piece, mouvmtpattern);
                    if (entreeValide) {
                        delete chessboard[coup.end];
                        if (piece == "Q") {
                            chessboard[coup.end] = new Queen(
                                c, ((c == White) ? "\u2655" : "\u265B"),
                                coup.end);
                        }
                        if (piece == "R") {
                            chessboard[coup.end] = new Rook(
                                c, ((c == White) ? "\u2656" : "\u265C"),
                                coup.end);
                        }
                        if (piece == "B") {
                            chessboard[coup.end] = new Bishop(
                                c, ((c == White) ? "\u2657" : "\u265D"),
                                coup.end);
                        }
                        if (piece == "K") {
                            chessboard[coup.end] = new Knith(
                                c, ((c == White) ? "\u2658" : "\u265E"),
                                coup.end);
                        }
                    } else {
                        cout << "Entree non valide" << endl;
                    }
                }
            } else {
                chessboard[coup.end] = new Queen(
                    c, ((c == White) ? "\u2655" : "\u265B"), coup.end);
            }
        }
    }
    if (move == 16) {
        if ((coup.end % 8 < 7) && chessboard[coup.end + 1]) {
            chessboard[coup.end + 1]->set_turnOfLastEnPassantEnabling(turn);
            chessboard[coup.end + 1]->set_sideOfEnPassant(-1);
        }
        if ((coup.end % 8 != 0) && chessboard[coup.end - 1]) {
            chessboard[coup.end - 1]->set_turnOfLastEnPassantEnabling(turn);
            chessboard[coup.end - 1]->set_sideOfEnPassant(+1);
        }
    }
    return p;
}

void Board::make_Qcastling(bool wTurn) {
    int offset = wTurn ? 0 : 56;
    chessboard[offset + 2] = chessboard[offset + 4];
    chessboard[offset + 4] = nullptr;
    chessboard[offset + 3] = chessboard[offset + 0];
    chessboard[offset + 0] = nullptr;
}

void Board::make_Kcastling(bool wTurn) {
    int offset = wTurn ? 0 : 56;
    chessboard[offset + 6] = chessboard[offset + 4];
    chessboard[offset + 4] = nullptr;
    chessboard[offset + 5] = chessboard[offset + 7];
    chessboard[offset + 7] = nullptr;
}

void Board::change_piece(int position, Piece *piece) {
    chessboard[position] = piece;
}

// free
void Board::free() {
    Piece *piece;
    for (int i = 0; i < 64; i++) {
        piece = chessboard[i];
        if (piece) {
            delete piece;
        }
    }
    for (size_t i = 0; i < piecesPrises.size(); i++) {
        piece = piecesPrises[i];
        delete piece;
        piecesPrises.pop_back();
    }
}

// getters

Piece *Board::get_piece(int num) { return chessboard[num]; }

int Board::get_wKingPos() const { return wKingPos; }

int Board::get_bKingPos() const { return bKingPos; }

Piece **Board::get_chessboard() { return chessboard; }

vector<Piece *> *Board::get_pieces_prises() { return &piecesPrises; }

// evaluation pour minimax

int Board::eval() {
    int sum = 0;
    for (size_t i = 0; i < 64; i++) {
        if (chessboard[i]) {
            int signe = (chessboard[i]->get_couleur() == White) ? 1 : -1;
            string nom = chessboard[i]->get_nom();
            sum += signe * chessboard[i]->get_value();
        }
    }
    return sum;
}

// afficher

void Board::affiche(int where) const {

    string space5 = string(5, ' ');
    cout << endl;
    cout << "     a     b     c     d     e     f     g     h    " << endl;
    cout << "  +-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
    for (int i = 7; i >= 0; i--) {
        cout << i + 1 << " "; // numérotation ligne dans affichage
        for (int j = 0; j < 8; j++) {
            cout << "|";
            if (chessboard[8 * i + j]) {
                cout << "\u0020\u0020"; // U+0020 est un esapce utf-8 taille
                                        // police
                if (8 * i + j == where) {
                    chessboard[8 * i + j]->affiche(true);
                } else {
                    chessboard[8 * i + j]->affiche(false);
                }
                cout << "\u0020"
                     << " ";
            } else
                cout << space5; // 2 ascii spaces
        }
        Piece *piece;
        if (i == 7) {
            cout << "| Pieces blanches prises :";
            for (size_t i = 0; i < piecesPrises.size(); i++) {
                piece = piecesPrises[i];
                if (piece->get_couleur() == White) {
                    cout << "  " << piece->get_nom();
                }
            }
        } else if (i == 0) {
            cout << "| Pieces noires prises :";
            for (size_t i = 0; i < piecesPrises.size(); i++) {
                piece = piecesPrises[i];
                if (piece->get_couleur() == Black) {
                    cout << "  " << piece->get_nom();
                }
            }
        } else {
            cout << "|";
        }
        cout << "\n  +-----+-----+-----+-----+-----+-----+-----+-----+";
        cout << endl;
    }
}

void Board::canonical_position() {
    string output;
    char c;
    char p;
    for (int i = 0; i < 64; i++) {
        if (chessboard[i]) {
            c = (chessboard[i]->get_couleur() == White) ? 'w' : 'b';
            string nom = chessboard[i]->get_nom();
            if (nom == "\u2655" || nom == "\u265B") {
                p = 'Q';
            } else if (nom == "\u2656" || nom == "\u265C") {
                p = 'R';
            } else if (nom == "\u2657" || nom == "\u265D") {
                p = 'B';
            } else if (nom == "\u2658" || nom == "\u265E") {
                p = 'N';
            } else if (nom == "\u2654" || nom == "\u265A") {
                p = 'K';
            } else {
                p = 'P';
            }
            output += c;
            output += p;
        }
        output += ',';
    }
    cout << output << endl;
}