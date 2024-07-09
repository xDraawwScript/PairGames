#include "tools.h"

#define HEIGHT 13 // hauteur des cartes
#define WIDTH 13 // largeur des cartes
#define NUM_CARDS 12 // nombre de cartes

char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'A', 'B', 'C', 'D', 'E', 'F'}; // tableau des lettres
int num_letters = sizeof(letters) / sizeof(letters[0]); // Nombre elt dans le tableau
int i;
int nombre_pairs_trouvees;

/*
Fonction : check_terminal_dimensions
Param : min_required_height - hauteur minimale requise
        min_required_width - largeur minimale requise
Traitement : Vérifie si les dimensions du terminal sont suffisantes pour le jeu.
              Affiche un message d'erreur si le terminal est trop petit.
Retour : 0 si les dimensions sont suffisantes, -1 si le terminal est trop petit,
         -2 en cas d'erreur lors de l'initialisation de NCurses.
*/

int check_terminal_dimensions(int min_required_height, int min_required_width) {
    noecho(); // Désactive l'affichage de l'entrée de l'utilisateur
    initscr(); // Initialisation de ncurses
    if (initscr() == NULL) {
        // Erreur lors de l'initialisation de NCurses
        return -2;
    }
    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width); // recuperer taille terminal

    if (terminal_height < min_required_height || terminal_width < min_required_width) { // si la taille est inférieure a la taille requise
        // Gestion de l'erreur
        clear();
        mvprintw(0, 0, "Erreur terminal trop petit. Le jeu necessite au moins %d lignes et %d colonnes.", min_required_height, min_required_width);
        refresh();
        getch();
        endwin();
        return -1;
    }

    // Si le terminal a des dimensions suffisantes, retourne 0 pour indiquer que le jeu peut continuer
    return 0;
}

/*
Fonction : dessinCarte
Param : row - position en ligne de la carte
        col - position en colonne de la carte
        valeur - valeur de la carte à afficher
        selectionne - indicateur si la carte est sélectionnée (1) ou non (0)
        verte - indicateur si la carte est de couleur verte (1) ou non (0)
        decouvert - indicateur si la carte est découverte (1) ou non (0)
Traitement : Affiche une carte dans la fenêtre du terminal en fonction de ses caractéristiques.
Retour : Aucun retour, la fonction gère l'affichage directement dans la fenêtre.
*/

void dessinCarte(int row, int col, int valeur, int selectionne, int verte, int decouvert){

    WINDOW *card_win = newwin(WIDTH, HEIGHT, row, col); // création fenêtre de la carte
    if (decouvert == 1){ // si la carte est déjà découverte
    wattron(card_win, COLOR_PAIR(4)); // couleur carte trouvée
            mvwprintw(card_win, WIDTH/2, HEIGHT/2, "%c", valeur); // on affiche la valeur de la carte au centre d'elle même
            wborder(card_win, '|', '|', '-', '-', '+', '+', '+', '+'); // on crée sa bordure
            wattroff(card_win, COLOR_PAIR(4)); // desactiver la couleur

    }else if (selectionne == 0) { // dans le cas ou elle n'est pas retournée
        if (verte == 0) { // et qu'elle n'est également pas selectionée
            mvwprintw(card_win, WIDTH/2, HEIGHT/2, "X"); // affichage d'une carte simple
            wborder(card_win, '|', '|', '-', '-', '+', '+', '+', '+');

        } else { // si elle est selectionée
            wattron(card_win, COLOR_PAIR(3)); // on crée la carte avec une couleur verte
            mvwprintw(card_win, WIDTH/2, HEIGHT/2, "X");
            wborder(card_win, '|', '|', '-', '-', '+', '+', '+', '+');
            wattroff(card_win, COLOR_PAIR(3));
        }


    } else { // si elle est retournée
        wattron(card_win, COLOR_PAIR(2)); // couleur bleue pour dire qu'elle est selectionée
        mvwprintw(card_win, WIDTH/2, HEIGHT/2, "%c", valeur);
        wborder(card_win, '|', '|', '-', '-', '+', '+', '+', '+');
        wattroff(card_win, COLOR_PAIR(2));
    }



    wrefresh(card_win);
    delwin(card_win);
}

