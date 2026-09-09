#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "battleship.h"

void runPart1C_1B_Sim1() {
    struct Battleship B;
    int n;

    printf("\n--- Part 1-C: Part 1-B Sim 1 (Cumulative Path) ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type); 
    printf("Enter Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    B.health = 100.0;

    printf("Enter Number of Escort Ships (N): ");
    scanf("%d", &n);

    struct EscortShip *ships = (struct EscortShip *)malloc(n * sizeof(struct EscortShip));
    if (ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    initializeEscortShips(ships, n, B.velocity);
    for (int i = 0; i < n; i++) {
        ships[i].health = 100.0;
        ships[i].isDestroyed = 0;
    }

    FILE *fp = fopen("part1c_1b_sim1_output.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "========================================\n");
        fprintf(fp, " PART 1-C (1-B SIM 1) CUMULATIVE PATH   \n");
        fprintf(fp, "========================================\n");
        fprintf(fp, "Battleship -> Type: %c | Pos: (%.2f, %.2f) | Initial Health: %.2f%%\n\n", B.type, B.x, B.y, B.health);
    }

    printf("\n--- Processing Path Simulation with Cumulative Impact ---\n");
    float b_max_range = (B.velocity * B.velocity) / G;
    int destroyed_count = 0;

    // Simulating movement/path steps
    for (int step = 1; step <= 5; step++) {
        printf("\n[Simulation Step %d]\n", step);
        if (fp) fprintf(fp, "\n--- Step %d ---\n", step);

        for (int i = 0; i < n; i++) {
            // Path movement adjustment simulation per step
            ships[i].y -= 10.0 * step; 
            if (ships[i].y < 0) ships[i].y = 0;

            float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
            float rad = ships[i].angle * (PI / 180.0);
            float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

            if (distance <= b_max_range && !ships[i].isDestroyed) {
                float b_damage = 25.0;
                ships[i].health -= b_damage;
                if (ships[i].health < 0.0) ships[i].health = 0.0;

                printf("  -> [Hit]: Escort %s (ID: %d) hit at Distance: %.2fm. Health left: %.1f%%\n", 
                       ships[i].type, ships[i].id, distance, ships[i].health);
                if (fp) fprintf(fp, "  -> Escort ID: %d [%s] hit at Dist %.2fm | Health Left: %.1f%%\n", 
                                ships[i].id, ships[i].type, distance, ships[i].health);

                if (ships[i].health <= 0.0) {
                    ships[i].isDestroyed = 1;
                    destroyed_count++;
                    printf("     [Destroyed]: Escort ID: %d destroyed!\n", ships[i].id);
                    if (fp) fprintf(fp, "     [DESTROYED]: Escort ID: %d [%s]\n", ships[i].id, ships[i].type);
                }
            }

            if (distance <= escort_range) {
                float impact_power = 0.04;
                if (strcmp(ships[i].type, "EA") == 0) impact_power = 0.08;
                else if (strcmp(ships[i].type, "EB") == 0) impact_power = 0.06;
                else if (strcmp(ships[i].type, "EC") == 0) impact_power = 0.07;
                else if (strcmp(ships[i].type, "ED") == 0) impact_power = 0.05;
                else if (strcmp(ships[i].type, "EE") == 0) impact_power = 0.04;

                float damage = impact_power * 100.0;
                B.health -= damage;
                if (B.health < 0.0) B.health = 0.0;

                printf("  -> [Danger]: Battleship hit by Escort %s (ID: %d)! Health Left: %.1f%%\n", 
                       ships[i].type, ships[i].id, B.health);
                if (fp) fprintf(fp, "  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Health Left: %.1f%%\n", 
                                ships[i].id, ships[i].type, B.health);
            }
        }
    }

    if (fp) {
        fprintf(fp, "\n----------------------------------------\n");
        fprintf(fp, "Final Outcome:\n");
        fprintf(fp, " - Total Escort ships destroyed: %d / %d\n", destroyed_count, n);
        fprintf(fp, " - Battleship Final Health: %.2f%%\n", B.health);
        fclose(fp);
        printf("\n[Success]: Results saved to 'part1c_1b_sim1_output.txt'\n");
    }

    free(ships);
}
