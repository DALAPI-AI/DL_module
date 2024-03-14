#include "utils.h"

Room allocMemoireRoom(int height, int width){
    int nbCol = width/DIM_CELL;
    int nblig = height/DIM_CELL;
    Room room;
    room.mat = (int**)malloc(nblig * sizeof(int*));
    if(room.mat == NULL){
        printf("Erreur d'allocation mémoire !!\n");
        exit(1);
    }
    for(int i = 0; i < nblig; i ++){
        room.mat[i] = (int*)malloc(nbCol * sizeof(int)); 
        if(room.mat[i] == NULL){
            printf("Erreur d'allocation mémoire !!\n");
            exit(1);
        }
    }
    room.nbCol = nbCol;
    room.nbLig = nblig;
    return room;
}


int randomPosition(Room room){
    srand(time(NULL));
    int col = rand()%(room.nbCol);
    int lig = rand()%(room.nbLig);
    int pos = (lig * room.nbCol) + col;
    return pos;
}

// à voir sur les côté ce qui se passe 
void generateTr(int nbState, int nbActions, float ***T, Room room){
    for(int curentstate = 0; curentstate < nbState; curentstate ++){
        for(int nextstate = 0; nextstate < nbState; nextstate ++){
            float res = 0;
            int action = 0;
            if(nextstate == curentstate - room.nbCol){
                    res = 0.5;
            }else{
                    if(nextstate == (curentstate - room.nbCol + 1) || nextstate == (curentstate - room.nbCol -1)){
                        res = 0.25;
                    }
            } 
            T[curentstate][action][nextstate] = res;

            action = 1;
            res = 0;
            if(nextstate == curentstate + room.nbCol){
                    res = 0.5;
            }else{
                    if(nextstate == (curentstate + room.nbCol + 1) || nextstate == (curentstate + room.nbCol -1)){
                        res = 0.25;
                    }
            } 
            T[curentstate][action][nextstate] = res;


            action = 2;
            res = 0;
                if(nextstate == curentstate + 1){
                    res = 0.5;
            }else{
                    if(nextstate == (curentstate - room.nbCol + 1) || nextstate == (curentstate + room.nbCol + 1)){
                        res = 0.25;
                    }
            } 
            T[curentstate][action][nextstate] = res;


            action = 3;
            res = 0;
            if(nextstate == curentstate - 1){
                    res = 0.5;
            }else{
                    if(nextstate == (curentstate - room.nbCol - 1) || nextstate == (curentstate + room.nbCol - 1)){
                        res = 0.25;
                    }
            }
            T[curentstate][action][nextstate] = res;

        }
    }

}


void affichageT(float ***T, int nbstates, int nbactions){
    for(int s = 0; s < nbstates; s ++){
        printf("{\n");
        for(int a = 0; a < nbactions; a ++){
            printf("\t{ ");
            for(int sf = 0; sf < nbstates; sf ++){
                printf("%0.2f, ", T[s][a][sf]);
            }
            printf("}\n");
        }
        printf("}\n");
    }
}

float ***allocMemoireT(int nbStates, int nbActions){
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
    return t;
}


int main(int argc, char const *argv[])
{
    int nbstates = 16; 
    int nbactions = 4;
    Room room = allocMemoireRoom(120,120);
    printf(" nbcol = %d, nblig = %d \n", room.nbCol, room.nbLig);
    float ***T = allocMemoireT(nbstates, nbactions);
    generateTr(nbstates, nbactions, T, room);
    affichageT(T, nbstates, nbactions);
    return 0;
}




