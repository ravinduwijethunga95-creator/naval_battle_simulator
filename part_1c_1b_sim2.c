#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "battleship.h"

void runPart1C_1B_Sim2() {
    struct Battleship B;
    int n, k, t;

    printf("\n--- Part 1-C: 1-B Simulation 2 (Gun Jam with Cumulative Impact) ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type);
    printf("Enter Initial Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Battleship Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    B.health = 100.0; // Battleship health starts at 100%

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
    for (int i = 0; i < n; i++) {
        ships[i].health = 100.0; // Cumulative health initialization for Escort
        ships[i].isDestroyed = 0; // Active/Destroyed status flag
    }

    float *path_x = (float *)malloc(k * sizeof(float));
    float *path_y = (float *)malloc(k * sizeof(float));
    for (int i = 0; i < k; i++) {
        path_x[i] = (rand() % 10000) / 10.0;
        path_y[i] = (rand() % 10000) / 10.0;
    }

    FILE *fp = fopen("part1c_sim2_output.txt", "w");
    if (fp) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "  PART 1-C (1-B SIMULATION 2) CUMULATIVE LOG  \n");
        fprintf(fp, "========================================\n");
    }

    float theta_min_jam = 15.0; // Restricted min angle after gun jam

    for (int step = 0; step < k; step++) {
        B.x = path_x[step];
        B.y = path_y[step];
        
        int is_jammed = (step >= t);
        printf("\n[Step %d/%d] Pos: (%.2f, %.2f) Health: %.1f%% %s\n", step + 1, k, B.x, B.y, B.health, is_jammed ? "[GUN JAMMED]" : "[NORMAL]");
        if (fp) fprintf(fp, "\n--- Step %d %s | Health: %.1f%% ---\n", step + 1, is_jammed ? "[JAMMED]" : "[NORMAL]", B.health);

        float effective_angle = is_jammed ? theta_min_jam : 45.0; 
        float rad_b = effective_angle * (PI / 180.0);
        float b_effective_range = (B.velocity * B.velocity * sin(2 * rad_b)) / G;

        for (int i = 0; i < n; i++) {
            if (ships[i].isDestroyed) continue;

            float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
            float rad = ships[i].angle * (PI / 180.0);
            float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

            // Battleship cumulative damage on Escort (e.g., 25.0 damage per hit)
            if (distance <= b_effective_range) {
                float b_damage = 25.0;
                ships[i].health -= b_damage;
                if (ships[i].health < 0.0) ships[i].health = 0.0;

                printf("  -> Escort %s (ID: %d) hit! Health left: %.1f%%\n", ships[i].type, ships[i].id, ships[i].health);
                if (fp) fprintf(fp, "  -> Escort ID: %d [%s] hit | Health Left: %.1f%%\n", ships[i].id, ships[i].type, ships[i].health);

                if (ships[i].health <= 0.0) {
                    ships[i].isDestroyed = 1;
                    printf("     [Destroyed]: Escort ID: %d completely destroyed!\n", ships[i].id);
                    if (fp) fprintf(fp, "     [DESTROYED]: Escort ID: %d [%s]\n", ships[i].id, ships[i].type);
                }
            }

            // Escort attack on Battleship with cumulative impact power percentages
            if (distance <= escort_range) {
                float impact_power = 0.04; // Default for EE
                if (strcmp(ships[i].type, "EA") == 0) impact_power = 0.08;      // 8%
                else if (strcmp(ships[i].type, "EB") == 0) impact_power = 0.06; // 6%
                else if (strcmp(ships[i].type, "EC") == 0) impact_power = 0.07; // 7%
                else if (strcmp(ships[i].type, "ED") == 0) impact_power = 0.05; // 5%
                else if (strcmp(ships[i].type, "EE") == 0) impact_power = 0.04; // 4%

                float damage = impact_power * 100.0;
                B.health -= damage;
                if (B.health < 0.0) B.health = 0.0;

                printf("  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Damage: %.1f%% | Health Left: %.1f%%\n", 
                       ships[i].id, ships[i].type, damage, B.health);
                if (fp) fprintf(fp, "  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Damage: %.1f%% | Health Left: %.1f%%\n", 
                                ships[i].id, ships[i].type, damage, B.health);
            }
        }

        if (B.health <= 0.0) {
            printf("\n  -> [CRITICAL]: Battleship SUNK due to cumulative damage at step %d!\n", step + 1);
            if (fp) fprintf(fp, "\n[CRITICAL]: Battleship SUNK due to cumulative damage at step %d!\n", step + 1);
            break;
        }
    }

    if (fp) {
        fprintf(fp, "\n----------------------------------------\n");
        if (B.health <= 0.0) {
            fprintf(fp, "Final Outcome: Battleship SUNK.\n");
        } else {
            fprintf(fp, "Final Outcome: Battleship SURVIVED. Final Health: %.2f%%\n", B.health);
        }
        fclose(fp);
        printf("\n[Success]: Results saved to 'part1c_sim2_output.txt'\n");
    }

    free(ships);
    free(path_x);
    free(path_y);
}
