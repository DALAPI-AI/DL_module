/**
 * @file imageProcessing.h
 * @author Dania OULKADI
 * @author Laurent LIN
 * @author Pierre FRANCK-PAPUCHON
 * @author Alicia BERROUANE
 * @brief Gère la reconnaissance d'image à partir d'un vecteur
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "process/imageProcessing.h"

#define AMOUNT_NORTH 8
#define NB_INDEXES 4
#define PI (4 * atan(1))
#define B_RANGE 4 
#define DECAL_MARG  DIM_CELL/3 // marge de décalage pour la génération de vecteur  

/**
 * @brief Extrait les distances du premier capteur (north)
 * @param vect vecteur position renvoyé par l'arduino
 * @return tableau de 8 entier correspondant aux distances du capteurs 
*/
unsigned int* getNorth(VecteurImg vect){
    unsigned int* northTab = (unsigned int*) malloc(8 * sizeof(unsigned int));
    if (northTab == NULL){
        printf("Erreur allocation tableau de Nord");
    }
    for (int i = 0; i < AMOUNT_NORTH ; i++)
    {
        northTab[i] = vect.array[i];
    }
    
    return northTab;
}
/**
 * @brief Extrait la distance du 3em capteur (sud)
 * @param vect vecteur position 
 * @return distance sud 
*/
unsigned int getSouth(VecteurImg vect){
    return vect.array[9];
}

/**
 * @brief Extrait la distance du 4em capteur (ouest)
 * @param vect vecteur position 
 * @return distance ouest 
*/
unsigned int getWest(VecteurImg vect){
    return vect.array[10];
}

/**
 * @brief Extrait la distance du 2em capteur (est)
 * @param vect vecteur position 
 * @return distance est 
*/
unsigned int getEast(VecteurImg vect){
    return vect.array[8];
}

/**
 * @brief Extrait la valeur de la boussole {0-4}
 * @param vect vecteur position 
 * @return orientation de la boussole  
*/
unsigned int getBoussole(VecteurImg vect){
    return vect.array[11];
}

/**
 * @brief Calcule l'opposé à partir de l'hypotenus et de l'angle 
 * @param theta angle de lecture
 * @return distance opposé 
*/
static float getOppose(float theta,float hypotenuse){
    return hypotenuse * sin(theta);
}

/**
 * @brief Calcule l'hypotenus à partir de l'opposé et de l'angle 
 * @param theta angle de lecture
 * @return distance hypotenus 
*/
static float getHypo(float theta, float opp){
    return opp / sin(theta);
}
/**
 * @brief génère une distance random à partir du nombre de cellule devant le capteur 
 * @param nbCel nombre de cellule devant le capteur 
*/
unsigned int randomDistance(int nbCel){
    return nbCel * DIM_CELL + rand()%DECAL_MARG;
}
/**
 * @brief génère 8 distances random à partir du nombre de cellule devant le capteur 1 (nord)
 * @param nbCel nombre de cellule devant le capteur  
*/
unsigned int* randomNorthDistance(int nbCel){
    unsigned int* northTab = (unsigned int*) malloc(AMOUNT_NORTH * sizeof(unsigned int));
    int oppose = nbCel * DIM_CELL + rand()%DECAL_MARG;

    float totalAngleRange = 2 * PI/3;
    float step = totalAngleRange / (AMOUNT_NORTH - 1);
    float startAngle = PI/6; // 30°
    float currentA;
    // calcul des 8 mesures du premier capteur + une erreur de 10cm 
    for (int i = 0; i < AMOUNT_NORTH; i++) {
        currentA = startAngle + (i * step);
        northTab[i] = getHypo(currentA, oppose) + rand()%10;
    }
    return northTab;
}


