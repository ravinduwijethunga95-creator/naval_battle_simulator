#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battleship.h"

void runPart1B_Sim2() {
    struct Battleship B;
    int n, k, t;

    printf("\n--- Part 1-B: Simulation 2 (Gun Jam) ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type);
    printf("Enter Initial Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Battleship Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    B.health = 100.0;

    printf("Enter Number of Escort Ships (N): ");
    scanf("%d", &n);
    printf("Enter Total Path Points (k): ");
    scanf("%d", &k);
    printf("Enter Jam Iteration Threshold (t < %d): ", k);
    scanf("%d", &t);

    struct EscortShip *ships = (struct EscortShip *)malloc(n * sizeof(struct EscortShip));
    if (ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    initializeEscortShips(ships, n, B.velocity);
    for (int i = 0; i < n; i++) ships[i].active = 1;

    float *path_x = (float *)malloc(k * sizeof(float));
    float *path_y = (float *)malloc(k * sizeof(float));
    for (int i = 0; i < k; i++) {
        path_x[i] = (rand() % 10000) / 10.0;
        path_y[i] = (rand() % 10000) / 10.0;
    }

    FILE *fp = fopen("part1b_sim2_output.txt", "w");
    if (fp) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "      PART 1-B SIMULATION 2 LOG         \n");
        fprintf(fp, "========================================\n");
    }

    int b_sunk = 0;
    float theta_min_jam = 15.0; // Restricted min angle after jam (0 < theta_min < 30)

    for (int step = 0; step < k; step++) {
        B.x = path_x[step];
        B.y = path_y[step];
        
        int is_jammed = (step >= t);
        printf("\n[Step %d/%d] Pos: (%.2f, %.2f) %s\n", step + 1, k, B.x, B.y, is_jammed ? "[GUN JAMMED]" : "[NORMAL]");
        if (fp) fprintf(fp, "\n--- Step %d %s ---\n", step + 1, is_jammed ? "[JAMMED]" : "[NORMAL]");

        // If jammed, restrict effective angle range (e.g., min angle constraint between 0 and 30)
        float effective_angle = is_jammed ? theta_min_jam : 45.0; 
        float rad_b = effective_angle * (PI / 180.0);
        float b_effective_range = (B.velocity * B.velocity * sin(2 * rad_b)) / G;

        for (int i = 0; i < n; i++) {
            if (!ships[i].active) continue;

            float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
            float rad = ships[i].angle * (PI / 180.0);
            float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

            if (distance <= b_effective_range) {
                ships[i].active = 0;
                printf("  -> Escort %s (ID: %d) destroyed at distance %.2fm\n", ships[i].type, ships[i].id, distance);
                if (fp) fprintf(fp, "  -> Escort ID: %d [%s] DESTROYED at distance %.2fm\n", ships[i].id, ships[i].type, distance);
            }

            if (distance <= escort_range) {
                b_sunk = 1;
                printf("  -> [CRITICAL]: Battleship SUNK by Escort ID: %d [%s]!\n", ships[i].id, ships[i].type);
                if (fp) fprintf(fp, "  -> [CRITICAL]: Battleship SUNK by Escort ID: %d [%s]!\n", ships[i].id, ships[i].type);
                break;
            }
        }
        if (b_sunk) break;
    }

    if (fp) {
        fprintf(fp, "\n----------------------------------------\n");
        fprintf(fp, b_sunk ? "Final Outcome: Battleship SUNK (Gun Jam Impact).\n" : "Final Outcome: Battleship SURVIVED.\n");
        fclose(fp);
        printf("\n[Success]: Results saved to 'part1b_sim2_output.txt'\n");
    }

    free(ships);
    free(path_x);
    free(path_y);
}