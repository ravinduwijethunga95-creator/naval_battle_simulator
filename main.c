#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846
#define G 9.81 // Gravity

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

// Function Prototypes
void setupBattleship(struct Battleship *B);
float calculateRange(float u, float theta);
void updatePositions(struct EscortShip ships[], int n, float time_step);
void generateEscortShips(struct EscortShip ships[], int n);
void saveInitialConditions(struct Battleship B, struct EscortShip ships[], int n);

int main() {
    srand(time(NULL));
    int choice;

    do {
        printf("\n==============================\n");
        printf("   NAVAL BATTLE SIMULATOR     \n");
        printf("==============================\n");
        printf("1. Start Simulation\n");
        printf("2. View Instructions\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("\n--- Starting Simulation --- \n");

                // Battleship Setup
		struct Battleship B;
		setupBattleship(&B); 

                int n = 5; // Number of escort ships
                struct EscortShip ships[5];

                // Generate and randomize escort ships using function
                generateEscortShips(ships, n);

                int k = 10; // Total movement steps
                float time_step = 1.0; 

                for (int step = 0; step < k; step++) {
                    printf("\n--- Simulation Step %d / %d ---\n", step + 1, k);

                    // 1. Update positions of active Escort Ships
                    updatePositions(ships, n, time_step);

                    // 2. Combat & Cumulative Damage Tracking for this step
                    float b_max_range = (B.velocity * B.velocity) / G;
                    int hits_by_b = 0;

                    for (int i = 0; i < n; i++) {
                        if (ships[i].health <= 0) continue; // Skip destroyed escort ships

                        float dx = ships[i].x - B.x;
                        float dy = ships[i].y - B.y;
                        float distance = sqrt(dx * dx + dy * dy);
                        float e_range = calculateRange(ships[i].velocity, ships[i].angle);

                        // Escort hits Battleship (Reduces Battleship Health)
                        if (distance <= e_range) {
                            float damage = ships[i].impactPower * 100.0; 
                            B.health -= damage;
                            printf("[Hit Alert]: Escort ID: %d (%s) hit Battleship! Damage: %.1f | Battleship Health Left: %.1f\n", 
                                   ships[i].id, ships[i].type, damage, B.health < 0 ? 0 : B.health);
                        }

                        // Battleship hits Escort (Reduces Escort Health)
                        if (distance <= b_max_range) {
                            hits_by_b++;
                            ships[i].health -= 25.0; // Battleship hit damage per step
                            printf("[Attack Alert]: Battleship hit Escort ID: %d (%s)! Escort Health Left: %.1f\n", 
                                   ships[i].id, ships[i].type, ships[i].health < 0 ? 0 : ships[i].health);
                        }
                    }

                    // Check if Battleship is destroyed
                    if (B.health <= 0) {
                        printf("\n[Battle Result]: Battleship SANK! Health reached 0 at Step %d.\n", step + 1);
                        break;
                    }

                    // Display step summary
                    printf("[Step %d Status]: Battleship Health: %.1f | Escort hits recorded: %d\n", 
                           step + 1, B.health, hits_by_b);
                }

                // Save initial conditions using modular function
                saveInitialConditions(B, ships, n);
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
    } while (choice != 3);

    return 0;
}

void setupBattleship(struct Battleship *B) {
    printf("\n--- Enter Battleship Details ---\n");

    printf("Enter Battleship Type (e.g., U, M, R, S): ");
    scanf(" %c", &B->type); 

    printf("Enter Initial X coordinate: ");
    scanf("%f", &B->x);

    printf("Enter Initial Y coordinate: ");
    scanf("%f", &B->y);

    printf("Enter Maximum Shell Velocity (V_max): ");
    scanf("%f", &B->velocity);

    B->health = 100.0;

    printf("[Success]: Battleship configuration completed!\n");
}

// Function to calculate projectile range: R = (u^2 * sin(2*theta)) / g
float calculateRange(float u, float theta) {
    float rad = theta * (PI / 180.0);
    return (u * u * sin(2 * rad)) / G;
}

// Function to update positions of active escort ships
void updatePositions(struct EscortShip ships[], int n, float time_step) {
    for (int i = 0; i < n; i++) {
        if (ships[i].health > 0) {
            float rad = ships[i].angle * (PI / 180.0);
            ships[i].x += ships[i].velocity * cos(rad) * time_step;
            ships[i].y += ships[i].velocity * sin(rad) * time_step;
        }
    }
}

// Function to generate and randomize escort ships
void generateEscortShips(struct EscortShip ships[], int n) {
    for (int i = 0; i < n; i++) {
        ships[i].id = i + 1;
        ships[i].x = (rand() % 10000) / 10.0;
        ships[i].y = (rand() % 10000) / 10.0;
        ships[i].health = 100.0;

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
}

// Function to save initial battlefield conditions to a text file
void saveInitialConditions(struct Battleship B, struct EscortShip ships[], int n) {
    FILE *fp = fopen("battlefield_init.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
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