/**
 * @brief Calcule la moyenne des distances retourné par le capteur 1 (capteur nord)
 * @param vectImg vecteur de position retrouné par l'arduino 
 * @brief moyenne des distances 
*/
float calculateNorthMean(VecteurImg vectImg){
    unsigned int* northArray = (unsigned int*)malloc(AMOUNT_NORTH*sizeof(unsigned int));
    
    northArray = getNorth(vectImg);
    /*for (int i = 0; i < AMOUNT_NORTH; i++)
    {
        printf("int %d\n",northArray[i]);
    }*/
    float mean = 0;
    float oppose = 0;
    float totalAngleRange = 2 * PI/3;
    float step = totalAngleRange / (AMOUNT_NORTH - 1);
    float startAngle = PI/6; // 30°
    float currentA;
    for (int i = 0; i < AMOUNT_NORTH; i++) {
        currentA = startAngle + (i * step);
        oppose = getOppose(currentA, northArray[i]);
        mean += oppose;
    }
    mean /= AMOUNT_NORTH;
    printf("la valeur nord => %f\n ", mean);
    return mean;
}

/**
 * @brief Calcule les indices équivalents à un vecteur position 
 * @param vect vecteur position renvoyé par l'arduino
 * @return tableau d'indice des 4 cotés {Nord, Est, Sud, Ouest}
*/
int *getIndexes(VecteurImg vect){
    int *indexes = (int *)malloc(NB_INDEXES * sizeof(int));
    indexes[0] = getIndex(calculateNorthMean(vect));
    indexes[1] =  getIndex(getEast(vect));
    indexes[2] = getIndex(getSouth(vect));
    indexes[3] = getIndex(getWest(vect));
    translateVect(vect, indexes);
    return indexes;
}

/**
 * @brief Calcule d'indice par rapport à une distance 
*/
int getIndex(int distance){
    return distance/DIM_CELL;
}

/**
 * @brief translation les indices pour les avoir dans le bon ordre selon le retour de la boussole
 * @param vect vecteur de position 
 * @param indexes tableau d'indices calculé sans prise en compte de la boussole  
*/
void translateVect(VecteurImg vect, int* indexes){
    int bousole = getBoussole(vect);
    int north = 0;
    int est = 1;
    int south = 2;
    int west = 3;
    switch (bousole)
    {
    case 0:
        printf("#### robot orienté vers le <NORD> ##\n");
        break;
    case 2:
        printf("#### robot orienté vers <EST> \n");
        north = 3;
        est = 0;
        south = 1;
        west = 2;
        break;
    case 1:
        printf("#### robot orienté vers le <SUD> \n");
        north = 2;
        est = 3;
        south = 0;
        west = 1;
        break;
    case 3:
        printf("#### robot orientéé vers <OUEST> \n");
        north = 1;
        est = 2;
        south = 3;
        west = 0;
        break;

    default:
        break;
    }
    int northIndex = indexes[north];
    int estIndex = indexes[est];
    int southIndex = indexes[south];
    int westIndex = indexes[west];

    indexes[0] = northIndex;
    indexes[1] = estIndex;
    indexes[2] = southIndex;
    indexes[3] = westIndex;
}

/**
 * @brief Calcule de l'état dans la matrice (room) selon le vecteur de position
 * @param vect vecteur position 
 * @param room environnement du robot (matrice : nbligne * nbColonne)
 * @return envoie le numéro d'état correspondant à sa position, -1 en cas d'echec ou de valeur incohérente
*/
int getState(VecteurImg vect, Room room){
    int *indexes = getIndexes(vect);

    int col = (int)(indexes[3] + (room.nbCol - indexes[1] - 1))/2;
    int lig = (int)(0.9 * indexes[0] + 0.1 * (room.nbLig- indexes[2] - 1));

    if(col >= room.nbCol || lig >= room.nbLig){
        printf("Erreur de calcul de la colonne et la ligne \n");
        return -1;
    } 

    int state = (room.nbCol)*lig + col;

    if(state >= room.nbStats){
        printf("Erreur de calcule d'état \n");
        return -1;
    } 
    printf("La ligne detecté => %d | La colonne detecté => %d \n", lig, col);

    printf(" =====> état correspondant : %d\n",state);

    return state;
}

void normalizeVec(VecteurImg vect){

}

float calculateSimilarity(VecteurImg vec1, VecteurImg vec2){
    return 0.0f;
}

/**
 * @brief génère une ligne aléatoire pour la simulation de vecteur 
*/
int generateLig(int nbLig){
    return rand() % nbLig;
}

