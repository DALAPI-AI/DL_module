#include <stdlib.h>
#include <stdio.h>
#include "process/imageProcessing.h"


unsigned int* getNorth(VecteurImg vect){
    unsigned int* northTab = (unsigned int*) malloc(8 * sizeof(unsigned int));
    if (northTab == NULL){
        printf("Erreur allocation tableau de Nord");
    }
    for (int i = 0; i < 8; i++)
    {
        northTab[i] = vect.array[i];
    }
    
    return northTab;
}
unsigned int getSouth(VecteurImg vect){
    return vect.array[8];
}
unsigned int getWest(VecteurImg vect){
    return vect.array[9];
}
unsigned int getEast(VecteurImg vect){
    return vect.array[10];
}
unsigned int getBoussole(VecteurImg vect){
    return vect.array[11];
}
