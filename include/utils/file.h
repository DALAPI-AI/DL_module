#ifndef FILE_H
#define FILE_H
#include "process/imageProcessing.h"


/// @brief Efface et sauvegarde le tableau de vecteur qui se trouve dans fileName
int saveVector(VecteurImg* vect,int nbVect, char* fileName);
/// @brief Charge les vecteurs dans le tableau (attention ne vérifie pas la taille)
int loadVector(VecteurImg* vect,int* nbVect ,char* fileName);
/// @brief Ajoute un vecteur à la suite des vecteurs déja existant
int appendVector(VecteurImg vect, char* fileName);
/// @brief Supprime le vecteur à l'index Index dans le fichier
int deleteVector(int index,char* fileName);

#endif // FILE_H
