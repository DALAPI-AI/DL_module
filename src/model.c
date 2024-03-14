#include <unistd.h>
#include "../utils/utils.h"

#define NB_ACTIONS 4
#define R_LOOP 1
#define R_HIT -10
#define R_WIN 90
// supposition 4 * 4
#define NB_STATES 16 


int actions[NB_ACTIONS] = {
    0b00, // STRAIGHT
    0b01, // BACK 
    0b10, // RIGHT / est 
    0b11  // LEFT / ouest
};

float Q [NB_STATES][NB_ACTIONS] = {0};

float g = 0.95;

 // TODO : redefinir la stratégie de selon le modèle choisi 
int R[NB_STATES][NB_ACTIONS][NB_STATES]={
    
};

float max(float *tab, int nbElt){
    float max = tab[0];
    for(int i = 1; i < nbElt; i++){
        if(max < tab[i]){
            max = tab[i];
        }
    }
    return max;
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    float learn = 0.1;
    float exploration = 0.9;
    int ***T = allocMemoireT(NB_STATES, NB_ACTIONS);
    for(int i = 0; i < 100; i++){    
        for(int s = 0; s < NB_STATES; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                float sum = 0;
                for(int s = 0; s < NB_STATES; s++){
                    sum += T[s][a][s] * (R[s][a][s] + g * max(Q[s], NB_ACTIONS));
                }
                Q[s][a] = sum;
            }
        }

        printf("Iteration: %d\n", i);
        for(int s = 0; s < NB_STATES; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                printf("Q[State:%d, Action:%d]=%+10.4f", s, a, Q[s][a]);
                if( Q[s][a] == max(Q[s], NB_ACTIONS)){
                    printf(" <- ");
                }
                printf("\n");
            }
        }

    } 
    printf("---------------------------------\n");  
    return 0;
}