/*
Fonction : dessinCarteBis
Param : cartes - tableau de pointeurs vers des structures Cartes
        startX - position de départ en colonne pour l'affichage des cartes
        index - indice de la carte à dessiner dans le tableau
Traitement : Affiche une carte à la position spécifiée en fonction de ses caractéristiques.
Retour : Aucun retour, la fonction utilise la fonction dessinCarte pour gérer l'affichage.
*/

void dessinCarteBis(struct Carte *cartes[],int startX,int index){
    if (index < 6){
    dessinCarte(10, startX -2 + index * espace, cartes[index]->valeur,cartes[index]->selectionne, cartes[index]->verte, cartes[index]->decouvert);

    }else{
    dessinCarte(25, startX - 2 + (index - 6) * espace, cartes[index]->valeur, cartes[index]->selectionne, cartes[index]->verte, cartes[index]->decouvert);
    }
}

/*
Fonction : handleKeyPress
Param : cartes - tableau de pointeurs vers des structures Carte
        sleep_ptr - pointeur vers une variable indiquant si la pause est activée
        startX - position de départ en colonne pour l'affichage des cartes
        num_cards - nombre total de cartes
        index_ptr - pointeur vers la variable d'indice actuel
        nb_card_lock_ptr - pointeur vers la variable indiquant le nombre de cartes retournées
        index_value_lock_ptr - pointeur vers la variable indiquant l'indice de la première carte retournée
        fin - pointeur vers la variable indiquant si le jeu est terminé
Traitement : Gère les appuis sur les touches du clavier pour interagir avec les cartes.
Retour : Aucun retour, les modifications sont effectuées directement sur les structures de cartes et les variables pointées.
*/

