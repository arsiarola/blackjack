#include "player.h"

// deal every player 2 cards, print them and check blackjack
void deal_starting_hands(Player players[], int player_count, Deck deck[], FILE *file) {
    for (int i = 0; i < player_count; i++) {
        printf("\n%s's hand: \n", players[i].name);
        hit_hand(&players[i].hands[0], deck);
        hit_hand(&players[i].hands[0], deck);

        check_blackjack(&players[i].hands[0], players[i].name, file);
    }
    printf("\n");
    fprintf(file, "\n");
}

// remove dealer's previous hand, deal 2 cards and print only the first one
void reset_deal_dealer(Hand *dealer, Deck deck[]) {
    reset_hand(dealer);
    dealer->cards = malloc(sizeof(Card));

    printf("Dealer's hand: \n");
    hit_hand(dealer, deck);
    print_empty_card();
    deal_card(dealer, deck);
    printf("\n");
}


// ask every player for bets or if they want to quit
void ask_for_bets(Player players[], int *player_count, const int min_bet,  FILE *file) {
    int bet = 0;
    bool left = false;
    for (int i = 0; i < *player_count; i++) {
        left = false;
        reset_hands(&players[i]);

        do {
            // ask for bet until a number
            do {
                printf("%s enter your bet (%d-%d) or enter 0 to quit: ",
                        players[i].name, min_bet, players[i].chips);
            } while (!read_int(0, players[i].chips, &bet));

            // free players's hands and remove the player
            if (bet == 0) {
                printf("%*s left the table\n", NAME_PADDING, players[i].name);
                fprintf(file, "%*s left the table\n", NAME_PADDING, players[i].name);

                left = true;
                free_hands(&players[i]);
                remove_player(players, player_count, i);
                i--;
                break;
            }
        } while(bet < MIN_BET);

        if (!left) {
            fprintf(file, "%*s bet %d\n", NAME_PADDING, players[i].name, bet);
            players[i].hands[0].bet = bet;
            players[i].chips -= bet;
        }
    }
    fprintf(file, "\n");
}

// give initial chips and ask player for his name
void init_player(Player *p, int index) {
    p->chips = START_CHIPS;

    printf("Player%d Enter your name (max length %d): ", index, NAME_LENGTH-1);
    char string[NAME_LENGTH] = "";

    while (!read_string(string, NAME_LENGTH)) {
        printf("Name was too long (max length is %d), try again: ", NAME_LENGTH-1);
    }
    strcpy(p->name ,string);
}

// remove player from the table
void remove_player(Player players[], int *size, int index) {
    int temp_size = *size;

    if (temp_size > 0) {
        for (int i = index; i < temp_size-1; i++) {
            players[i] = players[i+1];
        }

        *size = temp_size - 1;
    }
}

// this function comes after player has been dealt two cards
// if player has two cards with the same value and and has chips to double the bet
void ask_player(Hand dealer, Player *p, Deck deck[], FILE *file) {
    int option = 0;
    int num1 = num_to_value(p->hands[0].cards[0].num);
    int num2 = num_to_value(p->hands[0].cards[1].num);

    // print hit and stand no matter what
    printf("\n%s's turn | Dealer's card:", p->name);
    print_first_card(dealer);
    printf("\n");
    print_cards(p->hands[0]);

    // check if player has enough chips to double
    if (p->chips >= p->hands[0].bet) {

        // ask also for split if cards equal
        if (num1 == num2) {
            int options[4] = { HIT, STAND, DOUBLE, SPLIT};
            option = ask_options(options, 4);
        }

        else {
            int options[3] = { HIT, STAND, DOUBLE };
            option = ask_options(options, 3);
        }

        // bet will be doubled, only one card will be dealt
        if (option == DOUBLE) {
            fprintf(file, "%*s doubled down\n", NAME_PADDING, p->name);
            double_hand(p);
            hit_hand(&p->hands[0], deck);
            print_hand_total(p, 0, file);
        }

        // hand will be split into two different hands, that are played seperately
        else if (option == SPLIT) {
            split_hand(p, 0);
            fprintf(file, "%*s split hand\n", NAME_PADDING, p->name);
            printf("\n");
            ask_hands(p, deck, file);
        }

        else if (option == HIT) {
            hit_player_hand(p, 0, deck, file);
            ask_hands(p, deck, file);
        }

        else if (option == STAND) {
            print_hand_total(p, 0, file);
        }
    }

    // player didn't have enough chips for double or split
    else {
        int options[2] = { HIT, STAND};
        option = ask_options(options, 2);

        if (option == HIT) {
            hit_player_hand(p, 0, deck, file);
            ask_hands(p, deck, file);
        }

        else if (option == STAND) {
            print_hand_total(p, 0, file);
        }
    }
}

// ask player's every hand until he goes over or stops
void ask_hands(Player *p, Deck deck[], FILE *file) {
    for (int i = 0; i < p->hand_count; i++) {
        int option = 0;
        if (p->hand_count > 1) {
            print_cards(p->hands[i]);
        }

        while(option != 2 && !(p->hands[i].over)) {
            printf("   1.HIT, 2.STAND: ");

            while(!read_int(1, 2, &option));

            if (option == 1) {
                hit_player_hand(p, i, deck, file);
            }
        }
        print_hand_total(p, i, file);
    }
}

