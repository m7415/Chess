#include "Game.hpp"

#include <iostream>
using namespace std;

// Constructeur

Game::Game() : wTurn(true), check(false), turn(0), mode(-1) {
    numSquaresToEdge = new int *[64];
    compute_numSquaresToEdge(numSquaresToEdge);
    cout << "Bienvenue dans une nouvelle partie de echecs 2000, les echecs du "
            "futur !"
         << endl
         << endl;
    while (mode != 1 && mode != 2 && mode != 3 && mode != 4) {
        cout << "Veuillez choisir parmis les options suivantes : " << endl
             << "    ->Mode 2 joueurs : 1" << endl
             << "    ->Mode contre l'ordinateur qui joue noir: 2" << endl
             << "    ->Mode contre l'ordinateur qui joue blanc : 3" << endl
             << "    ->Quitter : 4" << endl
             << "Choix : ";
        cin >> mode;
    }
}

// Gestion d'un tour
bool Game::move(Board *echiquier) {
    if (mode == 4) {
        return true;
    }
    string mouvement;
    Move coup;
    coup.flag = Normal;
    Color c = wTurn ? White : Black;
    Color compC = (mode == 1) ? None : ((mode == 2) ? Black : White);
    if (check) {
        if (is_checkmate(c, echiquier)) {
            cout << "Les " << (wTurn ? "blancs" : "noirs")
                 << " sont en echec et mat." << endl;
            cout << "Fin de la partie" << endl;
            return true;
        }
    }
    if (c != compC) {
        cout << "Tour " << turn << " : mouvement des "
             << (wTurn ? "blancs" : "noirs") << "?  ";
        cin >> mouvement;
        if (mouvement == "/quit") {
            cout << endl;
            return true;
        }
        if (saisie_correcte(mouvement)) {
            coup.start =
                8 * (int(mouvement[1]) - 48 - 1) + (int(mouvement[0]) - 97);
            coup.end =
                8 * (int(mouvement[3]) - 48 - 1) + (int(mouvement[2]) - 97);
            Piece *pieceDep = echiquier->get_piece(coup.start);
            if (pieceDep) {
                if (pieceDep->get_couleur() == c) {
                    if (is_move_legal(coup, echiquier, c)) {
                        if (Piece *p =
                                echiquier->make_move(coup, true, turn, false)) {
                            echiquier->get_pieces_prises()->push_back(p);
                        }
                        wTurn = !wTurn;
                        turn++;
                    } else {
                        cout << "Un " << pieceDep->get_nom()
                             << " ne peut pas faire ce mouvement."
                             << " Code : " << coup.flag << endl;
                    }
                } else {
                    cout << "Ce n'est pas aux " << (wTurn ? "noirs" : "blancs")
                         << " de jouer" << endl;
                }
            } else {
                cout << "Pas de piece au depart de ce mouvement" << endl;
            }
        } else if (saisie_correcte_petitroque(mouvement)) {
            check = is_check(c, *echiquier);
            if (is_Kcastling_legal(*echiquier)) {
                echiquier->make_Kcastling(wTurn);
                wTurn = !wTurn;
                turn++;
            } else {
                cout << "Roque illegal dans cette situation" << endl;
            }
        } else if (saisie_correcte_grandroque(mouvement)) {
            check = is_check(c, *echiquier);
            if (is_Qcastling_legal(*echiquier)) {
                echiquier->make_Qcastling(wTurn);
                wTurn = !wTurn;
                turn++;
            } else {
                cout << "Roque illegal dans cette situation" << endl;
            }
        } else {
            cout << "Saisi incorrecte (coup non algebrique)" << endl;
        }
        return false;
    } else {
        minimax(echiquier, 4, -INFINITY, INFINITY, wTurn, &coup, 0);
        if (Piece *p = echiquier->make_move(coup, true, turn, true)) {
            echiquier->get_pieces_prises()->push_back(p);
        }
        wTurn = !wTurn;
        turn++;
        return false;
    }
}

// saisie
bool Game::saisie_correcte(string const &cmd) {
    regex mouvmtpattern("[a-h][1-8][a-h][1-8]");
    return regex_match(cmd, mouvmtpattern);
}