void handleKeyPress(struct Carte *cartes[],int *sleep_ptr,int startX, int num_cards, int *index_ptr, int *nb_card_lock_ptr, int *index_value_lock_ptr, int *fin) { // fonction pour la gestion d'appui de touche
    int ch;

    if (nombre_pairs_trouvees==6){
        *fin=1;
    }

    else{

    int index_save=*index_ptr;

    if (*nb_card_lock_ptr == 2){ // Tout d'abord si deux cartes sont retournées ( donc ont été selectionées )
       *sleep_ptr=1;
        sleep(1);
        flushinp(); // On désactive la possibilité d'appuyer sur des touches entre temps
        *sleep_ptr=0; // On attend deux secondes
        // on désactive la possibilité d'appuyer sur des touches entre temps

        if (cartes[*index_value_lock_ptr]->valeur == cartes[*index_ptr]->valeur){ // Si la valeur des deux cartes est équivalente
            cartes[*index_value_lock_ptr]->decouvert = 1; // L'attribut découvert de la carte précédamment vérouillée passe à 1
            cartes[*index_ptr]->decouvert = 1; // L'attribut de la carte actuelle passe à 1
            nombre_pairs_trouvees++;

            dessinCarteBis(cartes,startX,*index_ptr);

            for(int i = 0;i < 11;i++) { // Boucle pour sauter les cartes qui ont été découvertes
                if (cartes[*index_ptr]->decouvert == 1){

                   (*index_ptr)++; // Passer à la carte suivante
                    if (*index_ptr == num_cards) { // Si on est à la douxième carte, on passe à la première
                        *index_ptr = 0;
                    }
                }else{ // Si la carte actuelle n'est pas découverte la fonction se stoppe
                break;
                }
            }
        }else{ // Si la valeur des deux cartes n'est pas équivalente

        cartes[*index_value_lock_ptr]->selectionne = 0;
        cartes[*index_ptr]->selectionne = 0; // On met à l'endroit les deux cartes
        }
        *nb_card_lock_ptr = 0; // Le nombre de cartes retournées passe à 0
        cartes[*index_ptr]->verte = 1; // On selectionne la carte actuelle
        dessinCarteBis(cartes,startX,*index_value_lock_ptr);
        dessinCarteBis(cartes,startX,*index_ptr);

    }else { // Si deux cartes ne sont pas selectionées

        ch = getch(); // Entrée utilisateur

        if(ch=='q'||ch=='Q'){
            *fin=-1;

        }
        if (ch == 'a' || ch == 'A') { // Si on appuie sur 'a' ( déplacement à gauche )
            cartes[*index_ptr]->verte = 0; // On déselectionne la carte actuelle
            (*index_ptr)--; // On passe à la carte précédente
            if (*index_ptr == -1) { // Si on était à la premiere carte
                *index_ptr = num_cards - 1; // On passe à la dernière
            }
            while (cartes[*index_ptr]->decouvert == 1 || cartes[*index_ptr]->selectionne == 1){ // Sinon tant que l'on ne sommes pas sur une carte lambda
               (*index_ptr)--; // On passe à la carte précédente
            if (*index_ptr == -1) {
                *index_ptr = num_cards - 1;
            }
            }

            cartes[*index_ptr]->verte = 1; // Enfin on selectionne la carte actuelle

        } else if (ch == 'z' || ch == 'Z') { // Si on appuie sur 'z' déplacement à droite
            cartes[*index_ptr]->verte = 0; // On deseletionne la carte actuelle
            (*index_ptr)++; // On passe à la carte suivante
            if (*index_ptr == num_cards) { // Retour à la première carte si on est après la douxième
                *index_ptr = 0;
            }
            while (cartes[*index_ptr]->decouvert == 1 || cartes[*index_ptr]->selectionne == 1){ // Tant qu'on a pas une carte lambda
               (*index_ptr)++; // Carte suivante
            if (*index_ptr == num_cards) {
                *index_ptr = 0;
            }
            }
            cartes[*index_ptr]->verte = 1; // On selectionne la carte libre

        } else if (ch == 'e' || ch == 'E') { // Si on appuie sur 'e' ( retourner la carte actuelle )
            if (*nb_card_lock_ptr == 0){ // Si aucune carte n'est déjà retournée
            cartes[*index_ptr]->selectionne = 1; // On retourne la carte actuelle
            *nb_card_lock_ptr = 1; // Le nombre de cartes retournées passe à 1
            *index_value_lock_ptr = *index_ptr; // On inscrit le numéro de la carte retournée

            } else if(*index_ptr != *index_value_lock_ptr){ // En cas de bug ( carte déja retournée )
            cartes[*index_ptr]->selectionne = 1;
            *nb_card_lock_ptr = 2;
            }
        }
        dessinCarteBis(cartes,startX,index_save);
        dessinCarteBis(cartes,startX,*index_ptr);
        }

    }
}

/*
Fonction : recuperer_score
Param : scores - tableau de pointeurs vers des structures Score pour stocker les scores récupérés
        nom_fichier - chemin du fichier à partir duquel récupérer les scores
Traitement : Récupère jusqu'à trois scores à partir d'un fichier et les stocke dans le tableau de scores.
Retour : 1 en cas de succès, -3 en cas d'erreur lors de l'ouverture du fichier.
*/

int recuperer_score(struct Score *scores[], const char *nom_fichier){
    // Ouverture du fichier en mode lecture
    FILE *fichier = fopen(nom_fichier,"r");

        // Vérification si l'ouverture du fichier a réussi
        if (fichier == NULL) {
        return -3;
    }
    int i = 0;

    // Lecture des scores depuis le fichier jusqu'à la fin du fichier ou jusqu'à ce que 3 scores soient lus

    while( i < 3 && !feof(fichier)){
        // Lecture du pseudo et du temps depuis le fichier
        if(fscanf(fichier, "%s %f", scores[i]->pseudo, &scores[i]->temps) == 2){
            i++;
        }
    }
    fclose(fichier);
    return 1;
}

/*
Fonction : checkscore
Param : scores - tableau de pointeurs vers des structures Score représentant les scores
        temps - temps à vérifier par rapport aux scores existants
        nom_fichier - chemin du fichier où mettre à jour les scores en cas de réussite
Traitement : Vérifie si le temps passé est meilleur que l'un des trois scores existants.
             Si oui, ajoute le nouveau score et met à jour le fichier des scores.
Retour : Aucun retour, la fonction agit directement sur le tableau de scores et le fichier.
*/