// dealers turn comes after every player has made their choices
// dealer will take cards until he hits at least 17
void dealers_turn(Hand *dealer, Deck deck[], FILE *file) {
    printf("\nDealer\n");
    print_cards(*dealer);
    printf("\n");
    check_blackjack(dealer, "Dealer", file);

    // deal cards to dealer until hits at least 17
    if (!dealer->bjack) {
        while (cards_value(dealer->cards, dealer->count) < 17) {
            hit_hand(dealer, deck);
        }

        if (cards_value(dealer->cards, dealer->count) > 21) {
            printf("Dealer went over\n");
            fprintf(file, "\nDealer went over\n\n");
        }

        else {
            printf("Dealer ended with total: %d\n", cards_value(dealer->cards, dealer->count));
            fprintf(file, "\nDealer ended with total: %d\n\n", cards_value(dealer->cards, dealer->count));
        }
    }

    else {
        printf("all hands lose except "
                "the ones that hit blackjack tie\n");
    }
    printf("\n");
}

// calculate the result for player
// possibilites: BJACK, WIN, TIE, LOSE
void player_results(Player *p, Hand dealer) {
    for (int i = 0; i < p->hand_count; i++) {
        int p_total = cards_value(p->hands[i].cards, p->hands[i].count);
        int d_total = cards_value(dealer.cards, dealer.count);
        int result = 0;

        if (p->hands[i].over) { //player over
            result = LOSS;
        }

        //have to check for blackjack before dealer over
        //because bigger win rate from winning with blackjack
        else if (p->hands[i].bjack && !dealer.bjack) { // player hit bjack, dealer didn't
            result = BJACK;
        }

        else if (dealer.over) { //dealer over
            result = WIN;
        }

        else if (p->hands[i].bjack && dealer.bjack) { //both hit bjack
            result = TIE;
        }

        // have to check because if dealer has blackjack and player doesn't
        // but player has 21 total: the player must still lose
        else if (!p->hands[i].bjack && dealer.bjack) { // dealer hit bjack, player didn't
            result = LOSS;
        }

        else  //player and dealer don't have bjack and didn't go over
        {
            if (p_total > d_total) {
                result = WIN;
            }

            else if (p_total == d_total) {
                result = TIE;
            }

            else {
                result = LOSS;
            }
        }

        p->hands[i].result = result;
    }
}

// pay bets according to results and print the result
void transfer_player_bets(Player *p, FILE *file) {
    int result = 0;
    int bet = 0;

    for (int i = 0; i < p->hand_count; i++) {
        print_player_format(*p, i);
        print_player_format_file(*p, i, file);
        result = p->hands[i].result;
        bet = p->hands[i].bet;
        int payout = 0;;

        if (result == BJACK) {
            payout = 3*(bet * 2) /2;
            p->chips += payout;
            printf(" won %d with Blackjack\n", payout);
            fprintf(file," won %d with Blackjack\n", payout);

        }

        else if (result == WIN) {
            payout = bet * 2;
            p->chips += payout;
            printf(" won %d \n", payout);
            fprintf(file," won %d\n", payout);

        }
        else if (result == TIE) {
            payout = bet;
            p->chips += payout;
            printf(" tied %d \n", payout);
            fprintf(file," tied %d\n", payout);
        }

        else {
            printf(" lost %d \n", bet);
            fprintf(file," lost %d\n", bet);
        }
    }
    printf("\n");
}

// if player has only one hand print his name
// otherwise print name and the hand number
void print_player_format(const Player p, int index) {
    if (p.hand_count > 1) {
        printf("%*s (hand %d)", NAME_PADDING, p.name, p.hands[index].num);
    }
    else {
        printf("%*s", NAME_PADDING, p.name);
    }
}

// same as print_player_format but for printing to file
void print_player_format_file(const Player p, int index, FILE *file) {
    if (p.hand_count > 1) {
        fprintf(file, "%*s (hand %d)", NAME_PADDING, p.name, p.hands[index].num);
    }

    else {
        fprintf(file, "%*s", NAME_PADDING, p.name);
    }
}

int ask_options(const int options[], int size) {
    int option = 0;
    int lowest = lowest_in_array(options, size);
    int highest = highest_in_array(options, size);

    print_options(options, size);
    do {
        read_int(lowest, highest, &option);
    } while (option > highest || option < lowest);

    return option;
}

int print_options(const int options[], int size) {
    for (int i = 0; i < size; ++i) {
        if (i == size -1) {
            printf("  %d. %s: ", options[i], option_to_str(options[i]));
        }

        else {
            printf("  %d. %s, ", options[i], option_to_str(options[i]));
        }
    }
}

const char* option_to_str(int option) {
    switch(option) {
        case HIT:
            return "HIT";
        case STAND:
            return "STAND";
        case DOUBLE:
            return "DOUBLE";
        case SPLIT:
            return "SPLIT";
    }
    return "";
}