bool Game::saisie_correcte_petitroque(string const &cmd) {
    regex mouvmtpattern("(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}

bool Game::saisie_correcte_grandroque(string const &cmd) {
    regex mouvmtpattern("(O|o|0)-(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}

// Move légaux / echec

bool Game::is_move_legal(Move coup, Board *echiquier, Color c) {
    Piece *pieceDep = echiquier->get_piece(coup.start);
    Piece *pieceAr = echiquier->get_piece(coup.end);
    if (pieceDep->is_move_pseudo_legal(&coup, echiquier->get_chessboard(),
                                       numSquaresToEdge, turn)) {
        Piece *saveDep = echiquier->get_piece(coup.start);
        Piece *saveAr = echiquier->get_piece(coup.end);
        echiquier->make_move(coup, false, turn, false);
        check = is_check(c, *echiquier);
        echiquier->change_piece(coup.start, saveDep);
        echiquier->change_piece(coup.end, saveAr);
        if (!check) {
            return true;
        }
    }
    return false;
}

bool Game::is_check(Color c, Board echiquier) {
    int kingPos =
        (c == White) ? echiquier.get_wKingPos() : echiquier.get_bKingPos();
    Piece **plateau = echiquier.get_chessboard();
    Move coup;
    coup.end = kingPos;
    for (int i = 0; i < 64; i++) {
        coup.start = i;
        Piece *piece = echiquier.get_piece(i);
        if (piece && piece->get_couleur() != c &&
            piece->is_move_pseudo_legal(&coup, plateau, numSquaresToEdge,
                                        turn)) {
            return true;
        }
    }
    return false;
}

bool Game::is_checkmate(Color c, Board *echiquier) {
    vector<Move> list = generer_coups(echiquier, c == White, turn);
    Move coup;
    bool tempCheck;
    for (size_t l = 0; l < list.size(); l++) {
        coup = list[l];
        if (is_move_legal(coup, echiquier, c)) {
            return false;
        }
    }
    return true;
}

// Rocque
bool Game::is_Qcastling_legal(Board echiquier) {
    if (check) {
        return false;
    }
    int offset = wTurn ? 0 : 56;
    Color c = wTurn ? White : Black;
    string nomK = wTurn ? "\u2654" : "\u265A";
    string nomR = wTurn ? "\u2656" : "\u265C";
    Piece *king = echiquier.get_piece(offset + 4);
    Piece *rook = echiquier.get_piece(offset + 0);
    bool king_legit = (king && king->get_nom() == nomK &&
                       king->get_couleur() == c && !king->get_haveMoved());
    bool rook_legit = (rook && rook->get_nom() == nomR &&
                       rook->get_couleur() == c && !rook->get_haveMoved());
    return king_legit && rook_legit && !echiquier.get_piece(offset + 1) &&
           !echiquier.get_piece(offset + 2) && !echiquier.get_piece(offset + 3);
}

bool Game::is_Kcastling_legal(Board echiquier) {
    if (check) {
        return false;
    }
    int offset = wTurn ? 0 : 56;
    Color c = wTurn ? White : Black;
    string nomK = wTurn ? "\u2654" : "\u265A";
    string nomR = wTurn ? "\u2656" : "\u265C";
    Piece *king = echiquier.get_piece(offset + 4);
    Piece *rook = echiquier.get_piece(offset + 7);
    bool king_legit = (king && king->get_nom() == nomK &&
                       king->get_couleur() == c && !king->get_haveMoved());
    bool rook_legit = (rook && rook->get_nom() == nomR &&
                       rook->get_couleur() == c && !rook->get_haveMoved());
    return king_legit && rook_legit && !echiquier.get_piece(offset + 5) &&
           !echiquier.get_piece(offset + 6);
}

// Affichage
void Game::affiche(Board echiquier) {
    Color c = wTurn ? White : Black;
    int kingPos = wTurn ? echiquier.get_wKingPos() : echiquier.get_bKingPos();
    check = is_check(c, echiquier);
    int red = check ? kingPos : -1;
    echiquier.affiche(red);
}

// Recherche

vector<Move> Game::generer_coups(Board *position, bool wTurn, int turn_a) {
    Color c = wTurn ? White : Black;
    vector<Move> coups;
    for (int i = 0; i < 64; i++) {
        Piece *piece = position->get_piece(i);
        if (piece && piece->get_couleur() == c) {
            vector<Move> coups_p = piece->move_list(position->get_chessboard(),
                                                    numSquaresToEdge, turn_a);
            for (size_t j = 0; j < coups_p.size(); j++) {
                Piece *piece = position->get_piece(coups_p[j].end);
                if (piece && piece->get_couleur() != c) {
                    coups_p[j].score += piece->get_value();
                }
                coups.push_back(coups_p[j]);
            }
        }
    }
    return coups;
}

int Game::compter_coups(Board *position, int depth, bool wTurn, int offset) {
    Color c = wTurn ? White : Black;
    if (depth == 0 || is_checkmate(c, position)) {
        return 0;
        position->free();
    }

    int nbCoups = 0;
    vector<Move> coups = generer_coups(position, wTurn, turn + offset);
    sort(coups.begin(), coups.end(),
         [](Move a, Move b) { return a.score < b.score; });
    for (int i = 0; i < coups.size(); i++) {
        Move coup = coups[i];
        if (depth == 1) {
            nbCoups++;
        }
        Board *newPos = new Board(position);
        if (Piece *p = newPos->make_move(coup, true, turn + offset, true)) {
            newPos->get_pieces_prises()->push_back(p);
        }
        if (!position->get_piece(coup.start)
                 ->is_move_pseudo_legal(&coup, position->get_chessboard(),
                                        numSquaresToEdge, turn + offset)) {
            cout << "Coup fautif : " << endl;
            newPos->affiche(-1);
        }
        nbCoups += compter_coups(newPos, depth - 1, !wTurn, offset + 1);
        newPos->free();
        delete newPos;
    }
    return nbCoups;
}

int Game::compter_coups_2(Board *position, int depth, bool wTurn, int offset) {
    Color c = wTurn ? White : Black;
    if (depth == 0 || is_checkmate(c, position)) {
        return 0;
    }
    int nbCoups = 0;
    for (int i = 0; i < 64; i++) {
        Move coup;
        coup.start = i;
        Piece *piece = position->get_piece(i);
        if (piece && piece->get_couleur() == c) {
            for (size_t j = 0; j < 64; j++) {
                coup.end = j;

                if (is_move_legal(coup, position, c)) {
                    if (depth == 1) {
                        nbCoups++;
                    }
                    Board *newPos = new Board(position);
                    string nom = newPos->get_piece(coup.start)->get_nom();
                    if (Piece *p = newPos->make_move(coup, true, turn + offset,
                                                     true)) {
                        newPos->get_pieces_prises()->push_back(p);
                    }
                    nbCoups +=
                        compter_coups_2(newPos, depth - 1, !wTurn, offset + 1);
                    newPos->free();
                    delete newPos;
                }
            }
        }
    }
    return nbCoups;
}

int Game::minimax(Board *position, int depth, int alpha, int beta, bool wTurn,
                  Move *bestMove, int offset) {
    Color c = wTurn ? White : Black;
    if (depth == 0 || is_checkmate(c, position)) {
        return position->eval();
        position->free();
    }
    int eval = 0;
    if (wTurn) {
        int max_eval = -INFINITY;
        vector<Move> coups = generer_coups(position, wTurn, turn + offset);
        sort(coups.begin(), coups.end(),
             [](Move a, Move b) { return a.score < b.score; });
        for (int i = 0; i < coups.size(); i++) {
            Move coup = coups[i];
            Board *newPos = new Board(position);
            if (Piece *p = newPos->make_move(coup, true, turn + offset, true)) {
                newPos->get_pieces_prises()->push_back(p);
            }
            eval = minimax(newPos, depth - 1, alpha, beta, false, nullptr,
                           offset + 1);
            newPos->free();
            delete newPos;
            if (eval > max_eval) {
                max_eval = eval;
                if (offset == 0) {
                    *bestMove = coup;
                }
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return max_eval;
    } else {
        int min_eval = INFINITY;
        vector<Move> coups = generer_coups(position, wTurn, turn + offset);
        sort(coups.begin(), coups.end(),
             [](Move a, Move b) { return a.score < b.score; });
        for (int i = 0; i < coups.size(); i++) {
            Move coup = coups[i];
            Board *newPos = new Board(position);
            if (Piece *p = newPos->make_move(coup, true, turn + offset, true)) {
                newPos->get_pieces_prises()->push_back(p);
            }
            eval = minimax(newPos, depth - 1, alpha, beta, true, nullptr,
                           offset + 1);
            newPos->free();
            delete newPos;
            if (eval < min_eval) {
                min_eval = eval;
                if (offset == 0) {
                    *bestMove = coup;
                }
            }
            beta = MIN(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return min_eval;
    }
}