#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "battleship.h"

void runPart1A() {
    struct Battleship B;
    int n;

    printf("\n--- Part 1-A: Naval Battlefield Setup ---\n");
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

    FILE *init_fp = fopen("part1a_output.txt", "w");
    if (init_fp != NULL) {
        fprintf(init_fp, "========================================\n");
        fprintf(init_fp, "        PART 1-A SIMULATION LOG         \n");
        fprintf(init_fp, "========================================\n");
        fprintf(init_fp, "Battleship -> Type: %c | Position: (%.2f, %.2f) | Max Velocity: %.2f\n\n", B.type, B.x, B.y, B.velocity);
        fprintf(init_fp, "Initial Escort Ships Configuration (Total: %d):\n", n);

        for (int i = 0; i < n; i++) {
            float rad = ships[i].angle * (PI / 180.0);
            float range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;
            fprintf(init_fp, "  - ID: %d | Type: %s | Pos: (%.2f, %.2f) | Max V: %.2f | Min Ang: %.2f | Range: %.2fm\n", 
                    ships[i].id, ships[i].type, ships[i].x, ships[i].y, ships[i].velocity, ships[i].minAngle, range);
        }
        fprintf(init_fp, "\n----------------------------------------\n");
        fprintf(init_fp, "Simulation Results & Combat Engagement:\n");
    }

    printf("\n--- Processing Combat Engagement ---\n");
    float b_max_range = (B.velocity * B.velocity) / G;
    int b_sunk = 0;
    int hit_count = 0;
    int sinking_escort_id = -1;

    for (int i = 0; i < n; i++) {
        float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
        float rad = ships[i].angle * (PI / 180.0);
        float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

        if (distance <= b_max_range) {
            hit_count++;
            ships[i].health = 0.0;
            printf("  -> [Hit]: Escort %s (ID: %d) destroyed at Distance: %.2fm\n", ships[i].type, ships[i].id, distance);
            if (init_fp) fprintf(init_fp, "  -> Escort ID: %d [%s] HIT and destroyed at Distance %.2fm\n", ships[i].id, ships[i].type, distance);
        } else {
            if (init_fp) fprintf(init_fp, "  -> Escort ID: %d [%s] OUT OF RANGE at Distance %.2fm\n", ships[i].id, ships[i].type, distance);
        }

        if (distance <= escort_range) {
            b_sunk = 1;
            sinking_escort_id = ships[i].id;
            printf("  -> [Danger]: Battleship hit by Escort %s (ID: %d) and SUNK!\n", ships[i].type, ships[i].id);
            if (init_fp) fprintf(init_fp, "  -> [CRITICAL]: Battleship was SUNK by Escort ID: %d [%s]!\n", ships[i].id, ships[i].type);
        }
    }

    if (init_fp) {
        fprintf(init_fp, "\n----------------------------------------\n");
        if (b_sunk) {
            fprintf(init_fp, "Final Outcome: Battleship is GOING TO SINK (Sunk by Escort ID: %d)\n", sinking_escort_id);
        } else {
            fprintf(init_fp, "Final Outcome: Battleship SURVIVED. Total E ships hit: %d\n", hit_count);
            fprintf(init_fp, "Battle Duration: 0 seconds (Instantaneous firing assumed in Part 1-A)\n");
        }
        fclose(init_fp);
        printf("\n[Success]: Detailed results saved to 'part1a_output.txt'\n");
    }

    if (b_sunk) {
        printf("\n[Outcome]: Battleship is GOING TO SINK.\n");
        printf("  -> Sunk by Escort Ship ID: %d\n", sinking_escort_id);
    } else {
        printf("\n[Outcome]: Battleship SURVIVED.\n");
        printf("  -> Total E ships hit by Battleship: %d\n", hit_count);
        printf("  -> Time to end battle: 0 seconds\n");
    }

    free(ships);
}