#ifndef TOOLS
#define TOOLS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DIM_CELL 30 // in centimeters 
#define DIM_VECT 34 // 16(N) + 16(S) + 1(E) + 1(W) 


typedef struct {
    int nbCol;
    int nbLig;
    int **mat; // contenu de la case (numéro de la case)
}Room;

typedef struct {
    // à voir avec les données dont on aurait besoin 
    int vect[DIM_VECT];
}Vecteur;


// functions 
Room allocMemoireRoom(int height, int width);
int randomPosition(Room room);
float ***allocMemoireT(int nbStates, int nbActions);
void affichageT(float ***T, int nbstates, int nbactions);
void generateTr(int nbState, int nbActions, float ***T, Room room);


#endif