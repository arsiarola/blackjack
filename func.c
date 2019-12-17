#include "func.h"

void play() {
    Deck deck[DECK_COUNT];

    Hand dealer;
    int player_count = 0;
    Player players[MAX_PLAYER];

    FILE *log_file = fopen(LOG_FILE, "w");
    if (log_file == NULL) {
        printf("Error in opening log file\n");
    }

    else {
        init_game(players, &player_count, MIN_PLAYER, MAX_PLAYER, log_file);

        //main play loop
        do {
            // ask for bets or if one wants to leave table
            ask_for_bets(players, &player_count, MIN_BET, log_file);
            if (player_count < 1) {
                break;
            }

            // reset main deck
            reset_deck(deck);
            /* print_decks(deck, DECK_COUNT); */

            // deal two cards for each player and print them
            deal_starting_hands(players, player_count, deck, log_file);

            // reset dealer's hand and deal two cards but show only one
            reset_deal_dealer(&dealer, deck);

            // ask every player what to do, except if they hit blackjack
            for (int i = 0; i < player_count; i++) {
                if (!players[i].hands[0].bjack) {
                    ask_player(dealer, &players[i], deck, log_file);
                }
            }

            // dealer will hit until his total is at least 17
            dealers_turn(&dealer, deck, log_file);

            // check results for every player and pay bets accordingly
            for (int i = 0; i < player_count; i++) {
                player_results(&players[i], dealer);
                transfer_player_bets(&players[i], log_file);
            }

            // free every players and dealer's cards
            for (int i = 0; i < player_count; i++) {
                free_hands(&players[i]);
            }
            free(dealer.cards);

            // remove players if chip count is less than MIN_BET
            for (int i = 0; i < player_count; i++) {
                if (players[i].chips < MIN_BET) {
                    printf("%s has less than %d chips, removing from table\n", players[i].name, MIN_BET);
                    fprintf(log_file, "%s has less than %d chips, removing from table\n", players[i].name, MIN_BET);
                    remove_player(players, &player_count, i);
                    i--;
                }
            }
            fprintf(log_file,FILE_BREAK);

        } while (player_count > 0); // loop while player's still in table

        printf("All players left the table\n");
        fprintf(log_file, "All players left the table\n");
        fclose(log_file);
    }
}

void init_game(Player players[], int *size, int min_player, int max_player,  FILE *file) {
    // Get player count
    do {
        printf("Enter how many players (%d-%d): ", min_player, max_player);
    } while (!read_int(min_player, max_player, &*size));
    fprintf(file, "Player count entered: %d\n", *size);

    // initalize players
    for (int i = 0; i < *size; i++) {
        init_player(&players[i], i+1);
        fprintf(file, "%*s registered for %d chips\n", NAME_PADDING, players[i].name, START_CHIPS);
    }
    fprintf(file, FILE_BREAK);
}

void print_log(char *f_name) {
    FILE *log_file = fopen(f_name, "r");
    if (log_file == NULL) {
        printf("Error in opening %s\n", LOG_FILE);
    }

    else {
        char *line = malloc(sizeof(char) * LINE_SIZE);
        if (line == NULL) {
            printf("Error in allocating memroy\n");
            fclose(log_file);
        }

        else {
            while (!feof(log_file)) {
                printf("%s", read_line(log_file, line, LINE_SIZE, LINE_SPACE));
            }
            printf("\n");
            fclose(log_file);
        }
        free(line);
    }
}

// read a line from file and expand the line string if necessary
char *read_line(FILE *file, char *line, int line_size, int line_space) {
    strcpy(line, "");
    int length = 0;
    int size = line_size;
    bool is_end = false;

    do {
        length = strlen(line);
        if (fgets(line+length, size-length, file) == NULL) {
            return "";
        }

        if(line[strlen(line)-1] == '\n')
            is_end = true;

        else {
            size = size + line_space;
            line = realloc(line, sizeof(char) * (size + line_space));
            if(line == NULL) {
                printf("Error in reallocating\n");
            }
        }
    } while(!is_end);

    return line;
}

// if input cant be converted to integer return false
// or if input is not within min and max return false
// otherwise return true and replace num with the input value
bool read_int(int min, int max, int *num) {
    int size = 50;
    char string[size];
    int temp;
    read_string(string, size);

    // sscanf has to be checked first if the input even is integer
    if (sscanf(string, " %d", &temp) != 1 || temp < min || temp > max ) {
        return false;
    }

    *num = temp;
    return true;
}

void delay(int number_of_seconds) {
    int milli_seconds = number_of_seconds * 1000 * 1000;
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}

// generate a random number between min and max
// works also if min is greater value than max
int random_number(const int min, const int max) {
    if (max < min) {
        return (rand() % (min-max+1)) + max;
    }
    return (rand() % (max-min+1)) + min;
}

// read a string, if input doesn't fit into string, clear input buffer
// return true if the input fit into the string
bool read_string(char *string, int size) {
    fgets(string, size ,stdin);
    if(!strchr(string, '\n')) {     //newline does not exist
        while(fgetc(stdin)!='\n');  //discard until newline aka clean stdin
        return false;
    }

    fix_string(string);
    return true;
}

// remove newline from string end
void fix_string(char *string) {
    if (string[strlen(string)-1] == '\n') string[strlen(string)-1] = '\0';
    if (string[strlen(string)-1] == '\r') string[strlen(string)-1] = '\0';
}


int lowest_in_array(const int array[], int size) {
    if (size < 1) {
        return 0;
    }

    int lowest = array[0];
    for (int i = 0; i < size; ++i) {
        if (array[i] < lowest) {
            lowest = array[i];
        }
    }
    return lowest;
}

int highest_in_array(const int array[], int size) {
    if (size < 1) {
        return 0;
    }

    int highest = array[0];
    for (int i = 0; i < size; ++i) {
        if (array[i] > highest) {
            highest = array[i];
        }
    }
    return highest;
}
