#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

/*to remove flicker*/
void clear_screen_fast() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(h, pos);
}

int main() {
    srand(time(0));
    char playAgain;

    do {
        int x = 1;              // player position
        int score = 0;          // Score counter
        int lanes[10];          // Obstacle lanes
        
        // Initialize lanes
        for(int i=0; i<10; i++) lanes[i] = -1; 

        int spawnTimer = 0;     
        int gameTick = 0;       // Counts frames to control speed

        while (1) {

            // ---- INPUT (Checked every 50ms for smooth feel) ----
            if (_kbhit()) {
                char ch = getch();
                if (ch == 75 && x > 0) x--;       // LEFT
                if (ch == 77 && x < 2) x++;       // RIGHT
            }

            // ---- DRAW (Updates every 50ms) ----
            clear_screen_fast(); 
            printf("SCORE: %d\n", score);
            printf("|--- --- ---|\n");

            for (int i = 0; i < 10; i++) {
                if (lanes[i] == 0)      printf("| X         |\n");
                else if (lanes[i] == 1) printf("|     X     |\n");
                else if (lanes[i] == 2) printf("|         X |\n");
                else                    printf("|           |\n");
            }

            if (x == 0)      printf("| O         |\n");
            else if (x == 1) printf("|     O     |\n");
            else if (x == 2) printf("|         O |\n");

            // ---- COLLISION CHECK (Checked constantly) ----
            if (lanes[9] != -1 && lanes[9] == x) {
                printf("\nGAME OVER! Final Score: %d\n", score);
                printf("Play Again? (y/n): ");
                break;
            }

            // ---- GAME LOGIC (Only runs every 4 ticks) ----
            // 50ms * 4 = 200ms (Same speed as before, but smoother input)
            gameTick++; 
            if (gameTick >= 4) {
                gameTick = 0; // Reset tick counter

                // Add Score
                if (lanes[9] != -1) score++;

                // Move obstacles down
                for (int i = 9; i > 0; i--) {
                    lanes[i] = lanes[i - 1];
                }

                // Spawn new obstacles
                spawnTimer++;
                if (spawnTimer > 2) {
                    lanes[0] = rand() % 3;
                    spawnTimer = 0;
                } else {
                    lanes[0] = -1;
                }
            }

            // Short sleep for high responsiveness
            Sleep(50); 
        }

        playAgain = getch();

    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}
