#include "tools.h"

/*
Fonction : chrono_function
Param : arg - pointeur vers la structure ThreadArgs contenant les arguments du thread
Traitement : Fonction exécutée par le thread de chronométrage, met à jour le chronomètre toutes les 0,1 seconde.
Retour : Aucun (void *)
*/

void *chrono_function(void *arg) {
    // Récupération des arguments
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    while (*(args->thread_running) == 1) {

        usleep(100000); // Pause de 0,1 seconde entre chaque mise à jour

        *(args->chronoscore) += 0.1; // Mise à jour du chrono
        // Verrouillage du mutex pour éviter les conflits d'accès aux ressources partagées

        pthread_mutex_lock(&args->mutex);
        mvprintw(1,86,"%.1f",*(args->chronoscore));

        // Rafraîchissement de l'écran si l'indicateur de pause est activé

        if (*(args->sleep)==1){
            refresh();

        }
        // Déverrouillage du mutex après l'accès aux ressources partagées
        pthread_mutex_unlock(&args->mutex);
    }
    // Sortie propre du mutex
    pthread_exit(NULL);
    return NULL;
}





