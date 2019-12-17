#include "main.h"

int main(int argc, char *argv[]) {
    srand(time(0)); // required for random numbers
    bool quit = false;
    int option = 0;

    while (!quit) {
        printf("Enter one of the options below: \n"
                "%d. Play a game\n"
                "%d. Print log file\n"
                "%d. Quit\n",
                PLAY, PRINT, QUIT
              );
        while (!read_int(PLAY, QUIT, &option)) {
            printf("Number must be %d-%d: ", PLAY, QUIT);
        }

        switch (option) {
            case 1:
                play();
                break;

            case 2:
                print_log(LOG_FILE);
                break;

            case 3:
                quit = true;
                break;

            default:
                printf("Some sort of error happened\n");
                break;
        }
    }

    return 0;
}
