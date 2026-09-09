#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "battleship.h"

void runPart2C() {
    struct Battleship B;
    int n, k, t;

    printf("\n--- Part 2-C: Impact Power Degradation Simulation (Power Law) ---\n");
    printf("Enter Battleship Type Notation (U, M, R, S): ");
    scanf(" %c", &B.type);
    printf("Enter Battleship Gamma (γ) degradation value: ");
    scanf("%f", &B.gamma);

    printf("Enter Initial Battleship X coordinate: ");
    scanf("%f", &B.x);
    printf("Enter Initial Battleship Y coordinate: ");
    scanf("%f", &B.y);
    printf("Enter Maximum Velocity of Battleship Shell (V_max): ");
    scanf("%f", &B.velocity);
    printf("Enter Battleship Firing Interval / Reload Time (T_B^q): ");
    scanf("%f", &B.firingInterval);
    
    // Part 2-C specific initializations
    B.health = 100.0;
    B.firingCount = 0;
    B.initialImpactPower = 1.0; // Baseline starting impact power
    B.currentImpactPower = B.initialImpactPower;

    printf("Enter Number of Escort Ships (N): ");
    scanf("%d", &n);
    printf("Enter Total Path Points (k): ");
    scanf("%d", &k);
    printf("Enter Gun Jam Iteration Threshold (t < %d): ", k);
    scanf("%d", &t);

    struct EscortShip *ships = (struct EscortShip *)malloc(n * sizeof(struct EscortShip));
    if (ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    initializeEscortShips(ships, n, B.velocity);
    for (int i = 0; i < n; i++) {
        ships[i].health = 100.0;
        ships[i].isDestroyed = 0;
        ships[i].firingCount = 0;
        
        // Assign default or user input Gamma for each escort type
        // Escort ship එකේ type එක අනුව initial impact power එක දෙමු
	float default_ip = 0.04;
	if (strcmp(ships[i].type, "EA") == 0) default_ip = 0.08;
	else if (strcmp(ships[i].type, "EB") == 0) default_ip = 0.06;
	else if (strcmp(ships[i].type, "EC") == 0) default_ip = 0.07;
	else if (strcmp(ships[i].type, "ED") == 0) default_ip = 0.05;
	else if (strcmp(ships[i].type, "EE") == 0) default_ip = 0.04;
	ships[i].initialImpactPower = default_ip;
        ships[i].currentImpactPower = ships[i].initialImpactPower;
        ships[i].gamma = 0.05; // Default relative gamma for escort ships if not separately inputted
    }

    float *path_x = (float *)malloc(k * sizeof(float));
    float *path_y = (float *)malloc(k * sizeof(float));
    for (int i = 0; i < k; i++) {
        path_x[i] = (rand() % 10000) / 10.0;
        path_y[i] = (rand() % 10000) / 10.0;
    }

    FILE *fp = fopen("part2c_output.txt", "w");
    if (fp) {
        fprintf(fp, "==================================================\n");
        fprintf(fp, "          PART 2-C SIMULATION LOG (DEGRADATION)   \n");
        fprintf(fp, "==================================================\n");
        fprintf(fp, "Battleship -> Type: %c | Gamma: %.4f | Initial IP: %.2f\n\n", 
                B.type, B.gamma, B.initialImpactPower);
    }

    float simulated_time = 0.0;
    float theta_min_jam = 15.0;
    int b_sunk = 0;

    for (int step = 0; step < k; step++) {
        B.x = path_x[step];
        B.y = path_y[step];
        simulated_time += B.firingInterval;

        int is_jammed = (step >= t);
        
        // Increment battleship firing count at each successful step action
        B.firingCount++;
        
        // Calculate degraded Impact Power using exponential decay formula: IPn = IP0 * exp(-gamma * n)
        B.currentImpactPower = B.initialImpactPower * exp(-B.gamma * B.firingCount);

        printf("\n[Step %d/%d | Time: %.1fs] Pos: (%.2f, %.2f) | B_IP: %.4f %s\n", 
               step + 1, k, simulated_time, B.x, B.y, B.currentImpactPower, is_jammed ? "[GUN JAMMED]" : "[NORMAL]");
        
        if (fp) {
            fprintf(fp, "\n--- Step %d (Time: %.1fs) Pos: (%.2f, %.2f) | B_IP: %.4f %s ---\n", 
                    step + 1, simulated_time, B.x, B.y, B.currentImpactPower, is_jammed ? "[JAMMED]" : "[NORMAL]");
        }

        float effective_angle = is_jammed ? theta_min_jam : 45.0;
        float rad_b = effective_angle * (PI / 180.0);
        float b_effective_range = (B.velocity * B.velocity * sin(2 * rad_b)) / G;

        for (int i = 0; i < n; i++) {
            if (ships[i].isDestroyed) continue;

            float distance = calculateDistance(B.x, B.y, ships[i].x, ships[i].y);
            float rad = ships[i].angle * (PI / 180.0);
            float escort_range = (ships[i].velocity * ships[i].velocity * sin(2 * rad)) / G;

            // Battleship attack with current degraded impact power
            if (distance <= b_effective_range) {
                // Damage scale based on degraded current impact power instead of fixed 25.0
                float b_damage = B.currentImpactPower * 100.0; 
                ships[i].health -= b_damage;
                if (ships[i].health < 0.0) ships[i].health = 0.0;

                printf("  -> Escort %s (ID: %d) hit! IP applied: %.4f | Health left: %.1f%%\n", 
                       ships[i].type, ships[i].id, B.currentImpactPower, ships[i].health);
                if (fp) {
                    fprintf(fp, "  -> Escort ID: %d [%s] hit | Applied IP: %.4f | Health Left: %.1f%%\n", 
                            ships[i].id, ships[i].type, B.currentImpactPower, ships[i].health);
                }

                if (ships[i].health <= 0.0) {
                    ships[i].isDestroyed = 1;
                    printf("     [Destroyed]: Escort ID: %d destroyed by degraded attack!\n", ships[i].id);
                    if (fp) fprintf(fp, "     [DESTROYED]: Escort ID: %d [%s]\n", ships[i].id, ships[i].type);
                }
            }

            // Escort counter-attack with its own degradation
            if (distance <= escort_range) {
                ships[i].firingCount++;
                ships[i].currentImpactPower = ships[i].initialImpactPower * exp(-ships[i].gamma * ships[i].firingCount);

                float damage = ships[i].currentImpactPower * 100.0;
                B.health -= damage;
                if (B.health < 0.0) B.health = 0.0;

                printf("  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Current IP: %.4f | Health Left: %.1f%%\n", 
                       ships[i].id, ships[i].type, ships[i].currentImpactPower, B.health);
                if (fp) {
                    fprintf(fp, "  -> [HIT ON B]: Hit by Escort ID: %d [%s] | Current IP: %.4f | Health Left: %.1f%%\n", 
                            ships[i].id, ships[i].type, ships[i].currentImpactPower, B.health);
                }
            }
        }

        if (B.health <= 0.0) {
            b_sunk = 1;
            printf("\n  -> [CRITICAL]: Battleship SUNK due to cumulative degradation damage at step %d!\n", step + 1);
            if (fp) fprintf(fp, "\n[CRITICAL]: Battleship SUNK at step %d!\n", step + 1);
            break;
        }
    }

    if (fp) {
        fprintf(fp, "\n--------------------------------------------------\n");
        if (b_sunk) {
            fprintf(fp, "Final Outcome: Battleship SUNK.\n");
        } else {
            fprintf(fp, "Final Outcome: Battleship SURVIVED. Final Health: %.2f%% | Final B_IP: %.4f\n", 
                    B.health, B.currentImpactPower);
        }
        fclose(fp);
        printf("\n[Success]: Part 2-C results saved to 'part2c_output.txt'\n");
    }

    free(ships);
    free(path_x);
    free(path_y);
}
