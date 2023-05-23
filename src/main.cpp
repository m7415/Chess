#include "Game.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Board *echiquier = new Board();
    Game partie;

    int nbCoups;
    for (size_t i = 1; i < 5; i++) {
        nbCoups = partie.compter_coups_2(echiquier, i, true, 0);
        cout << "Depth = " << i << " ; nbCoups = " << nbCoups << endl;
    }

    // boucle de jeu, s'arrete a la fin de la partie
    bool stop = false;
    /* do {
        partie.affiche(*echiquier);
        stop = partie.move(echiquier);
    } while (!stop); */
    echiquier->canonical_position();
    echiquier->free();
    delete echiquier;
}