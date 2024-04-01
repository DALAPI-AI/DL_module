#ifndef TOOLS
#define TOOLS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define DIM_CELL 30 // in centimeters 
#define DIM_VECT 19 // 16(N) + 1(S) + 1(E) + 1(W) 
#define MAIN_PROBALITY 0.50f
#define EDGE_PROBALITY 0.25f


#define R_HIT -5
#define R_WIN 20

typedef struct{
    int nbState;
    int wall;
}State;

typedef struct {
    int nbCol;
    int nbLig;
    int nbStats;
    State **mat; // contenu de la case (numéro de la case)
}Room;

typedef struct {
    // à voir avec les données dont on aurait besoin 
    int vect[DIM_VECT];
}Vecteur;


// functions 
Room allocMemoireRoom(int height, int width);
int randomPosition(Room room);
float ***allocMemoire3DimTab(int nbStates, int nbActions);
void affichageT(float ***T, Room room, int nbactions);
int closerToSuccState(int state, int fstate, int succState, Room room);
void generateTr(int nbActions, float ***T, Room room, int finalState);
float **allocMemoireQ(int nbActions, int nbStates);
void generateR(int nbActions, float ***R, Room room, int succState);

#endif