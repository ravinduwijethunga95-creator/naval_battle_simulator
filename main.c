#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "battleship.h"

int main() {
    srand(time(NULL));
    int choice;

    do {
        printf("\n=========================================\n");
        printf("    NAVAL BATTLE SIMULATOR (MULTI-FILE)  \n");
        printf("=========================================\n");
        printf("1. Start Simulation\n");
        printf("2. View Instructions\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int sub_choice;
                printf("\n--- Select Simulation Part ---\n");
                printf("1. Part 1-A (Basic Setup & File Logging)\n");
                printf("2. Part 1-B Sim 1 (Path Movement)\n");
                printf("3. Part 1-B Sim 2 (Gun Jam)\n");
                printf("4. Part 1-C (1-A Cumulative Impact)\n");
                printf("5. Part 1-C (Sim 1 Cumulative Path)\n");
                printf("6. Part 1-C (Sim 2 Cumulative Gun Jam)\n");
                printf("7. Part 2-A (Attack Order & Firing Interval)\n");
                printf("8. Part 2-B (Dynamic Path & Timing)\n");
                printf("Enter choice: ");
                scanf("%d", &sub_choice);

                switch(sub_choice) {
                    case 1: runPart1A(); break;
                    case 2: runPart1B_Sim1(); break;
                    case 3: runPart1B_Sim2(); break;
                    case 4: runPart1C_1A(); break;
                    case 5: runPart1C_1B_Sim1(); break;
                    case 6: runPart1C_1B_Sim2(); break;
                    case 7: runPart2A(); break;
                    case 8: runPart2B(); break;
                    default: printf("Invalid sub-choice!\n");
                }
                break;
            }
            case 2:
                printf("\n=================================================================\n");
                printf("                     SIMULATION INSTRUCTIONS                     \n");
                printf("=================================================================\n");
                printf(" Part 1-A : Basic combat setup, single-shot destruction, and logs.\n");
                printf(" Part 1-B1: Escorts & Battleship moving along generated path points.\n");
                printf(" Part 1-B2: Path movement with a Gun Jam threshold (restricted angles).\n");
                printf(" Part 1-C1: Part 1-A setup upgraded with Cumulative Health Damage.\n");
                printf(" Part 1-C2: Path movement simulation with Cumulative Health Damage.\n");
                printf(" Part 1-C3: Gun Jam simulation with Cumulative Health Damage.\n");
                printf(" Part 2-A : Attack Order sorting & Firing Interval (T_B^q) delays.\n");
                printf(" Part 2-B : Dynamic path movement, gun jams, and timing coordination.\n");
                printf("-----------------------------------------------------------------\n");
                printf(" Note: All outputs are automatically logged into separate text files.\n");
                printf("=================================================================\n");
                break;
            case 3:
                printf("\nExiting simulator. Good luck!\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 3);

    return 0;
}
