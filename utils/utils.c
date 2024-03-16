#include "utils.h"

Room allocMemoireRoom(int height, int width){
    int nbCol = width/DIM_CELL;
    int nblig = height/DIM_CELL;
    Room room;
    room.mat = (State**)malloc(nblig * sizeof(State*));
    if(room.mat == NULL){
        printf("Erreur d'allocation mémoire !!\n");
        exit(1);
    }
    for(int i = 0; i < nblig; i ++){
        room.mat[i] = (State*)malloc(nbCol * sizeof(State)); 
        if(room.mat[i] == NULL){
            printf("Erreur d'allocation mémoire !!\n");
            exit(1);
        }
    }
    room.nbCol = nbCol;
    room.nbLig = nblig;
    room.nbStats = nbCol * nblig;
    return room;
}

int randomPosition(Room room){
    srand(time(NULL));
    int col = rand()%(room.nbCol);
    int lig = rand()%(room.nbLig);
    int pos = (lig * room.nbCol) + col;
    return pos;
}


/* Check if the state is on the column like
    [A][B][C][D]
    [E][F][G][H]
    [I][J][K][L]
    A,E,I,D,H,L return true
*/
int isOnColumn(int numState, int nbColumn){
    return (((numState%nbColumn) == 0) || ((numState%nbColumn) == (nbColumn-1)));
}

/* Check if the state is on the row like
    [A][B][C][D]
    [E][F][G][H]
    [I][J][K][L]
    A,B,C,D,I,J,K,L return true
*/
int isOnRow(int numState, int nbRow, int totalColumn){
    return ((numState < totalColumn) || (numState >= (nbRow*(totalColumn-1))));
}

void applyProbability(int currentState, float*** T,int action,int edge1State, int mainState, int edge2State){
    T[currentState][action][edge1State] = EDGE_PROBALITY;
    T[currentState][action][mainState] = MAIN_PROBALITY;
    T[currentState][action][edge2State] = EDGE_PROBALITY;
}

void applyCornerProbability(int currentState, float*** T,int action,int edge1State, int mainState){
    T[currentState][action][edge1State] = EDGE_PROBALITY;
    T[currentState][action][mainState] = MAIN_PROBALITY + EDGE_PROBALITY;
}

// à voir sur les côté ce qui se passe 
void generateTr(int nbActions, float ***T, Room room){

    for(int s = 0; s < room.nbStats; s ++){
        for(int a = 0; a < nbActions; a ++){
            for(int sf = 0; sf < room.nbStats; sf++){
                T[s][a][sf] = 0;
            }
        }
    }

    // 0 UP, 1 DOWN , 2 RIGHT, 3 LEFT
    for (int currentState = 0; currentState < room.nbStats; currentState++){
        int UpLeft = currentState - 1 - room.nbCol;
        int Up = currentState - room.nbCol;
        int UpRight = currentState + 1 - room.nbCol;
        int Left = currentState - 1;
        int Right = currentState + 1;
        int DownLeft = currentState - 1 + room.nbCol;
        int Down = currentState + room.nbCol;
        int DownRight = currentState + 1 + room.nbCol;
        
        // suppose we are in the middle where each movement is possible
        if ((!isOnRow(currentState,room.nbLig,room.nbCol)) && (!isOnColumn(currentState,room.nbCol))){
            applyProbability(currentState,T,0,UpLeft,Up,UpRight);
            applyProbability(currentState,T,1,DownLeft,Down,DownRight);
            applyProbability(currentState,T,2,UpLeft,Left,DownLeft);
            applyProbability(currentState,T,3,UpRight,Right,DownRight);
        }else{
            // We are either at the first line or last line but not on the left or right wall
            if ((isOnRow(currentState,room.nbLig,room.nbCol)) && (!isOnColumn(currentState,room.nbCol)))
            {
                switch (currentState < room.nbLig)
                {
                    // we are on the first line no L/R wall
                case 1:
                    applyProbability(currentState,T,0,Left,currentState,Right);
                    applyProbability(currentState,T,1,DownLeft,Down,DownRight);
                    applyProbability(currentState,T,2,currentState,Right,DownRight);
                    applyProbability(currentState,T,3,currentState,Left,DownLeft);
                    break;
                    // we are on the last line no L/R wall
                default:
                    applyProbability(currentState,T,0,UpLeft,Up,UpRight);
                    applyProbability(currentState,T,1,Left,currentState,Right);
                    applyProbability(currentState,T,2,currentState,Right,UpRight);
                    applyProbability(currentState,T,3,currentState,Left,UpLeft);
                    break;
                }
                continue;
            }

            // Now we are the first column or last column but not on the up or down wall
            if ((!isOnRow(currentState,room.nbLig,room.nbCol)) && (isOnColumn(currentState,room.nbCol)))
            {
                switch (currentState%room.nbCol == 0)
                {
                    // we are on the first column no U/D wall
                case 1:
                    applyProbability(currentState,T,0,currentState,Up,UpRight);
                    applyProbability(currentState,T,1,currentState,Down,DownRight);
                    applyProbability(currentState,T,2,UpRight,Right,DownRight);
                    applyProbability(currentState,T,3,Up,currentState,Down);
                    break;
                    // we are on the last column no U/D wall
                default:
                    applyProbability(currentState,T,0,UpLeft,Up,currentState);
                    applyProbability(currentState,T,1,DownLeft,Down,currentState);
                    applyProbability(currentState,T,2,Up,currentState,Down);
                    applyProbability(currentState,T,3,UpLeft,Left,DownLeft);
                    break;
                }
                continue;
            }
        
            // Let work on the remaining corner
            // Upper left corner
            if (currentState == 0)
            {
                applyCornerProbability(currentState,T,0,Right,currentState);
                applyProbability(currentState,T,1,currentState,Down,DownRight);
                applyProbability(currentState,T,2,currentState,Right,DownRight);
                applyCornerProbability(currentState,T,3,Down,currentState);
                continue;
            }
            // Upper right corner
            if (currentState == room.nbCol)
            {
                applyCornerProbability(currentState,T,0,Left,currentState);
                applyProbability(currentState,T,1,currentState,Down,DownLeft);
                applyCornerProbability(currentState,T,2,Down,currentState);
                applyProbability(currentState,T,3,currentState,Left,DownLeft);
                continue;
            }
            // Lower left corner
            if (currentState%room.nbCol == 0)
            {
                applyProbability(currentState,T,0,currentState,Up,UpRight);
                applyCornerProbability(currentState,T,1,Right,currentState);
                applyProbability(currentState,T,2,currentState,Right,UpRight);
                applyCornerProbability(currentState,T,3,Up,currentState);
                continue;
            }
            // Lower right corner
            applyProbability(currentState,T,0,currentState,Up,UpLeft);
            applyCornerProbability(currentState,T,1,Left,currentState);
            applyCornerProbability(currentState,T,2,Up,currentState);
            applyProbability(currentState,T,3,currentState,Left,UpLeft);
        }
    }

}



