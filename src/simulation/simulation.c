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

int main(int argc, char const *argv[])
{
    // ajouter fichier de conf 
    int height = 180;
    int width = 180;
    int epoch = 100;
    Room room = allocMemoireRoom(height, width);

    int finalstate = rand()%room.nbStats;

    float **Q = learningModel(height, width, finalstate, epoch, g);

    printf("#######################################################################\n");
    printf("\t\t\t Representation de l'environnement \n");
    printf("#######################################################################\n");
    
    for(int lig = 0; lig < room.nbLig; lig++){
        printf("\t |");
        for(int col = 0; col < room.nbCol; col++){
            int state = lig*room.nbCol + col; 
            int action = indexMax(Q[state], NB_ACTIONS);
            if(state == finalstate){
                printf(" F |");
            }else{
                printf("%s|",signAction(action));
            }
            
        }
        printf("\n");
    }

    printf("#######################################################################\n");

    return 0;
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