void checkscore(struct Score *scores[],float temps,const char *nom_fichier){


    int index = -1;

    // Parcours des scores actuels
    for (int i = 0; i < 3; i++) {
        // Vérification si le temps actuel est plus rapide ou si le score actuel est vide
        if (scores[i]->temps == 0 || scores[i]->temps >= temps) {
            index = i;
            break;
        }
    }
    // Si un index valide est trouvé, ajouter le score
    if(index != -1){
        ajouterScore(scores,temps,index,nom_fichier);
    }
}

/*
Fonction : ajouterScore
Param : scores - tableau de pointeurs vers des structures Score représentant les scores
        temps - temps du nouveau score à ajouter
        index - indice où ajouter le nouveau score dans le tableau
        nom_fichier - chemin du fichier où mettre à jour les scores
Traitement : Ajoute un nouveau score au tableau, demande un pseudo à l'utilisateur,
             met à jour la fenêtre du score et le fichier des scores.
Retour : Aucun retour, la fonction agit directement sur le tableau de scores, la fenêtre et le fichier.
*/

void ajouterScore(struct Score *scores[],float temps,int index,const char *nom_fichier){

    echo();
    WINDOW *score;
    char nouveau_pseudo[5];

    // Création d'une fenêtre pour afficher le score
    score = newwin(10,99,40,0);
    wattron(score,COLOR_PAIR(2));
    box(score, 0, 0);
    wattroff(score,COLOR_PAIR(2));
    mvwprintw(score, 2, 2, "VICTOIRE !");
    wrefresh(score);
    // Saisie du pseudo du joueur avec vérification
    do{
        mvwprintw(score, 5, 5, "votre pseudo : ");
        wrefresh(score);
        wgetnstr(score,nouveau_pseudo,sizeof(nouveau_pseudo)-1);
        nouveau_pseudo[sizeof(nouveau_pseudo)-1]='\0';
        if(strlen(nouveau_pseudo)==0){

            wrefresh(score);
            usleep(2);
            werase(score);
            wattron(score,COLOR_PAIR(2));
            box(score,0,0);
            wattroff(score,COLOR_PAIR(2));
            mvwprintw(score, 4, 2, "le pseudo ne peut pas être vide");
            wattroff(score,COLOR_PAIR(2));
            wrefresh(score);
        }
    }while (strlen(nouveau_pseudo)==0);
    // Décalage des scores existants s'il y a déjà un score à cet index
    if (scores[index]->temps!=0){
        for(i=2;i>index;i--){
            strcpy(scores[i]->pseudo,scores[i-1]->pseudo);
            scores[i]->temps=scores[i-1]->temps;
        }
    }
    // Attribution du nouveau pseudo et temps au score
    strcpy(scores[index]->pseudo,nouveau_pseudo);
    scores[index]->temps=temps+0.1;

    // Effacement de la fenêtre du score
    werase(score);
    wattron(score,COLOR_PAIR(2));
    box(score,0,0);
    wattroff(score,COLOR_PAIR(2));
    wrefresh(score);

    FILE *fichier = fopen(nom_fichier,"w");

    // Écriture des scores dans le fichier
    for(int i=0;i<3;i++){

        if(scores[i]->temps!=0){
            fprintf(fichier,"%s %.1f\n",scores[i]->pseudo, scores[i]->temps);
        }
        else{
            break;
            }
    }
    fclose(fichier);

}

/*
Fonction : initialiserValeurCartes
Param : cartes - tableau de pointeurs vers des structures Carte pour stocker les cartes initialisées
Traitement : Alloue dynamiquement la mémoire pour chaque carte, mélange les cartes,
              et initialise les valeurs, états de sélection et de découverte.
Retour : Aucun retour, la fonction agit directement sur le tableau de cartes.
*/

