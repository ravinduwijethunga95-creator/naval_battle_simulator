#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battleship.h"

void runPart1B_Sim1() {
    struct Battleship B;
    int n, k;

    printf("\n--- Part 1-B: Simulation 1 (Path Movement) ---\n");
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
    printf("Enter Number of Path Points (k): ");
    scanf("%d", &k);

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

    FILE *fp = fopen("part1b_sim1_output.txt", "w");
    if (fp) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "      PART 1-B SIMULATION 1 LOG         \n");
        fprintf(fp, "========================================\n");
    }

    int b_sunk = 0;
    float b_max_range = (B.velocity * B.velocity) / G;

    for (int step = 0; step < k; step++) {
        B.x = path_x[step];
        B.y = path_y[step];
        printf("\n[Step %d/%d] Battleship moved to Position: (%.2f, %.2f)\n", step + 1, k, B.x, B.y);
        if (fp) fprintf(fp, "\n--- Step %d: Pos (%.2f, %.2f) ---\n", step + 1, B.x, B.y);

        for (int i = 0; i < n; i++) {
            if (!ships[i].active) continue;

            float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
            float rad = ships[i].angle * (PI / 180.0);
            float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

            if (distance <= b_max_range) {
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
        fprintf(fp, b_sunk ? "Final Outcome: Battleship SUNK.\n" : "Final Outcome: Battleship SURVIVED the path.\n");
        fclose(fp);
        printf("\n[Success]: Results saved to 'part1b_sim1_output.txt'\n");
    }

    free(ships);
    free(path_x);
    free(path_y);
}