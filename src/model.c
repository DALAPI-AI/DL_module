#include <unistd.h>
#include "utils/utils.h"
#include "utils/file.h"
#include "process/imageProcessing.h"
#ifdef __linux__
#include "process/comm.h"
#endif

int actions[NB_ACTIONS] = {
    0b00, // STRAIGHT
    0b01, // BACK 
    0b10, // RIGHT / est 
    0b11  // LEFT / ouest
};
float g = 0.95;


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    int height = 180;
    int width = 180;
    int finalState = 10;
    Room room = allocMemoireRoom(height, width);

    float **Q = learningModel(height, width, finalState, 100, g);
    
    printf("_____TEST RESULT _____\n");
    for(int s = 0; s < room.nbStats; s++){
        if ( s== finalState){
            printf("F ,");
        }else{
            printf("%d ,",indexMax(Q[s], NB_ACTIONS));

        }
        if((s+1)%room.nbCol == 0){
            printf("\n");
        }
    }
    printf("------------------------\n");
    VecteurImg sampleVectors[] = {
        {{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 1}},
        {{130, 140, 150, 160, 200, 180, 130, 110, 10, 20, 30, 3}},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 2}},
        {{250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360}},
        {{110, 110, 90, 70, 77, 102, 183, 138, 187, 20, 30, 2}}
    };
    int nbElt = 5;
    printf("#######################################################################\n");
    printf("\t\t Representation de vecteur \n");
    printf("#######################################################################\n");

    VecteurImg vect = generateRandomVector(room);
   
    int state = getState(vect, room);
    int action = indexMax(Q[state], NB_ACTIONS);
    printf("=================== Action à effectuer : %d ======================== \n",action);
    generateNextState(&vect, state, action, room);
    afficherVect(vect);
    int nextState =  getState(vect, room);

    printf("#######################################################################\n");
    char message[] = "{119:72:61:44:49:60:81:127:0:23:10:3}";
    VecteurImg test = getVectorFromArduino(message);
    afficherVect(test);
    printf("#######################################################################\n");
    

    int numEtat[5]= {2,5,6,8,4};
    if (saveVector(sampleVectors,numEtat,nbElt ,"vectors.txt") != 0) {
        printf("Error saving vector.\n");
        return 1;
    }

    printf("Affichage de tous les vecteurs:\n");
    VecteurImg* loadedVector = (VecteurImg*) malloc(nbElt*sizeof(VecteurImg));
    int loadEtat[5] = {0,0,0,0,0};
    if (loadVector(loadedVector,loadEtat ,&nbElt,"vectors.txt") != 0) {
        printf("Error loading vector.\n");
        return 1;
    }
    for (int i = 0; i < nbElt; i++) {
        for (int j = 0; j < 12; j++) {
            printf("%u ", loadedVector[i].array[j]);
        }
        
        printf("%d\n",loadEtat[i]);
    }
    VecteurImg newVector = {{250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360}};
    int newEtat = 9;
    printf("Ajout d'un vecteur :\n");
    if (appendVector(newVector,newEtat, "vectors.txt") != 0) {
        printf("Error appending vector.\n");
        return 1;
    }

    printf("Suppression du vecteur numero 2 (1 en algo)\n");
    if (deleteVector(1, "vectors.txt") != 0) {
        printf("Error deleting vector.\n");
        return 1;
    }

    printf("Rechargement des vecteurs\n");
    if (loadVector(loadedVector,loadEtat ,&nbElt,"vectors.txt") != 0) {
        printf("Error loading vector.\n");
        return 1;
    }

    printf("Affichage des vecteurs apres suppression:\n");
    FILE *file = fopen("vectors.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);

    printf("###################### SIMILARITE #################################\n");

    VecteurImg value_to_compare = {{110,110,90,70,75,100,180,140,190,20,30,2}};

    int similarIndex = calculateSimilarity(loadedVector, nbElt, value_to_compare);

    if (similarIndex == -1) {
        printf("No similar array found above the threshold.\n");
    } else {
        printf("Most similar array index: %d\n", similarIndex);
        printf("Most similar array: {");
        for (int i = 0; i < 12; i++) {
            printf("%d", sampleVectors[similarIndex].array[i]);
            if (i < 12 - 1) {
                printf(", ");
            }
        }
        printf("}\n");
    }

    printf("#######################################################\n");
    free(loadedVector);

    #ifdef __linux__
    int fd = configureSerial("/dev/ttyACM0",4800);
    
    char myMsg[5] = "G\n";
    char buffer[MAX_BUFFER_SIZE];
    int bufSize = MAX_BUFFER_SIZE;
    int i=0;
    while (1)
    {
        int nByte = retrieveMessage(fd,buffer,bufSize);

        if (nByte != 0){
            // Treat data and give action
            
            switch (rand()%4)
            {
            case 0:
                myMsg[0] = 'H';
                break;
            case 1:
                myMsg[0] = 'B';
                break;
            case 2:
                myMsg[0] = 'G';
                break;
            case 3:
                myMsg[0] = 'D';
                break;
            default:
                break;
            } 
            printf("j'ai recu %d %s\n",i++,buffer);
            sendMessage(fd,myMsg);
        }
        sleep(1);
    }
    #endif
    return 0;
}

