#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
/**
 * @brief Cette structure contiendra les informations reçues par la Raspberry via la communication Serial
 * 
 */
typedef struct vector
{
    unsigned int array[12];
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

#endif // IMAGE_PROCESS_H