/**
 * @brief génère une colonne aléatoire pour la simulation de vecteur 
*/
int generateCol(int nbCol){
    return rand() % nbCol;
}

/**
 * @brief génère une valeur aléatoire pour la boussole
*/
int generateBoussole(){
    return rand()% B_RANGE;
}

/**
 * @brief génère un vecteur aléatoire à partir de l'environnement du robot
 * @param room environnement où évolue le robot (matrice)
 * @return un vecteur contenant les distances des capteurs 
*/
VecteurImg generateRandomVector(Room room){
    int line = generateLig(room.nbLig); // vrai ligne 
    int col = generateCol(room.nbCol); // vrai colonne 
    int boussole = generateBoussole();


    printf("Etat choisi : ligne => %d, colonne => %d, boussole => %d\n", line, col, boussole);

    int northCel = 0;
    int westCel = 0;
    int estCel = 0;
    int southCel = 0;
    switch (boussole)
    {
    case 0:
        northCel = line;
        westCel = col;
        southCel = room.nbLig - line - 1;
        estCel = room.nbCol - col - 1;
        break;
    case 2:
        northCel = room.nbCol - col - 1;
        southCel = col;
        estCel = room.nbLig - line - 1;
        westCel = line;
        break;
    case 1:
        northCel = room.nbLig - line - 1;
        westCel = room.nbCol - col - 1;
        southCel = line;
        estCel = col;
        break;

    case 3:
        northCel = col;
        westCel = room.nbLig - line - 1;
        estCel = line;
        southCel = room.nbCol - col - 1;
        break;
    
    default:
        break;
    }
    unsigned int *northDistances = randomNorthDistance(northCel);

    VecteurImg vect;
    for(int i = 0; i < AMOUNT_NORTH; i++){
        vect.array[i] = northDistances[i];
    }

    vect.array[8] = randomDistance(estCel);
    vect.array[9] = randomDistance(southCel);
    vect.array[10] = randomDistance(westCel);
    vect.array[11] = boussole;

    afficherVect(vect);
    return vect;
}

/**
 * @brief Affiche les distances du vecteurs 
*/
void afficherVect(VecteurImg vect){
    printf(" Distance Nord : ");
    for(int i = 0; i < AMOUNT_NORTH; i++){
        printf(" %d ",vect.array[i]);
    }
    printf("\n Distance Est : %d ",getEast(vect));
    printf("\n Distance Sud : %d ",getSouth(vect));
    printf("\n Distance Ouest : %d ", getWest(vect));
    printf("\n Boussole : %d \n", getBoussole(vect));
}

int isOnNorthCorner(int state, Room room){
    return state/room.nbCol == 0;
}

int isOnSouthCorner(int state, Room room){
    return state/room.nbCol == room.nbLig - 1;
}

int isOnRightCorner(int state, Room room){
    return state%room.nbCol == room.nbCol - 1;
}

int isOnLeftCorner(int state, Room room){
    return state%room.nbCol == 0;
}

int nextBoussoleVal(int action){
    return action;
}

