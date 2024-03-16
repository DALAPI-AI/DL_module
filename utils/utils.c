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

void generateTr(int nbActions, float ***T, Room room, int finalState){

    for(int s = 0; s < room.nbStats; s ++){
        for(int a = 0; a < nbActions; a ++){
            for(int sf = 0; sf < room.nbStats; sf++){
                T[s][a][sf] = 0;
            }
        }
    }

    for(int curentstate = 0; curentstate < room.nbStats; curentstate ++){
        if(curentstate == 0 || curentstate == room.nbCol-1){
            
            T[curentstate][0][curentstate] = 0.75;
            T[curentstate][1][curentstate + room.nbCol] = 0.5;
            T[curentstate][1][curentstate] = 0.25;

        if(curentstate != finalState){
            if(curentstate == 0){
                T[curentstate][0][curentstate + 1] = 0.25;
                T[curentstate][1][curentstate + room.nbCol + 1] = 0.25;
                T[curentstate][2][curentstate + 1] = 0.5;
                T[curentstate][2][curentstate] = 0.25;
                T[curentstate][2][curentstate + room.nbCol + 1] = 0.25;
                T[curentstate][3][curentstate] = 0.75;
                T[curentstate][3][curentstate + room.nbCol] = 0.25;
                
            }else{
                T[curentstate][0][curentstate - 1] = 0.25;
                T[curentstate][1][curentstate + room.nbCol - 1] = 0.25;
                T[curentstate][2][curentstate] = 0.75;
                T[curentstate][2][curentstate + room.nbCol] = 0.25;
                T[curentstate][3][curentstate - 1] = 0.5;
                T[curentstate][3][curentstate] = 0.25;
                T[curentstate][3][curentstate + room.nbCol - 1] = 0.25;
            } 
        }else{
            if(curentstate == (room.nbLig-1)*room.nbCol || curentstate == room.nbStats -1){
                T[curentstate][0][curentstate] = 0.25;
                T[curentstate][0][curentstate - room.nbCol] = 0.5;
                T[curentstate][1][curentstate] = 0.75;

                if(curentstate == (room.nbLig-1)*room.nbCol){
                    T[curentstate][0][curentstate - room.nbCol + 1] = 0.25;
                    T[curentstate][1][curentstate + 1] = 0.25;
                    T[curentstate][2][curentstate + 1] = 0.5;
                    T[curentstate][2][curentstate] = 0.25;
                    T[curentstate][2][curentstate - room.nbCol + 1] = 0.25;
                    T[curentstate][3][curentstate] = 0.75;
                    T[curentstate][3][curentstate - room.nbCol] = 0.25;

                }else{
                    T[curentstate][0][curentstate - room.nbCol - 1] = 0.25;
                    T[curentstate][1][curentstate - 1] = 0.25;
                    T[curentstate][2][curentstate] = 0.75;
                    T[curentstate][2][curentstate - room.nbCol] = 0.25;
                    T[curentstate][3][curentstate - 1] = 0.5;
                    T[curentstate][3][curentstate] = 0.25;
                    T[curentstate][3][curentstate - room.nbCol - 1] = 0.25;
                }
            }
            else{
                if(curentstate < room.nbCol - 1){
                    int action = 0;
                    T[curentstate][action][curentstate] = 0.5;
                    T[curentstate][action][curentstate + 1] = 0.25;
                    T[curentstate][action][curentstate - 1] = 0.25;
                    action = 1;
                    T[curentstate][action][curentstate + room.nbCol] = 0.5;
                    T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                    T[curentstate][action][curentstate + room.nbCol - 1] = 0.25;
                    action = 2;
                    T[curentstate][action][curentstate + 1] = 0.5;
                    T[curentstate][action][curentstate] = 0.25;
                    T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                    action = 3;
                    T[curentstate][action][curentstate - 1] = 0.5;
                    T[curentstate][action][curentstate] = 0.25;
                    T[curentstate][action][curentstate + room.nbCol - 1] = 0.25;

                }else{
                    if(curentstate%room.nbCol == 0){
                            int action = 0;
                            T[curentstate][action][curentstate] = 0.25;
                            T[curentstate][action][curentstate -room.nbCol] = 0.5;
                            T[curentstate][action][curentstate -room.nbCol + 1] = 0.25;
                            action = 1;
                            T[curentstate][action][curentstate + room.nbCol] = 0.5;
                            T[curentstate][action][curentstate] = 0.25;
                            T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                            action = 2;
                            T[curentstate][action][curentstate + 1] = 0.5;
                            T[curentstate][action][curentstate -room.nbCol + 1] = 0.25;
                            T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                            action = 3;
                            T[curentstate][action][curentstate - room.nbCol] = 0.25;
                            T[curentstate][action][curentstate] = 0.5;
                            T[curentstate][action][curentstate + room.nbCol ] = 0.25;
                    }else{
                            if((curentstate+1)%room.nbCol == 0){
                                int action = 0;
                                T[curentstate][action][curentstate - room.nbCol] = 0.5;
                                T[curentstate][action][curentstate] = 0.25;
                                T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                                action = 1;
                                T[curentstate][action][curentstate] = 0.25;
                                T[curentstate][action][curentstate + room.nbCol] = 0.5;
                                T[curentstate][action][curentstate +room.nbCol - 1] = 0.25;
                                action = 2;
                                T[curentstate][action][curentstate] = 0.5;
                                T[curentstate][action][curentstate + room.nbCol] = 0.25;
                                T[curentstate][action][curentstate - room.nbCol] = 0.25;
                                action = 3;
                                T[curentstate][action][curentstate - 1] = 0.5;
                                T[curentstate][action][curentstate + room.nbCol -1] = 0.25;
                                T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                            }else{
                                if((room.nbLig-1)*room.nbCol < curentstate && curentstate < room.nbStats - 1 ){
                                    int action = 0;
                                    T[curentstate][action][curentstate - room.nbCol] = 0.5;
                                    T[curentstate][action][curentstate - room.nbCol + 1] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                                    action = 1;
                                    T[curentstate][action][curentstate] = 0.5;
                                    T[curentstate][action][curentstate + 1] = 0.25;
                                    T[curentstate][action][curentstate - 1] = 0.25;
                                    action = 2;
                                    T[curentstate][action][curentstate + 1] = 0.5;
                                    T[curentstate][action][curentstate] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol + 1] = 0.25;
                                    action = 3;
                                    T[curentstate][action][curentstate - 1] = 0.5;
                                    T[curentstate][action][curentstate] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                                }else{
                                    int action = 0;
                                    T[curentstate][action][curentstate - room.nbCol] = 0.5;
                                    T[curentstate][action][curentstate - room.nbCol + 1] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                                    action = 1;
                                    T[curentstate][action][curentstate + room.nbCol] = 0.5;
                                    T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                                    T[curentstate][action][curentstate + room.nbCol - 1] = 0.25;
                                    action = 2;
                                    T[curentstate][action][curentstate + 1] = 0.5;
                                    T[curentstate][action][curentstate + room.nbCol + 1] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol + 1 ] = 0.25;
                                    action = 3;
                                    T[curentstate][action][curentstate - 1] = 0.5;
                                    T[curentstate][action][curentstate + room.nbCol - 1] = 0.25;
                                    T[curentstate][action][curentstate - room.nbCol - 1] = 0.25;
                                }
                            }
                    
                        }
                    }
                }

        }
        
        }
    }
}

void affichageT(float ***T, Room room, int nbactions){
    for(int s = 0; s < room.nbStats; s ++){
        printf("{\n");
        for(int a = 0; a < nbactions; a ++){
            printf("\t{ ");
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