void initialiserValeurCartes(struct Carte *cartes[]) {
        for (int i = 0; i < 12; ++i) {
        // Allouer dynamiquement une nouvelle carte
        cartes[i] = (struct Carte *)malloc(sizeof(struct Carte));

        // Vérifier si l'allocation a réussi
        if (cartes[i] == NULL) {
            // Gérer l'échec de l'allocation, en sortant de la fonction
            endwin();
            exit(EXIT_FAILURE);
        }
        }
        shuffleCards(*cartes);
        for(int i=0;i<6; i++){
        cartes[i]->valeur =letters[i];
        cartes[i]->selectionne = 0;  // Non sélectionné au départ
        cartes[i]->decouvert = 0;   // Non découvert au départ
        cartes[i]->verte=0;
    }
        for(i=6;i<12;i++){
        cartes[i]->valeur = letters[i];
        cartes[i]->selectionne = 0;  // Non sélectionné au départ
        cartes[i]->decouvert = 0;   // Non découvert au départ
        cartes[i]->verte=0;
        }
}

/*
Fonction : shuffleCards
Param : cartes - tableau de structures Carte à mélanger (lettre de chaque carte)
Traitement : Mélange aléatoirement les lettres des cartes dans le tableau.
Retour : Aucun retour, la fonction agit directement sur le tableau de lettres.
*/

void shuffleCards(Carte *cartes) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires avec une graine (seed)

    for (int i = 11; i > 0; i--) {
        int j = rand() % (i + 1); // Générer un index aléatoire entre 0 et i

        // Échanger les cartes à l'index i et j dans le tableau
        char temp = letters[i];
        letters[i] = letters[j];
        letters[j] = temp;
    }
}

/*
Fonction : handleKeyPressAuto
Param : cartes - tableau de pointeurs vers des structures Carte représentant les cartes du jeu
        sleep_ptr - pointeur vers une variable indiquant l'état de pause du jeu
        startX - position de départ en X pour le dessin des cartes
        num_cards - nombre total de cartes dans le jeu
        index_ptr - pointeur vers la variable représentant l'index de la carte sélectionnée
        nb_card_lock_ptr - pointeur vers la variable indiquant le nombre de cartes sélectionnées
        index_value_lock_ptr - pointeur vers la variable contenant l'index de la carte verrouillée
        fin - pointeur vers la variable indiquant si le jeu est terminé
Traitement : Gère les appuis de touche automatiquement pour le jeu (dans le mode automatique).
              Réalise les actions en fonction des appuis de touche et de l'état du jeu.
Retour : Aucun retour, la fonction agit directement sur les variables pointées.
*/

