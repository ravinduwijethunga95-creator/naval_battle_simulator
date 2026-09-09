#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#define PI 3.14159265358979323846
#define G 9.81

struct Battleship {
    char type;
    float x;
    float y;
    float velocity;
    float health;
    float firingInterval; // Part 2-A : T_B^q (Reload / Firing time gap)
    int firingCount;//part 2-c
    float gamma;
    float initialImpactPower;
    float currentImpactPower;
};

struct EscortShip {
    int id;
    char type[3];
    float x;
    float y;
    float velocity;
    float minAngle;
    float angle;
    float health;
    int isDestroyed;
    int active;
    int firingCount;// part 2-c
    float gamma;
    float initialImpactPower;
    float currentImpactPower;
};

// Shared Helper Functions
float calculateDistance(float x1, float y1, float x2, float y2);
void initializeEscortShips(struct EscortShip ships[], int n, float b_max_velocity);

// Part 1 Functions
void runPart1A();
void runPart1B_Sim1();
void runPart1B_Sim2();
void runPart1C_1A();
void runPart1C_1B_Sim1();
void runPart1C_1B_Sim2();

// Part 2-A Function
void runPart2A();
void runPart2B();
void runPart2C();

#endif
