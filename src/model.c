#include <unistd.h>
#include "../utils/utils.h"

#define NB_ACTIONS 4
#define R_LOOP 1



int actions[NB_ACTIONS] = {
    0b00, // STRAIGHT
    0b01, // BACK 
    0b10, // RIGHT / est 
    0b11  // LEFT / ouest
};
float g = 0.95;


float max(float *tab, int nbElt){
    float max = tab[0];
    for(int i = 1; i < nbElt; i++){
        if(max < tab[i]){
            max = tab[i];
        }
    }
    return max;
}

int indexMax(float *tab, int nbElt){
    int index = 0; 
    float max = tab[0];
    for(int i = 1; i < nbElt; i++){
        if(max < tab[i]){
            max = tab[i];
            index = i;
        }
    }
    return index;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    int height = 120;
    int width = 120;
    int finalState = 2*6;
    Room room = allocMemoireRoom(height, width);

    float ***T = allocMemoire3DimTab(room.nbStats, NB_ACTIONS);
    generateTr(NB_ACTIONS, T, room, finalState);
    affichageT(T, room, NB_ACTIONS);

    float **Q = allocMemoireQ(NB_ACTIONS, room.nbStats);

    float ***R = allocMemoire3DimTab(room.nbStats, NB_ACTIONS); 
    generateR(NB_ACTIONS, R, room, finalState);
    affichageT(R, room, NB_ACTIONS);

    for(int i = 0; i < 100; i++){    
        for(int s = 0; s < room.nbStats; s++){
            for(int a = 0; a < NB_ACTIONS; a++){
                float sum = 0;
                for(int sf = 0; sf < room.nbStats; sf++){
                    sum += T[s][a][sf] * (R[s][a][sf] + g * max(Q[sf], NB_ACTIONS));
                }
                Q[s][a] = sum;
            }
        }
        /*
        if(i%10)continue;
        printf("Iteration: %d\n", i);
        for(int s = 0; s < room.nbStats; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                printf("Q[State:%d, Action:%d]=%+10.4f", s, a, Q[s][a]);
                if( Q[s][a] == max(Q[s], NB_ACTIONS)){
                    printf(" <- ");
                }
                printf("\n");
            }
        }
        */

    } 
    printf("---------------------------------\n");  
    printf("-----------------Result------------------ \n");
    for(int s = 0; s < room.nbStats; s++){
        printf("%d ,",indexMax(Q[s], NB_ACTIONS));
        if((s+1)%room.nbCol == 0){
            printf("\n");
        }
    }
    printf("---------------------------------\n");
    
    
    return 0;
}











