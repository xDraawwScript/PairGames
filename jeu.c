#include "tools.h"

WINDOW *texte;
WINDOW *chrono_win;
WINDOW *score2;

/*
Fonction : jeu
Traitement : Gère le déroulement du jeu des paires, y compris l'affichage du plateau de jeu, la gestion des entrées utilisateur,
              le chronomètre, la victoire/défaite, et l'enregistrement/récupération des scores.
Retour : 0
*/

int jeu()
{
    // Initialisation de l'écran et des couleurs
    clear();
    refresh();
    start_color();
    nodelay(stdscr,TRUE);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    // Configuration de l'affichage
    cbreak();
    int startX = 8;
    int current_card = 0;
    int nb_card_lock = 0;
    int index_value_lock;
    int thread_running = 1;
    struct Carte *cartes[12];
    struct Score *scores[3];
    int fin=0;
    float chronoscore = 0;
    int sleep;
    struct ThreadArgs args = { &chronoscore, &thread_running, &sleep, PTHREAD_MUTEX_INITIALIZER};
    pthread_t thread;
    pthread_create(&thread, NULL, chrono_function, &args);
    noecho();
    curs_set(FALSE);

    // Configuration des fenêtres

    chrono_win=newwin(5,25,0,75);
    mvwprintw(chrono_win, 1, 2, "chrono : ");
    wattron(chrono_win,COLOR_PAIR(5));
    box(chrono_win, 0, 0);
    wattroff(chrono_win,COLOR_PAIR(5));
    wrefresh(chrono_win);
    texte = newwin(5, 75, 0, 0);
    wattron(texte,COLOR_PAIR(5));
    box(texte, 0, 0);
    wattroff(texte,COLOR_PAIR(5));
    wrefresh(texte);
    attron(A_BOLD);
    mvwprintw(texte, 1, 2, "Jeu des paires !");
    mvwprintw(texte, 2, 2, "Trouver les paires en un minimum de temps.");
    attroff(A_BOLD);
    wrefresh(texte); // Refresh the "texte" window to display it
    score2 = newwin(10,100,40,0);

    // Initialisation des cartes

    initialiserValeurCartes(cartes);
    cartes[current_card]->verte = 1;

     // Affichage des cartes

    for (int i = 0; i < 6; i++) { // pour les cartes du haut
        dessinCarte(10, startX -2 + i * espace, cartes[i]->valeur,cartes[i]->selectionne, cartes[i]->verte, cartes[i]->decouvert);
        }

    for (int i = 6; i < 12; i++) {
        dessinCarte(25, startX - 2 + (i - 6) * espace, cartes[i]->valeur, cartes[i]->selectionne, cartes[i]->verte, cartes[i]->decouvert);
    }

    // Boucle principale du jeu

    while (fin==0) {

        if(mode==1){
                // si c'est le mode joueur, on attend des entrées utilisateurs
        handleKeyPress(cartes,&sleep, startX, 12, &current_card , &nb_card_lock, &index_value_lock,&fin);}
        if(mode==2){
            // sinon les entrées seront simulées
            handleKeyPressAuto(cartes, &sleep, startX, 12, &current_card , &nb_card_lock, &index_value_lock,&fin) ;
        }

        refresh();
        // Vérification des dimensions du terminal
        if (check_terminal_dimensions(50, 100) !=0){
        exit_curses(-1);}
    }
    // Arrêt du thread du chronomètre
    thread_running=0;

    // Allocation dynamique de la mémoire pour les scores

    for(int i=0;i<3;i++){
        scores[i]=malloc(sizeof(struct Score));}

    // Initialisation des scores à zéro

    for(int e=0;e<3;e++){
        scores[e]->temps=0;}

    recuperer_score(scores,"scores.txt");  // Récupération des scores

    // Affichage des résultats en fonction de la fin du jeu

    if(fin==1){ // En cas de victoire
        checkscore(scores,chronoscore,"scores.txt");
        wattron(score2,COLOR_PAIR(2));
        box(score2, 0, 0);
        wattroff(score2,COLOR_PAIR(2));
        mvwprintw(score2, 2, 38, " Victoire ! Scores :");
        mvwprintw(score2, 1, 35, "Appuyez sur 'q' pour quitter");
        for (int i = 0; i < 3; i++){ // affichage des trois scores
            if (scores[i]->temps != 0){
            mvprintw(44 + i, 36, "%d. pseudo : %s temps : %.1f \n",i+1 , scores[i]->pseudo, scores[i]->temps);
        }
    }
    wrefresh(score2);
    }

    if(fin==-1){ // En cas de fin par interruption (pression de 'q')

    wattron(score2,COLOR_PAIR(2));
    box(score2, 0, 0);
    wattroff(score2,COLOR_PAIR(2));

    mvwprintw(score2, 3, 40, "Scores à battre :");
    mvwprintw(score2, 1, 35, "Appuyez sur 'q' pour quitter");

    for (int i = 0; i < 3; i++){

        if (scores[i]->temps != 0){
            mvwprintw(score2, 5 + i, 36, "%d. pseudo : %s temps : %.1f \n",i+1 , scores[i]->pseudo, scores[i]->temps);
        }
    }
       wrefresh(score2);
    }
        for (int i = 0; i < 12; ++i) { // libérer la mémoire
        free(cartes[i]);
        cartes[i] = NULL;
    }
    noecho();
    refresh();
        int ch;
    while ((ch = getch()) != 'q') { // q pour fermer le jeu
    }
    endwin();
    return 0;

}
