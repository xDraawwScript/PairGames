#include "tools.h"

int main(void) { // avant de lancer le jeu, on vérifie les dimensions du terminal
    if (check_terminal_dimensions(50, 100) == 0) {
        menu();
    } else {
        return -1;
    }
    return 0;
}

