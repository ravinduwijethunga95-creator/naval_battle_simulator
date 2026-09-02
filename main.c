#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846
#define G 9.81  // Gravity

struct Battleship {
    char type;
    float x, y;
    float velocity;
    float health;
};

struct EscortShip {
    int id;
    char type[3];
    float x, y;
    float velocity;
    float angle;
    float minAngle;
    float impactPower;
    float health;
};

// Function to calculate projectile range: R = (u^2 * sin(2*theta)) / g
float calculateRange(float u, float theta) {
    float rad = theta * (PI / 180.0);
    return (u * u * sin(2 * rad)) / G;
}

int main() {
    srand(time(NULL));
    int choice;

    do {
        printf("\n=============================\n");
        printf("   NAVAL BATTLE SIMULATOR    \n");
        printf("=============================\n");
        printf("1. Start Simulation\n");
        printf("2. View Instructions\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                printf("\n--- Starting Simulation (Part 1-A) ---\n");
                
                // Battleship Setup
                struct Battleship B;
                B.type = 'U';
                B.x = 500.0;
                B.y = 500.0;
                B.velocity = 300.0;
                B.health = 100.0;

                int n = 5; // Number of escort ships
                struct EscortShip ships[5];

                for (int i = 0; i < n; i++) {
                    ships[i].id = i + 1;
                    ships[i].x = (rand() % 10000) / 10.0;
                    ships[i].y = (rand() % 10000) / 10.0;
                    ships[i].health = 100.0;

                    // Randomly select one of the 5 escort types from Table 1
                    int type_choice = rand() % 5;
                    if (type_choice == 0) {
                        sprintf(ships[i].type, "EA");
                        ships[i].impactPower = 0.08;
                        ships[i].minAngle = 20;
                        ships[i].velocity = 150.0 + (rand() % 50);
                    } else if (type_choice == 1) {
                        sprintf(ships[i].type, "EB");
                        ships[i].impactPower = 0.06;
                        ships[i].minAngle = 30;
                        ships[i].velocity = 140.0 + (rand() % 50);
                    } else if (type_choice == 2) {
                        sprintf(ships[i].type, "EC");
                        ships[i].impactPower = 0.07;
                        ships[i].minAngle = 25;
                        ships[i].velocity = 145.0 + (rand() % 50);
                    } else if (type_choice == 3) {
                        sprintf(ships[i].type, "ED");
                        ships[i].impactPower = 0.05;
                        ships[i].minAngle = 50;
                        ships[i].velocity = 130.0 + (rand() % 50);
                    } else {
                        sprintf(ships[i].type, "EE");
                        ships[i].impactPower = 0.04;
                        ships[i].minAngle = 70;
                        ships[i].velocity = 120.0 + (rand() % 50);
                    }
                    ships[i].angle = ships[i].minAngle + (rand() % 20);
                }

		// Part 1-A Simulation & Hit Detection Logic
                int b_sunk = 0;
                int sinking_e_id = -1;
                int hits_by_b = 0;

                // Battleship maximum range (at 45 degrees optimal angle)
                float b_max_range = (B.velocity * B.velocity) / G;

                for (int i = 0; i < n; i++) {
                    // Distance between Battleship and Escort Ship
                    float dx = ships[i].x - B.x;
                    float dy = ships[i].y - B.y;
                    float distance = sqrt(dx * dx + dy * dy);

                    // Escort ship's attack range
                    float e_range = calculateRange(ships[i].velocity, ships[i].angle);

                    // Check if Escort ship can hit Battleship (Part 1-A: single hit sinks B)
                    if (distance <= e_range) {
                        b_sunk = 1;
                        sinking_e_id = ships[i].id;
                        break;
                    }

                    // Check if Battleship can hit the Escort ship
                    if (distance <= b_max_range) {
                        hits_by_b++;
                    }
                }

                // Display Simulation Outcomes
                if (b_sunk) {
                    printf("\n[Battle Result]: Battleship SANK! Destroyed by Escort Ship ID: %d\n", sinking_e_id);
                } else {
                    printf("\n[Battle Result]: Battleship Survived!\n");
                    printf("Number of Escort Ships hit by Battleship: %d\n", hits_by_b);
                }

                // Display generated simulation state
                printf("\nBattleship Type: %c | Position: (%.2f, %.2f)\n", B.type, B.x, B.y);
                printf("\nEscort Ships on Battlefield:\n");
                for (int i = 0; i < n; i++) {
                    float range = calculateRange(ships[i].velocity, ships[i].angle);
                    printf("ID: %d | Type: %s | Pos: (%.2f, %.2f) | Range: %.2fm | Impact Power: %.2f\n", 
                           ships[i].id, ships[i].type, ships[i].x, ships[i].y, range, ships[i].impactPower);
                }

		// Save initial battlefield conditions to a text file
                FILE *fp = fopen("battlefield_init.txt", "w");
                if (fp == NULL) {
                    printf("Error opening file for writing!\n");
                } else {
                    fprintf(fp, "Battleship Type: %c | Position: (%.2f, %.2f)\n", B.type, B.x, B.y);
                    fprintf(fp, "Escort Ships Initial Details:\n");
                    for (int i = 0; i < n; i++) {
                        float range = calculateRange(ships[i].velocity, ships[i].angle);
                        fprintf(fp, "ID: %d | Type: %s | Pos: (%.2f, %.2f) | Range: %.2fm | Impact Power: %.2f\n", 
                               ships[i].id, ships[i].type, ships[i].x, ships[i].y, range, ships[i].impactPower);
                    }
                    fclose(fp);
                    printf("\n[Success]: Battlefield initial conditions saved to 'battlefield_init.txt'!\n");
                }
                break;
            }
            case 2:
                printf("\n[Instructions]: This is a WWII naval simulator where a battleship engages escort ships using projectile motion physics.\n");
                break;
            case 3:
                printf("\nExiting simulator. Good luck with the assignment!\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while(choice != 3);

    return 0;
}
