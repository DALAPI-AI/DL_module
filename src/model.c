#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NB_STATES 17
#define NB_ACTIONS 4
#define R_LOOP -1
#define R_HIT -10
#define R_WIN 999

/**
 * (HAUT, BAS, GAUCHE, DROITE)
 *  1 <=> active side
*/
int states [NB_STATES] = {
        0b00000, // RIEN
        0b00010, // DROITE
        0b00100, // GAUCHE
        0b00110, // GAUCHE | DROITE
        0b01000, // BAS
        0b01010, // BAS | DROITE
        0b01100, // BAS | GAUCHE
        0b01110, // BAS | GAUCHE | DROITE
        0b10000, // HAUT
        0b10010, // HAUT | DROITE 
        0b10100, // HAUT | GAUCHE
        0b10110, // HAUT | GAUCHE | DROITE 
        0b11000, // HAUT | BAS
        0b11010, // HAUT | BAS | DROITE 
        0b11100, // HAUT | BAS | GAUCHE
        0b11110, // HAUT | BAS | GAUCHE | DROITE 
        0b00001  // FIN  
    };


int actions[NB_ACTIONS] = {
    0b00, // STRAIGHT
    0b01, // BACK
    0b10, // RIGHT
    0b11  // LEFT
};

float Q [NB_STATES][NB_ACTIONS] = {0};

float gamma = 0.99;

// Pour chaque action : probabilité :
// une ligne <=> un etat de départ : action1=>[proba d'arriver en etat0, proba d'arriver en etat1,...], action2 =>[proba d'arriver etat1, ....] 
float T[NB_STATES][NB_ACTIONS][NB_STATES] = {
    { // état 1 
        {0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0, 0.0625}, // action 1 : état d'arrivée , probabilité 
        {0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0, 0.0625}, // action 2
        {0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0, 0.0625}, // action 3 
        {0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0, 0.0625}  // action 4
    },
    { // état 2 
        {0.1, 0.15, 0.1, 0.13, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0.02, 0.1}, // action 1 : état d'arrivée , probabilité 
        {0.1, 0.15, 0.05, 0.15, 0.05, 0.15, 0.05, 0.05, 0, 0, 0, 0, 0, 0, 0, 0.15, 0.1}, // action 2
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, // action 3 
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // action 4
    },
    { // état 3
        {0.1, 0.1, 0.15, 0.13, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0.02, 0.1},
        {0.1, 0.05, 0.15, 0.15, 0.05, 0.15, 0.05, 0.05, 0, 0, 0, 0, 0, 0, 0, 0.15, 0.1},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 4
        {0.1, 0.1, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.13, 0, 0, 0, 0.02, 0.1},
        {0.1, 0.15, 0.05, 0.15, 0.05, 0.15, 0.05, 0.05, 0, 0, 0, 0, 0, 0, 0, 0.15, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
   
    },
    { // état 5
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1},
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1}
    },
    { // état 6
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1}
    },
    { // état 7
        {0.1, 0.15, 0.1, 0.15, 0, 0, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 8
        {0.1, 0.1, 0.1, 0.15, 0, 0, 0, 0, 0.15, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 9
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.15, 0.1, 0.13, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0, 0, 0, 0.02, 0.1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { // état 10
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.15, 0.1, 0.1, 0.1, 0.15, 0.1, 0.1, 0, 0, 0, 0, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { // état 11
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.1, 0.15, 0.1, 0.1, 0.15, 0.1, 0.1, 0, 0, 0, 0, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 12
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0.1, 0.125, 0.125, 0.15, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0, 0, 0, 0, 0.1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 13  HB
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { // état 14  
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { // état 15  
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    // à revoir 
    { // état 16 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    },
    { // état 17  
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    }
};

 // TODO : redefinir la stratégie de selon le modèle choisi 
int R[NB_STATES][NB_ACTIONS][NB_STATES]={
    { // état 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 2
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 3
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 4
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 5
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 6
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 7
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 8
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 9
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 10
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 11
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 12
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 13
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 14
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 15
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }, 
    { // état 16
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    },
    { // état 17
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, // action 1
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}, 
        {R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}
    }
};

float max(float *tab, int nbElt){
    float max = tab[0];
    for(int i = 1; i < nbElt; i++){
        if(max < tab[i]){
            max = tab[i];
        }
    }
    return max;
}

float randomFloat()
{
      float r = (float)rand() / (float)RAND_MAX;
      return r;
}

int findBest(int cell){
    float best = Q[cell][0];
    int a = 0;
    for(int i = 1; i < NB_ACTIONS; i++){
        if (best < Q[cell][i])
        {
            best = Q[cell][i];
            a = i; 
        }
    }
    return a;
}

int generateFakeMouvement(int currentCell,int action){
    // Should return a new random state that should follow some logics
    // i mean we dont care lmao
    int next =  rand()%NB_STATES;
    while (next == currentCell)
    {
        next = rand()%NB_STATES;
    }
    // Don't let him die too quickly
    if (next == 15)
    {
        if(rand()%2 > 0 ){
            return next;
        }
        next = rand()%NB_STATES;
    }
    return next;
}

int main(int argc, char const *argv[])
{

    float moyenneReward[1000] = {0};
    int cptR = 0;
    
    srand(time(NULL));
    float learn = 0.1;
    float exploration = 1.0;

    // Pour chaque Episode
    for(int i = 0; i < 1000; i++){
        int currentCell = rand()%(NB_STATES-2); // On autorise pas l'apparition dans la case FIN ou BLOQUé HBGD
        int action=0;
        int nextCell=0;
        // Combien de mouvement total fera en 1 episode
        for (int j = 0; j < 100; j++)
        {
            // Selection via epsilon greedy
            if (randomFloat() >= exploration){
                // Choose a random state
                action = rand()%NB_ACTIONS;
            }else{
                // Choose the best action
                action = findBest(currentCell);
            }

            // Ici le robot devra se deplacer selon action et nous retourner les nouvelles infos
            // C'est la partie la plus compliqué il faut communiquer avec l'équipe Embedded
            nextCell = generateFakeMouvement(currentCell,action);

            int qval = T[currentCell][action][nextCell] * (R[currentCell][action][nextCell] + gamma * max(Q[nextCell], NB_ACTIONS));
            Q[currentCell][action] = (1-learn)* Q[currentCell][action] + learn* qval;
            
            moyenneReward[cptR] += R[currentCell][action][nextCell];
            currentCell = nextCell;
            
            // il arrive à la fin
            if (currentCell == 16 || currentCell == 15)
            {
                break;
            }

        }
        learn = learn * 0.999;
        exploration = exploration * 0.999;
        if (exploration < 0.01)
        {
            exploration = 0.01;
        }

        cptR++; 
        
        if (i%10 != 0) continue;
        printf("Iteration: %d\n", i);
        for(int s = 0; s < NB_STATES; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                printf("Q[State:%d, Action:%d]=%+10.4f", s, a, Q[s][a]);
                if( Q[s][a] == max(Q[s], NB_ACTIONS)){
                    printf(" <- ");
                }
                printf("\n");
            }
        }
    } 
    printf("---------------------------------\n");  
    // for (int i = 0; i < 9; i++)
    // {
    //     float moy = 0;
    //     for (int j = i*100; j < (i+1)*100; j++)
    //     {
    //         moy += moyenneReward[j];
    //     }
    //     moy = moy/100.0;
    //     printf("moyenne Reward a iteration %d : %f\n",(i+1)*100,moy);
    // }
    
    return 0;
}











