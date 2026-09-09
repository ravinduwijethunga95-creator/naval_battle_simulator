#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battleship.h"

// Distance calculating helper function
float calculateDistance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Escort ships initializing and ID generating function (ID 1 to N)
void initializeEscortShips(struct EscortShip ships[], int n, float b_max_velocity) {
    for (int i = 0; i < n; i++) {
        ships[i].id = i + 1; // ID එක 1 සිට N දක්වා automatically හැදේ
        ships[i].x = (rand() % 10000) / 10.0;
        ships[i].y = (rand() % 10000) / 10.0;

        int type_choice = rand() % 5;
        if (type_choice == 0) {
            sprintf(ships[i].type, "EA");
            ships[i].minAngle = 20.0;
            ships[i].velocity = 1.2 * b_max_velocity;
        } else if (type_choice == 1) {
            sprintf(ships[i].type, "EB");
            ships[i].minAngle = 30.0;
            ships[i].velocity = 50.0 + (rand() % (int)(b_max_velocity - 50));
        } else if (type_choice == 2) {
            sprintf(ships[i].type, "EC");
            ships[i].minAngle = 25.0;
            ships[i].velocity = 50.0 + (rand() % (int)(b_max_velocity - 50));
        } else if (type_choice == 3) {
            sprintf(ships[i].type, "ED");
            ships[i].minAngle = 50.0;
            ships[i].velocity = 50.0 + (rand() % (int)(b_max_velocity - 50));
        } else {
            sprintf(ships[i].type, "EE");
            ships[i].minAngle = 70.0;
            ships[i].velocity = 50.0 + (rand() % (int)(b_max_velocity - 50));
        }

        ships[i].angle = ships[i].minAngle + (rand() % 20);
        ships[i].health = 100.0;
        ships[i].active = 1;
    }
}