void handleKeyPressAuto(struct Carte *cartes[],int *sleep_ptr,int startX, int num_cards, int *index_ptr, int *nb_card_lock_ptr, int *index_value_lock_ptr, int *fin) {

    int ch ;
    int index_save=*index_ptr;

    if (nombre_pairs_trouvees==6){
        *fin=1;
    }

    else{


    if (*nb_card_lock_ptr == 2){ // tout d'abord si deux cartes sont retournées ( donc ont été selectionées )
        *sleep_ptr=1;
        sleep(1);
        flushinp(); // on désactive la possibilité d'appuyer sur des touches entre temps
        *sleep_ptr=0; // on attend deux secondes
        if (cartes[*index_value_lock_ptr]->valeur == cartes[*index_ptr]->valeur){ // si la valeur des deux cartes est équivalente
            cartes[*index_value_lock_ptr]->decouvert = 1; // l'attribut découvert de la carte précédamment vérouillée passe à 1
            cartes[*index_ptr]->decouvert = 1; // l'attribut de la carte actuelle passe à 1
            nombre_pairs_trouvees++;
            dessinCarteBis(cartes,startX,*index_ptr);

            for(int i = 0;i < 11;i++) { // boucle pour sauter les cartes qui ont été découvertes
                if (cartes[*index_ptr]->decouvert == 1){

                   (*index_ptr)++; // passer à la carte suivante
                    if (*index_ptr == num_cards) { // si on est à la douxième carte, on passe à la première
                        *index_ptr = 0;
                    }
                }else{ // si la carte actuelle n'est pas découverte la fonction se stoppe
                break;
                }
            }
        }else{ // si la valeur des deux cartes n'est pas équivalente

        cartes[*index_value_lock_ptr]->selectionne = 0;
        cartes[*index_ptr]->selectionne = 0; // on met à l'endroit les deux cartes
        }
        *nb_card_lock_ptr = 0; // le nombre de cartes retournées passe à 0
        cartes[*index_ptr]->verte = 1; // on selectionne la carte actuelle
        dessinCarteBis(cartes,startX,*index_value_lock_ptr);
        dessinCarteBis(cartes,startX,*index_ptr);

    }else { // si deux cartes ne sont pas selectionées
        *sleep_ptr=1;
        sleep(1);
        flushinp(); // on désactive la possibilité d'appuyer sur des touches entre temps
        *sleep_ptr=0; // on attend deux secondes
        ch = getRandomInput(); // entrée utilisateur

        if(ch=='q'||ch=='Q'){
            *fin=-1;

        }
        if (ch == 'a' || ch == 'A') { // si on appuie sur 'a' ( déplacement à gauche )
            cartes[*index_ptr]->verte = 0; // on déselectionne la carte actuelle
            (*index_ptr)--; // on passe à la carte précédente
            if (*index_ptr == -1) { // si on était à la premiere carte
                *index_ptr = num_cards - 1; // on passe à la dernière
            }
            while (cartes[*index_ptr]->decouvert == 1 || cartes[*index_ptr]->selectionne == 1){ // sinon tant que l'on ne sommes pas sur une carte lambda
               (*index_ptr)--; // on passe à la carte précédente
            if (*index_ptr == -1) {
                *index_ptr = num_cards - 1;
            }
            }

            cartes[*index_ptr]->verte = 1; // enfin on selectionne la carte actuelle

        } else if (ch == 'z' || ch == 'Z') { // si on appuie sur 'z' déplacement à droite
            cartes[*index_ptr]->verte = 0; // on deseletionne la carte actuelle
            (*index_ptr)++; // on passe à la carte suivante
            if (*index_ptr == num_cards) { // retour à la première carte si on est après la douxième
                *index_ptr = 0;
            }
            while (cartes[*index_ptr]->decouvert == 1 || cartes[*index_ptr]->selectionne == 1){ // tant qu'on a pas une carte lambda
               (*index_ptr)++; // carte suivante
            if (*index_ptr == num_cards) {
                *index_ptr = 0;
            }
            }
            cartes[*index_ptr]->verte = 1; // on selectionne la carte libre

        } else if (ch == 'e' || ch == 'E') { // si on appuie sur 'e' ( retourner la carte actuelle )
            if (*nb_card_lock_ptr == 0){ // si aucune carte n'est déjà retournée
            cartes[*index_ptr]->selectionne = 1; // on retourne la carte actuelle
            *nb_card_lock_ptr = 1; // le nombre de cartes retournées passe à 1
            *index_value_lock_ptr = *index_ptr; // on inscrit le numéro de la carte retournée

            } else if(*index_ptr != *index_value_lock_ptr){ // en cas de bug ( carte déja retournée )
            cartes[*index_ptr]->selectionne = 1;
            *nb_card_lock_ptr = 2;
            }
        }
        dessinCarteBis(cartes,startX,index_save);
        dessinCarteBis(cartes,startX,*index_ptr);
        }

    }
}

/*
Fonction : getRandomInput
Param : Aucun paramètre
Traitement : Génère un caractère d'entrée utilisateur aléatoire parmi 'e', 'a' ou 'z'.
Retour : Caractère d'entrée utilisateur généré aléatoirement.
*/

int getRandomInput() {
    int randomValue = rand() % 3;  // Génère un nombre entre 0 et 2
    char userInput;

    switch (randomValue) { // gestion des cas
        case 0: // si par exemple randomValue==0
            userInput = 'e'; // on simule 'e'
            break;
        case 1:
            userInput = 'a';
            break;
        case 2:
            userInput = 'z';
            break;
    }

    return userInput;
}