void affichageT(float ***T, Room room, int nbactions){
    char* actionName[4] = {"Up","Down","Right","Left"};
    for(int s = 0; s < room.nbStats; s ++){
        printf("{ State %d\n",s);
        for(int a = 0; a < nbactions; a ++){
            printf("\t{ Action %s",actionName[a]);
            for(int sf = 0; sf < room.nbStats; sf ++){
                if(sf%room.nbCol == 0){
                    printf("\n\t");
                }
                printf("%0.2f, ", T[s][a][sf]);
            }
            printf("}\n");
        }
        printf("}\n");
    }
}

float ***allocMemoire3DimTab(int nbStates, int nbActions){
    float ***t = (float ***)malloc(nbStates * sizeof(float **));
    if(t == NULL){
        printf("Erreur d'allocation mémoire \n");
        exit(1);
    }
    for(int s = 0; s < nbStates; s ++){
        t[s] = (float **)malloc(nbActions * sizeof(float *));
        if(t[s] ==  NULL){
            printf("Erreur d'allocation mémoire");
            exit(1);
        }
        for(int a  = 0; a < nbActions; a ++){
            t[s][a] = (float *)malloc(nbStates * sizeof(float));
            if(t[s][a] == NULL){
                printf("Erreur d'allocation mémoire");
                exit(1);
            }
        }
    }
    for(int s = 0; s < nbStates; s++){
        for(int a = 0; a < nbActions; a++){
            for(int sf = 0; sf < nbStates; sf++){
                t[s][a][sf] = 0; 
            }
        }
    }

    return t;
}

float **allocMemoireQ(int nbActions, int nbStates){
    float **Q = (float**)malloc(nbStates * sizeof(float*));
    if(Q == NULL){
        printf("Erreur d'allocation mémoire !\n");
        exit(1);
    }
    for(int s = 0; s < nbStates; s ++){
        Q[s] = (float*)malloc(nbActions * sizeof(float));
        if(Q[s] == NULL){
            printf("Erreur d'allocation mémoire !\n");
            exit(1);
        }
    }
    for(int s = 0; s < nbStates; s++){
        for(int a = 0; a < nbActions; a++){
            Q[s][a] = 0;
        }
    }
    return Q;
}

void generateR(int nbActions, float ***R, Room room, int succState){
    for(int s = 0; s < room.nbStats; s++){
        for(int a = 0; a < nbActions; a++){
            R[s][a][s] = R_HIT;
            R[s][a][succState] = R_WIN;

        }
        if(s == 0){
            R[s][0][s+1] = R_HIT;
            R[s][3][s + room.nbCol] = R_HIT;
        }else{
            if(s == (room.nbCol -1)){
                R[s][0][s-1] = R_HIT;
                R[s][2][s + room.nbCol] = R_HIT;
            }else{
                if(s == ((room.nbLig-1)*room.nbCol)){
                    R[s][1][s+1] = R_HIT;
                    R[s][3][s - room.nbCol] = R_HIT;
                }else{
                    if(s == (room.nbStats -1)){
                        R[s][1][s-1] = R_HIT;
                        R[s][2][s-room.nbCol] = R_HIT;
                    }else{
                        if(s > 0 && s < (room.nbCol -1)){
                            R[s][0][s+1] = R_HIT;
                            R[s][0][s-1] = R_HIT;
                        }else{
                            if(s > ((room.nbLig-1)*room.nbCol) && s < (room.nbStats -1)){
                                R[s][1][s+1] = R_HIT;
                                R[s][1][s-1] = R_HIT;
                            }else{
                                if(s%(room.nbCol)==0){
                                   R[s][3][s - room.nbCol] = R_HIT; 
                                   R[s][3][s + room.nbCol] = R_HIT;
                                }else{
                                    if((s+1)%room.nbCol==0){
                                        R[s][2][s - room.nbCol] = R_HIT; 
                                        R[s][2][s + room.nbCol] = R_HIT;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*int main(int argc, char const *argv[])
{ 
    int nbactions = 4;
    int height = 120;
    int width = 120;
    Room room = allocMemoireRoom(height,width);
    printf(" nbcol = %d, nblig = %d \n", room.nbCol, room.nbLig);
    float ***T = allocMemoire3DimTab(room.nbStats, nbactions);
    generateTr(nbactions, T, room);
    //affichageT(T, room, nbactions);

    float ***R = allocMemoire3DimTab(room.nbStats, nbactions);
    generateR(nbactions, R, room, 2);

    affichageT(R, room, nbactions);
    return 0;
}

*/


