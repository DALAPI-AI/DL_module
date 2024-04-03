#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include "utils/utils.h"

#define DIM_VECT 12 

/**
 * @brief Cette structure contiendra les informations reçues par la Raspberry via la communication Serial
 * {N1,...,N8,E,S,O,B}
 */
typedef struct vector
{
    unsigned int array[DIM_VECT];
} VecteurImg;

/// @brief Retourne le tableau de valeur de la direction NORD
unsigned int* getNorth(VecteurImg vect);
/// @brief Retourne la valeur de la direction SUD
unsigned int getSouth(VecteurImg vect);
/// @brief Retourne la valeur de la direction OUEST
unsigned int getWest(VecteurImg vect);
/// @brief Retourne la valeur de la direction EST
unsigned int getEast(VecteurImg vect);
/// @brief Retourne la valeur du boussole
unsigned int getBoussole(VecteurImg vect);

float calculateNorthMean(VecteurImg vectImg);
void normalizeVec(VecteurImg vect);
float calculateSimilarity(VecteurImg vec1, VecteurImg vec2);

int *getIndexes(VecteurImg vect);
int getIndex(int distance);
void translateVect(VecteurImg vect, int* indexes);
unsigned int* randomNorthDistance(int nbCel);
int getState(VecteurImg vect, Room room );
VecteurImg generateRandomVector(Room room);
void afficherVect(VecteurImg vect);

#endif // IMAGE_PROCESS_H
