#include <unistd.h>
#include "utils/utils.h"
#include "utils/file.h"
#include "process/imageProcessing.h"

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

    int height = 180;
    int width = 180;
    int finalState = 18;
    Room room = allocMemoireRoom(height, width);

    float ***T = allocMemoire3DimTab(room.nbStats, NB_ACTIONS);
    float ***R = allocMemoire3DimTab(room.nbStats, NB_ACTIONS);
    float **Q = allocMemoireQ(NB_ACTIONS, room.nbStats);

    generateTr(NB_ACTIONS, T, room, finalState);
    // affichageT(T, room, NB_ACTIONS);
    
    generateR(NB_ACTIONS, R, room, finalState);
    // affichageT(R, room, NB_ACTIONS);

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
        
        if(i%10)continue;
        /*printf("Iteration: %d\n", i);
        for(int s = 0; s < room.nbStats; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                printf("Q[State:%d, Action:%d]=%+10.4f", s, a, Q[s][a]);
                if( Q[s][a] == max(Q[s], NB_ACTIONS)){
                    printf(" <- ");
                }
                printf("\n");
            }
        }*/
        

    } 
    
    printf("_____TEST RESULT _____\n");
    for(int s = 0; s < room.nbStats; s++){
        if ( s== finalState){
            printf("F ,");
        }else{
            printf("%d ,",indexMax(Q[s], NB_ACTIONS));

        }
        if((s+1)%room.nbCol == 0){
            printf("\n");
        }
    }
    printf("------------------------\n");
    VecteurImg sampleVectors[] = {
        {{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 1}},
        {{130, 140, 150, 160, 200, 180, 130, 110, 10, 20, 30, 40}},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}
    };
    int nbElt = 3;
    printf("#######################################################################\n");
    printf("\t\t Representation de vecteur \n");
    printf("#######################################################################\n");

    VecteurImg vect = generateRandomVector(room);

    int st = getState(vect, room);
    
    /*
    if (saveVector(sampleVectors,nbElt ,"vectors.txt") != 0) {
        printf("Error saving vector.\n");
        return 1;
    }

    printf("Affichage de tous les vecteurs:\n");
    VecteurImg* loadedVector = (VecteurImg*) malloc(nbElt*sizeof(VecteurImg));
    if (loadVector(loadedVector, &nbElt,"vectors.txt") != 0) {
        printf("Error loading vector.\n");
        return 1;
    }
    for (int i = 0; i < nbElt; i++) {
        for (int j = 0; j < 12; j++) {
            printf("%u ", loadedVector[i].array[j]);
        }
        printf("\n");
    }
    VecteurImg newVector = {{250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360}};
    printf("Ajout d'un vecteur :\n");
    if (appendVector(newVector, "vectors.txt") != 0) {
        printf("Error appending vector.\n");
        return 1;
    }

    printf("Suppression du vecteur numero 2 (1 en algo)\n");
    if (deleteVector(1, "vectors.txt") != 0) {
        printf("Error deleting vector.\n");
        return 1;
    }

    printf("Affichage des vecteurs apres suppression:\n");
    FILE *file = fopen("vectors.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);

    free(loadedVector);
    */
    return 0;
}

