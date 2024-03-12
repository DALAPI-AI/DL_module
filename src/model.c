#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NB_STATES 17
#define NB_ACTIONS 4
#define R_LOOP 1
#define R_HIT -10
#define R_WIN 20

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

float gamma = 0.95;

// Pour chaque action : probabilité :
// une ligne <=> un etat de départ : action1=>[proba d'arriver en etat0, proba d'arriver en etat1,...], action2 =>[proba d'arriver etat1, ....] 
float T[NB_STATES][NB_ACTIONS][NB_STATES] = {
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
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 2
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 3
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 4
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 5
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 6
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 7
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 8
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 9
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 10
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 11
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 12
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 13
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 14
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 15
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 16
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    },
    { // état 16
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
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


int main(int argc, char const *argv[])
{
    for(int i = 0; i < 100; i++){
        usleep(50000);
        
        for(int s = 0; s < NB_STATES; s++){
            printf("\n");
            for(int a = 0; a < NB_ACTIONS; a++){
                float sum = 0;
                for(int s = 0; s < NB_STATES; s++){
                    //printf("T : %f | R : %d | Q : %f \n",T[s][a][s], R[s][a][s], max(Q[s], NB_ACTIONS));
                    sum += T[s][a][s] * (R[s][a][s] + gamma * max(Q[s], NB_ACTIONS));
                }
                Q[s][a] = sum;
            }
        }

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
    return 0;
}