void generateNextState(VecteurImg *vect, int state, int action, Room room){
    int dx = 0;
    int dy = 0;
    int error = rand()%10;
    int step_error = rand()%(DIM_CELL/2);
    int direction[3] = {0, 1, -1};
    int random_direction = rand()%2;
    int randDir = direction[random_direction];
    switch (action)
    {
    case 0:
        dy = -STEP + error;
        if(isOnLeftCorner(state, room)){
            randDir = 1;
        }else{
            if(isOnRightCorner(state, room)){
                randDir = -1;
            }
        }
        dx = step_error*randDir;
       
        break;
    case 1:
        dy = STEP + error;
        if(isOnLeftCorner(state, room)){
            randDir = 1;
        }else{
            if(isOnRightCorner(state, room)){
                randDir = -1;
            }
        }
        dx = step_error*randDir;
        break;
    case 2:
        dx = STEP + error;
        if(isOnNorthCorner(state, room)){
            randDir = 1;
        }else{
            if(isOnSouthCorner(state, room)){
                randDir = -1;
            }
        }
        dy = step_error*randDir;
        break;
    case 3:
        dx = -STEP + error;
        if(isOnLeftCorner(state, room)){
            randDir = 1;
        }else{
            if(isOnRightCorner(state, room)){
                randDir = -1;
            }
        }
        dy = step_error*randDir;
        break;
    default:
        break;
    }

    int northStep = 0;
    int southStep = 0;
    int estStep = 0;
    int westStep = 0;

    rotateVect(vect, nextBoussoleVal(action));
    switch (getBoussole(*vect))
    {
    case 0:
        northStep = dy;
        southStep = -dy;
        estStep = -dx;
        westStep = dx;
        break;
    case 1:
        northStep = -dy;
        southStep = dy;
        westStep = -dx;
        estStep = dx;
        break;
    case 2:
        westStep = dy;
        estStep = -dy;
        northStep = -dx;
        southStep = dx;
        break;
    case 3:
        westStep = -dy;
        estStep = dy;
        northStep = dx;
        southStep = -dx;
        break;
    default:
        break;
    }

    // TODO faire la translation avec la rotation d'abord 
     printf("north step => %d, southStep => %d, estStep => %d, westStep => %d \n",northStep, southStep, estStep, westStep);
    for(int i = 0; i < AMOUNT_NORTH; i++){
        vect->array[i] += northStep + rand()%10;
    }
   
    vect->array[8] += estStep;
    vect->array[9] += southStep;
    vect->array[10] += westStep;

}

int getNbRotation(int boussole, int newBoussole){
    int nbrot = 0;
    switch (boussole)
    {
    case 0:
        switch (newBoussole)
        {
        case 2:
            nbrot = 1;
            break;
        case 1:
            nbrot = 2;
            break;
        case 3:
            nbrot = 3;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (newBoussole)
        {
        case 2:
            nbrot = 3;
            break;
        case 0:
            nbrot = 2;
            break;
        case 3:
            nbrot = 1;
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (newBoussole)
        {
        case 1:
            nbrot = 1;
            break;
        case 3:
            nbrot = 2;
            break;
        case 0:
            nbrot = 3;
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (newBoussole)
        {
        case 0:
            nbrot = 1;
            break;
        case 2:
            nbrot = 2;
            break;
        case 1:
            nbrot = 3;
            break;
        default:
            break;
        }
        break; 
    default:
        break;
    }
    
    return nbrot;
}


void rotateVect(VecteurImg *vect, int newBoussole){
    int distances[4] = {calculateNorthMean(*vect), getEast(*vect), getSouth(*vect), getWest(*vect)};
    int newdistances[4];
    for(int i = 0; i < 4; i++){
        int index = (i + getNbRotation(getBoussole(*vect), newBoussole))%4;
        newdistances[i] = distances[index];
    }

    float totalAngleRange = 2 * PI/3;
    float step = totalAngleRange / (AMOUNT_NORTH - 1);
    float startAngle = PI/6; // 30°
    float currentA;
    // calcul des 8 mesures du premier capteur + une erreur de 10cm 
    for (int i = 0; i < AMOUNT_NORTH; i++) {
        currentA = startAngle + (i * step);
        vect->array[i] = getHypo(currentA, newdistances[0]) + rand()%10;
    }

    vect->array[8] = newdistances[1];
    vect->array[9] = newdistances[2];
    vect->array[10] = newdistances[3];
    vect->array[11] = newBoussole;
}

int isNumber(char c){
    return  c >= '0' && c <= '9';
}

/**
 * @brief Construit le vecteur position à partir du message envoyé par l'arduino 
 * @param message message envoyé par l'arduino avec le format {N1:N2:N3:N4:N5:N6:N7:N8:E:S:O:B}
*/
VecteurImg getVectorFromArduino(char *message){
    VecteurImg vect;
    int i = 0;
    int capt = 0;
    int nb = 0;
    while(message[i] != '\0'){
        if(isNumber(message[i])){
            int asci = message[i] - '0';
            nb = nb*10 + asci;
        }else{
            if(message[i] == ':' || message[i] == '}' ){
                vect.array[capt] = nb;
                capt ++;
                nb = 0;
            }
        }
        i++;
    }

    return vect;
}
