#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
typedef struct vector
{
    unsigned int array[12];
} VecteurImg;

unsigned int* getNorth(VecteurImg vect);
unsigned int getSouth(VecteurImg vect);
unsigned int getWest(VecteurImg vect);
unsigned int getEast(VecteurImg vect);
unsigned int getBoussole(VecteurImg vect);

#endif // IMAGE_PROCESS_H
