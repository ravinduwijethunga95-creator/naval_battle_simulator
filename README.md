# Naval Battle Simulator (C-Based Multi-File Simulation)

A comprehensive command-line tactical naval combat simulator developed in C. This project simulates tactical interactions, cumulative damage mechanics, path-based movement, and gun-jam constraints between a Battleship and an escort fleet.

## Project Structure

- `main.c`: Entry point providing an interactive menu system for all simulation modules.
- `battleship.h`: Header file containing core data structures (`Battleship`, `EscortShip`), simulation constants, and function prototypes.
- `functions.c`: Shared helper functions (distance calculations, escort initialization algorithms).
- **Module Files**: Dedicated source files for Part 1 (A, B, C) and Part 2 (A, B) covering basic combat, pathing, gun jams, and proximity-based prioritization.

## Key Features

1. **Cumulative Damage Mechanics**: Ships track health percentages (`100.0%`) and status flags (`isDestroyed`) rather than instant destruction.
2. **Tactical Pathing & Movement**: Ships and battleships move along dynamic 2D coordinate path points.
3. **Gun Jam Constraints**: Simulates weapon jams at designated steps, restricting effective firing angles (`theta_min_jam = 15.0°`) and maximum ballistic range.
4. **Proximity-Based Targeting**: Uses sorting algorithms (`qsort`) to prioritize targets based on distance.
5. **Persistent File Logging**: Automatically generates structured text logs (`.txt`) for individual simulation runs.

## Compilation and Execution (Ubuntu Linux)

Open your terminal in the project directory and run the following commands:

```bash
# Compile all source files together with the math library flag
gcc main.c functions.c part1a.c part1b.c part_1c_1a.c part_1c_1bsim1.c part_1c_1bsim2.c part_2a.c part_2b.c -o battleship_sim -lm

# Run the simulator
./battleship_sim
