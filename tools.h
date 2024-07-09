#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

#define espace 15

extern int mode;

struct ThreadArgs {
    float *chronoscore;
    int *thread_running;
    int *sleep;
    pthread_mutex_t mutex;
};

typedef struct Score{
    char pseudo[5];
    float temps;
} Score;

typedef struct Carte{

int valeur;
int selectionne;
int decouvert;
int verte;
} Carte;


int automode();
int menu();
int jeu();
void ajouterScore(struct Score *scores[],float temps,int index,const char *nom_fichier);
void ecrirescore(struct Score *scores[],const char *nom_fichier);
void checkscore(struct Score *scores[],float temps,const char *nom_fichier);
int recuperer_score(struct Score *scores[], const char *nom_fichier);

void enregistrerScore(float chronoscore);
int check_terminal_dimensions(int min_required_height, int min_required_width);

void handleKeyPress(struct Carte *cartes[],int *sleep_ptr,int startX, int num_cards, int *index_ptr, int *nb_card_lock_ptr, int *index_value_lock_ptr, int *fin);
void handleKeyPressAuto(struct Carte *cartes[],int *sleep_ptr,int startX, int num_cards, int *index_ptr, int *nb_card_lock_ptr, int *index_value_lock_ptr, int *fin);
void dessinCarteBis(struct Carte *cartes[],int startX,int index);

void shuffleCards(struct Carte *cartes);
void *chrono_function(void *arg);
void dessinCarte(int row, int col, int valeur, int selectionne, int verte, int decouvert);
void initialiserValeurCartes(struct Carte *cartes[]);
int getRandomInput();
extern int current_card,max_x,max_y;
void cartesSelectionne(int);




#endif // TOOLS_H_INCLUDED

