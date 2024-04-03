#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/file.h"

int saveVector(VecteurImg* vect,int* state,int nbVect, char* fileName){
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    for (int j = 0; j < nbVect; j++)
    {
        for (int i = 0; i < 12; i++) {
            fprintf(file, "%u", vect[j].array[i]);
            if (i < 11) {
                fprintf(file, ",");
            }
        }
        fprintf(file, ",%d\n",state[j]);
    }

    fclose(file);
    return 0;   
}
int loadVector(VecteurImg* vect,int* state ,int* nbVect, char* fileName){
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    *nbVect = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        for (int i = 0; i < 12 && token != NULL; i++) {
            vect[*nbVect].array[i] = atoi(token);
            token = strtok(NULL, ",");
        }
        state[*nbVect] = atoi(token);
        (*nbVect)++;
    }
    fclose(file);
    return 0;
}
int appendVector(VecteurImg vect,int state ,char* fileName){
    FILE *file = fopen(fileName, "a");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < 12; i++) {
        fprintf(file, "%u", vect.array[i]);
        if (i < 11) {
            fprintf(file, ",");
        }
    }
    fprintf(file, ",%d\n", state);

    fclose(file);
    return 0;
}
int deleteVector(int index, char* fileName) {
    FILE *file = fopen(fileName, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    int currentIndex = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (currentIndex != index) {
            fputs(line, tempFile);
        }
        currentIndex++;
    }

    fclose(file);
    fclose(tempFile);

    if (remove(fileName) != 0) {
        perror("Error deleting file");
        return 1;
    }
    if (rename("temp.txt", fileName) != 0) {
        perror("Error renaming file");
        return 1;
    }

    return 0;
}