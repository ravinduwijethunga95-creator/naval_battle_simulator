#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "battleship.h"

void runPart1C_1A() {
    struct Battleship B;
    int n;

    printf("\n--- Part 1-C: Part 1-A (Cumulative Impact) ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type); 
    printf("Enter Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    B.health = 100.0; // Battleship health starts at 100%

    printf("Enter Number of Escort Ships (N): ");
    scanf("%d", &n);

    struct EscortShip *ships = (struct EscortShip *)malloc(n * sizeof(struct EscortShip));
    if (ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    initializeEscortShips(ships, n, B.velocity);
    
    // Initialize cumulative health for each escort ship
    for (int i = 0; i < n; i++) {
        ships[i].health = 100.0;
        ships[i].isDestroyed = 0;
    }

    FILE *fp = fopen("part1c_1a_output.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "    PART 1-C (1-A) CUMULATIVE LOG       \n");
        fprintf(fp, "========================================\n");
        fprintf(fp, "Battleship -> Type: %c | Pos: (%.2f, %.2f) | Initial Health: %.2f%%\n\n", B.type, B.x, B.y, B.health);
    }

    printf("\n--- Processing Combat Engagement with Cumulative Impact ---\n");
    float b_max_range = (B.velocity * B.velocity) / G;
    int destroyed_count = 0;

    for (int i = 0; i < n; i++) {
        float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
        float rad = ships[i].angle * (PI / 180.0);
        float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

        // Battleship attacks Escort ship with cumulative damage (25.0 per hit)
        if (distance <= b_max_range && !ships[i].isDestroyed) {
            float b_damage = 25.0;
            ships[i].health -= b_damage;
            if (ships[i].health < 0.0) ships[i].health = 0.0;

            printf("  -> [Hit]: Escort %s (ID: %d) hit at Distance: %.2fm. Health left: %.1f%%\n", 
                   ships[i].type, ships[i].id, distance, ships[i].health);
            if (fp) fprintf(fp, "  -> Escort ID: %d [%s] hit at Distance %.2fm | Damage: %.1f%% | Health Left: %.1f%%\n", 
                            ships[i].id, ships[i].type, distance, b_damage, ships[i].health);

            if (ships[i].health <= 0.0) {
                ships[i].isDestroyed = 1;
                destroyed_count++;
                printf("     [Destroyed]: Escort ID: %d has been completely destroyed!\n", ships[i].id);
                if (fp) fprintf(fp, "     [DESTROYED]: Escort ID: %d [%s] is fully destroyed.\n", ships[i].id, ships[i].type);
            }
        } else {
            if (fp && !ships[i].isDestroyed) {
                fprintf(fp, "  -> Escort ID: %d [%s] OUT OF RANGE at Distance %.2fm\n", ships[i].id, ships[i].type, distance);
            }
        }

        // Escort attacks Battleship (Cumulative Impact based on Table 1)
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

            printf("  -> [Danger]: Battleship hit by Escort %s (ID: %d)! Damage: %.1f%% | Remaining Health: %.1f%%\n", 
                   ships[i].type, ships[i].id, damage, B.health);
            if (fp) fprintf(fp, "  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Damage: %.1f%% | Health Left: %.1f%%\n", 
                            ships[i].id, ships[i].type, damage, B.health);
        }
    }

    if (fp) {
        fprintf(fp, "\n----------------------------------------\n");
        fprintf(fp, "Final Outcome:\n");
        fprintf(fp, " - Total Escort ships destroyed: %d / %d\n", destroyed_count, n);
        fprintf(fp, " - Battleship Final Health: %.2f%%\n", B.health);
        if (B.health <= 0.0) {
            fprintf(fp, " - Status: Battleship SUNK due to cumulative damage.\n");
        } else {
            fprintf(fp, " - Status: Battleship SURVIVED.\n");
        }
        fclose(fp);
        printf("\n[Success]: Results saved to 'part1c_1a_output.txt'\n");
    }

    if (B.health <= 0.0) {
        printf("\n[Outcome]: Battleship SUNK due to cumulative damage.\n");
    } else {
        printf("\n[Outcome]: Battleship SURVIVED.\n");
        printf("  -> Final Battleship Health: %.2f%%\n", B.health);
        printf("  -> Total Escort ships destroyed: %d / %d\n", destroyed_count, n);
    }

    free(ships);
}
