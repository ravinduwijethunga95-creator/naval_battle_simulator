#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "battleship.h"

// Helper comparison function to sort escort ships by distance (Closest first for Attack Order)
int compareEscorts(const void *a, const void *b) {
    struct EscortShip *shipA = (struct EscortShip *)a;
    struct EscortShip *shipB = (struct EscortShip *)b;
    if (shipA->x < shipB->x) return -1;
    if (shipA->x > shipB->x) return 1;
    return 0;
}

void runPart2A() {
    struct Battleship B;
    int n;

    printf("\n--- Part 2-A: Attack Order & Firing Interval Simulation ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type);
    printf("Enter Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    printf("Enter Battleship Firing Interval / Reload Time (T_B^q): ");
    scanf("%f", &B.firingInterval);
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
        ships[i].health = 100.0;     // Cumulative health initialization
        ships[i].isDestroyed = 0;    // Active status flag
    }

    qsort(ships, n, sizeof(struct EscortShip), compareEscorts);

    FILE *fp = fopen("part2a_output.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "        PART 2-A SIMULATION LOG         \n");
        fprintf(fp, "========================================\n");
        fprintf(fp, "Battleship -> Type: %c | Pos: (%.2f, %.2f) | Firing Interval (T_B^q): %.2fs\n\n", 
                B.type, B.x, B.y, B.firingInterval);
        fprintf(fp, "Determined Attack Order (Sorted by Proximity/Threat):\n");
        for (int i = 0; i < n; i++) {
            fprintf(fp, "  Priority %d -> Escort ID: %d [%s] at Pos (%.2f, %.2f)\n", 
                    i + 1, ships[i].id, ships[i].type, ships[i].x, ships[i].y);
        }
        fprintf(fp, "\n----------------------------------------\n");
    }

    printf("\n--- Processing Attacks in Planned Order (considering T_B^q = %.2fs) ---\n", B.firingInterval);
    float b_max_range = (B.velocity * B.velocity) / G;
    float simulated_time = 0.0;
    int destroyed_count = 0;

    for (int i = 0; i < n; i++) {
        float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
        float rad = ships[i].angle * (PI / 180.0);
        float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

        // Account for reload/firing interval time delay per shot
        simulated_time += B.firingInterval;

        // Cumulative damage implementation for Battleship attacking Escort
        if (distance <= b_max_range && !ships[i].isDestroyed) {
            float b_damage = 25.0; // Cumulative damage per hit
            ships[i].health -= b_damage;
            if (ships[i].health < 0.0) ships[i].health = 0.0;

            printf("  -> [Time %.1fs] Escort %s (ID: %d) hit! Health left: %.1f%%\n", 
                   simulated_time, ships[i].type, ships[i].id, ships[i].health);
            if (fp) {
                fprintf(fp, "  -> [Time %.1fs] Escort ID: %d [%s] hit | Health Left: %.1f%%\n", 
                        simulated_time, ships[i].id, ships[i].type, ships[i].health);
            }

            if (ships[i].health <= 0.0) {
                ships[i].isDestroyed = 1;
                destroyed_count++;
                printf("     -> [Destroyed]: Escort ID: %d completely destroyed!\n", ships[i].id);
                if (fp) {
                    fprintf(fp, "     -> [DESTROYED]: Escort ID: %d [%s]\n", ships[i].id, ships[i].type);
                }
            }
        } else {
            if (fp && !ships[i].isDestroyed) {
                fprintf(fp, "  -> [Time %.1fs] Escort ID: %d [%s] OUT OF RANGE (Dist: %.2fm)\n", 
                        simulated_time, ships[i].id, ships[i].type, distance);
            }
        }

        // Check if escort counter-attacks
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

            printf("     [Danger] Counter-attacked by Escort ID: %d | Health Left: %.1f%%\n", ships[i].id, B.health);
            if (fp) {
                fprintf(fp, "     [HIT ON B] Counter-attacked by Escort ID: %d | Health Left: %.1f%%\n", ships[i].id, B.health);
            }
        }

        if (B.health <= 0.0) {
            printf("\n[Critical]: Battleship SUNK during engagement sequence!\n");
            if (fp) fprintf(fp, "\n[Critical]: Battleship SUNK during engagement sequence!\n");
            break;
        }
    }

    if (fp) {
        fprintf(fp, "\n----------------------------------------\n");
        fprintf(fp, "Final Simulation Results:\n");
        fprintf(fp, " - Total Time Elapsed: %.1f seconds\n", simulated_time);
        fprintf(fp, " - Escort Ships Destroyed: %d / %d\n", destroyed_count, n);
        fprintf(fp, " - Battleship Final Health: %.2f%%\n", B.health);
        fclose(fp);
        printf("\n[Success]: Results and attack order saved to 'part2a_output.txt'\n");
    }

    free(ships);
}