#include "tools.h"

int mode;

/*
Fonction : menu
Traitement : Affiche le menu du jeu avec deux options (1 joueur ou Autoplayer) et récupère le choix de l'utilisateur.
Retour : 0 si tout s'est bien déroulé, -2 en cas d'erreur lors de l'initialisation de ncurses
*/

int menu(){

    int ch; // Variable pour stocker le mode choisi
    WINDOW *acceuil; // Fenêtre pour l'affichage du menu
    cbreak(); // Désactive le buffering de ligne
    start_color(); // Active la prise en charge des couleurs
    resize_term(50, 100); // Redimensionnement de la fenêtre
    refresh();

    // Définition de deux paires de couleurs personnalisées
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    // AFFICHAGE

    // Obtenir les dimensions de l'écran
    int height, width;
    getmaxyx(stdscr, height, width);

    // Créer la fenêtre supérieure
    acceuil = subwin(stdscr, height/7, width, 0, 0);

    // Appliquer la première paire de couleurs à la bordure de la fenêtre supérieure
    wbkgd(acceuil, COLOR_PAIR(1));
    wborder(acceuil, '|', '|', '-', '-', '+', '+', '+', '+');

    // Utiliser la paire de couleurs 1 pour afficher un texte au milieu
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "     _ ___ _   _   ___  ___   ___  _   ___ ___ ___ ___ ");
    mvprintw(2, 20, "   _| | __| | | | |   \\| __| | _ \\/_\\ |_ _| _ \\ __/ __|");
    mvprintw(3, 20, " | || | _|| |_| | | |) | _|  |  _/ _ \\ | || _ / _|\\__ \\");
    mvprintw(4, 20, "  \\__/|___|\\___/  |___/|___| |_|/_/ \\_\\___|_|_\\___|___/");
    // Désactiver la paire de couleurs après le texte

     attrset(COLOR_PAIR(2)| A_BOLD | A_DIM);

    // Utiliser la paire de couleurs 2 pour afficher le menu

    mvprintw(height / 4 + 1, 30,"   __  __ ______ _   _ _    _  ");
    mvprintw(height / 4 + 2, 30,"  |  \\/  |  ____| \\ | | |  | | ");
    mvprintw(height / 4 + 3, 30,"  | \\  / | |__  |  \\| | |  | | ");
    mvprintw(height / 4 + 4, 30,"  | |\\/| |  __| | . ` | |  | | ");
    mvprintw(height / 4 + 5, 30,"  | |  | | |____| |\\  | |__| | ");
    mvprintw(height / 4 + 6, 30,"  |_|  |_|______|_| \\_\\_____/  ");

    mvprintw(height / 4 + 9, 20, " 1. 1 joueur");
    mvprintw(height / 4 + 9, 57, " 2. Autoplayer");
    mvprintw(height / 4 + 13, 29, " Choisissez votre mode (1 ou 2) : ");
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    wrefresh(acceuil); // Rafraîchir l'écran pour afficher le texte

    echo(); // activation de l'entrée utilisateur
    while (1) { // on attends l'entrée utilisateur
        ch = getch();
        if (ch == '1' || ch == '2') { // Lire le mode choisi par l'utilisateur
            mode = ch - '0'; // Convertir le caractère en entier
            break;
        }
        else{   // si != 1 ou 2, on relance la boucle
            refresh();
            menu();
        }
    }

    // gestion de l'entrée utilisateur
    if (mode == 1|| mode==2) {
            refresh();
            jeu();
    }

    return 0;
}
