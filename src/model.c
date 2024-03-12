#include <stdio.h>
#include <stdlib.h>

#define NB_STATES 17
#define NB_ACTIONS 4
#define R_LOOP 0
#define R_HIT -100
#define R_WIN 200

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

int Q [NB_STATES][2] = {0};


// Pour chaque action : probabilité :
// une ligne <=> un etat de départ : action1=>[proba d'arriver en etat0, proba d'arriver en etat1,...], action2 =>[proba d'arriver etat1, ....] 
float T[NB_STATES][NB_ACTIONS][NB_STATES] = {
    { // état 2 
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.13}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0.02}, {16, 0.1}}, // action 1 : état d'arrivée , probabilité 
        {{0, 0.1}, {1, 0.15}, {2, 0.05}, {3, 0.15}, {4, 0.05}, {5, 0.15}, {6, 0.05}, {7, 0.05}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0.15}, {16, 0.1}}, // action 2
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}, // action 3 
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 1}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}}  // action 4
    },
    { // état 3
        {{0, 0.1}, {1, 0.1}, {2, 0.15}, {3, 0.13}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0.02}, {16, 0.1}},
        {{0, 0.1}, {1, 0.05}, {2, 0.15}, {3, 0.15}, {4, 0.05}, {5, 0.15}, {6, 0.05}, {7, 0.05}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0.15}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 1}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    },
    { // état 4
        {{0, 0.1}, {1, 0.1}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.13}, {12, 0}, {13, 0}, {14, 0}, {15, 0.02}, {16, 0.1}},
        {{0, 0.1}, {1, 0.15}, {2, 0.05}, {3, 0.15}, {4, 0.05}, {5, 0.15}, {6, 0.05}, {7, 0.05}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0.15}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
   
    },
    { // état 5
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}}
    },
    { // état 6
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}}
    },
    { // état 7
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.1}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.5}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0.5}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    },
    { // état 8
        {{0, 0.1}, {1, 0.1}, {2, 0.1}, {3, 0.15}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0.15}, {9, 0.1}, {10, 0.1}, {11, 0.1}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    },
    { // état 9
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.13}, {4, 0.1}, {5, 0.1}, {6, 0.1}, {7, 0.1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0.02}, {16, 0.1}},
        {{0, 0}, {1, 1}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 1}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}}
    },
    { // état 10
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.15}, {2, 0.1}, {3, 0.1}, {4, 0.1}, {5, 0.15}, {6, 0.1}, {7, 0.1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 1}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}}
    },
    { // état 11
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.1}, {2, 0.15}, {3, 0.1}, {4, 0.1}, {5, 0.15}, {6, 0.1}, {7, 0.1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 1}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    },
    { // état 12
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0.1}, {1, 0.125}, {2, 0.125}, {3, 0.15}, {4, 0.1}, {5, 0.1}, {6, 0.1}, {7, 0.1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0.1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    },
    { // état 13  HB
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 1}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 1}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}}
    },
    { // état 14  
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}}
    },
    { // état 15  
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 1}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 1}, {16, 0}}
    }
};

// pour chaque action => etat de départ, d'arrivé et recompense 
int R[NB_STATES][NB_ACTIONS][NB_STATES]={
    { // état 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 2
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 3
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 4
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 5
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 6
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 7
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 8
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 9
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 10
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 11
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 12
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 13
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 14
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 15
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }, 
    { // état 16
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, // action 1
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}, 
        {{R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_LOOP, R_HIT, R_WIN}}
    }
};

int main(int argc, char const *argv[])
{
    
    return 0;
}











