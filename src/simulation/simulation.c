#include "utils/utils.h"
#include "process/imageProcessing.h"
#include <stdio.h>

int actions[NB_ACTIONS] = {
    0b00, // STRAIGHT
    0b01, // BACK 
    0b10, // RIGHT / est 
    0b11  // LEFT / ouest
};
float g = 0.95;

char *signAction(int action);
int getAction(float **Q, int state);
void environnementState(Room room, int depart, int final, int height, int width, int boussole);
void freeMatrice(float ***Q, int nbLig, int nbCol);
int fromParcours(int state, int* steps, int nbsteps);
char getBoussoleSignification(int bs);


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    // ajouter fichier de conf 
    int height = 180;
    int width = 180;

    int epoch = 100;
    Room room = allocMemoireRoom(height, width);

    int finalstate = rand()%room.nbStats;

    float **Q = learningModel(height, width, finalstate, epoch, g);

    printf("#######################################################################\n");
    printf("\t\t\t Representation de l'environnement \n");
    printf("#######################################################################\n\n");
    
    VecteurImg state = generateRandomVector(room);
    // éviter le cas d'etre directement sur la case finale 
    int d_state = getState(state, room);
    while(d_state ==  finalstate){
        state = generateRandomVector(room);
        d_state = getState(state, room);
    }
    environnementState(room, d_state, finalstate, height, width, getBoussole(state));

    for(int lig = 0; lig < room.nbLig; lig++){
        printf("\t\t\t |");
        for(int col = 0; col < room.nbCol; col++){
            int state = lig*room.nbCol + col; 
            int action = indexMax(Q[state], NB_ACTIONS);
            if(state == finalstate){
                printf(" F |");
            }
            else{
                if(state == d_state){
                    printf(" D |");
                }else{
                    printf("%s|",signAction(action));
                }
                
            }
            
        }
        printf("\n");
    }

    printf("\n#######################################################################\n");

    
    int cur_state = d_state;
    printf("Depart : %d  ===============> Fin : %d \n", d_state, finalstate);
    int *steps = (int*)malloc(room.nbStats *sizeof(int));
    int nbSteps = 0;

    while(cur_state != finalstate){
        int action = getAction(Q, cur_state);
        generateNextState(&state, cur_state, action, room);
        int next_state = getState(state, room);
        printf("Etat: %d x Action: %d ==> etat suivant: %d \n",cur_state, action, next_state);
        cur_state = next_state;
        steps[nbSteps] = next_state;
        nbSteps ++;
    }

    printf("=================== Parcours ======================\n\n");

    for(int lig = 0; lig < room.nbLig; lig++){
        printf("\t |");
        for(int col = 0; col < room.nbCol; col++){
            int state = lig*room.nbCol + col; 
            int action = indexMax(Q[state], NB_ACTIONS);
            if(state == finalstate){
                printf(" F |");
            }
            else{
                if(state == d_state){
                    printf(" D |");
                }else{
                    if(fromParcours(state, steps, nbSteps)){
                        printf("%s|",signAction(action));
                    }else{
                        printf("   |");
                    }
                    
                }
                
            }
            
        }
        printf("\n");
    }
    printf("========================================================\n\n");

    freeMatrice(&Q, room.nbStats, NB_ACTIONS);
    free(steps);
    return 0;
}

int fromParcours(int state, int* steps, int nbsteps){
    int i = 0;
    while(i < nbsteps && state != steps[i]){
        i++;
    }
    return i < nbsteps;
}

void freeMatrice(float ***Q, int nbLig, int nbCol){
    for(int i = 0; i < nbLig; i++){
        free(Q[i]);
        Q[i] = NULL;
    }
    free(Q);
    Q = NULL;
}

int getAction(float **Q, int state){
    return indexMax(Q[state], NB_ACTIONS);
}

char *signAction(int action){
    switch (action)
    {
    case 0:
        return " ^ ";
    case 1:
        return " v ";
    case 2:
        return " > ";
    case 3:
        return " < ";
    
    default:
        return "";
    }
}

void environnementState(Room room, int depart, int final, int height, int width, int boussole){
    printf("Environnement : %d x %d \n",height, width);
    printf("Matrice : %d x %d  |     Etat de depart : %d    |   Orientation boussole : %c  |      Etat final : %d \n\n",
    room.nbLig, room.nbCol,depart, getBoussoleSignification(boussole), final);
}

char getBoussoleSignification(int bs){
    switch (bs)
    {
    case 0:
        return 'N';
    case 1:
        return 'S';
    case 2:
        return 'E';
    case 3:
        return 'W';
    
    default:
        return ' ';
    }
}