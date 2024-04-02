#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "process/imageProcessing.h"
#define AMOUNT_NORTH 8
#define PI (4 * atan(1))   

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

static float getOppose(float theta,float hypotenuse){
    return hypotenuse * sin(theta);
}

float calculateNorthMean(VecteurImg vectImg){
    unsigned int* northArray = (unsigned int*)malloc(AMOUNT_NORTH*sizeof(unsigned int));
    
    northArray = getNorth(vectImg);
    for (int i = 0; i < AMOUNT_NORTH; i++)
    {
        printf("int %d\n",northArray[i]);
    }
    float mean = 0;
    float oppose = 0;
    float totalAngleRange = (5 * PI / 6) - (PI / 6);
    float step = totalAngleRange / (AMOUNT_NORTH - 1);
    float startAngle = PI/6; // 30°
    float currentA;
    for (int i = 0; i < AMOUNT_NORTH; i++) {
        currentA = startAngle + (i * step);
        oppose = getOppose(currentA, northArray[i]);
        mean += oppose;
    }
    mean /= AMOUNT_NORTH;
    return mean;
}

void normalizeVec(VecteurImg vect){

}

float calculateSimilarity(VecteurImg vec1, VecteurImg vec2){
    return 0.0f;
}