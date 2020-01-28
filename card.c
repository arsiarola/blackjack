#include "card.h"

// deal one card and check if player went over
void deal_card(Hand *hand, Deck deck[]) {
    int deck_index = random_number(0, DECK_COUNT-1);
    int deck_size = deck[deck_index].count;

    // if half of the cards have been used, reset all the decks
    if (decks_size(deck, DECK_COUNT) < (DECK_SIZE * DECK_COUNT) / 2) {
        printf("Resetting decks\n");
        reset_deck(deck);
    }

    delay(DELAY); // delay so everything doesn't happen instantly

    hand->cards = realloc(hand->cards, sizeof(Card) * (hand->count+1));
    if (hand->cards == NULL) {
        printf("Error in allocating memroy\n");
    }

    else {
        int new_index = hand->count;
        int card_index = deck_size - 1;
        hand->count++;

        hand->cards[new_index].suit = deck[deck_index].cards[card_index].suit;
        hand->cards[new_index].num = deck[deck_index].cards[card_index].num;
        if (cards_value(hand->cards, hand->count) > 21) {
            hand->over = true;
        }

        remove_card_deck(deck, deck_index, card_index);
    }
}

// remove card from deck
void remove_card_deck(Deck deck[], int deck_index, int card_index) {
    int size = deck[deck_index].count;

    if (size > 1) {
        /* printf("removing: "); */
        /* print_a_card(deck[deck_index].cards[card_index].suit, deck[deck_index].cards[card_index].num); */
        /* printf("\n"); */
        for (int i = card_index; i < size-1; i++) {
            deck[deck_index].cards[i] = deck[deck_index].cards[i+1];
        }

        deck[deck_index].count -= 1;
    }

    else {
        printf("No cards to remove\n");
    }
}

// remove card from hand
void remove_card_hand(Card *deck, int *size, int index) {
    if (*size > 1) {
        for (int i = index; i < *size-1; i++) {
            deck[i] = deck[i+1];
        }

        *size = *size-1;
    }
    else {
        printf("No cards to remove\n");
    }
}

// deck will be reset and shuffled so there are suits from 1-4 and numbers from 1-13
// that is total of 52 cards
void reset_deck(Deck deck[]) {
    for (int a = 0; a < DECK_COUNT; a++) {
        for (int i = 0; i < MAX_SUIT; i++) {
            for (int j = 0; j < MAX_NUM; j++) {
                deck[a].cards[(i*MAX_NUM)+j].suit = i+1;
                deck[a].cards[(i*MAX_NUM)+j].num = j+1;
            }
        }

        deck[a].count = DECK_SIZE;
        shuffle_cards(deck[a].cards, DECK_SIZE);
    }
}

// shuffle the deck
void shuffle_cards(Card cards[], int size) {
    Card temp;
    int num = 0;
    for (int i = 0; i < size; i++) {
        num = random_number(0, size-1);

        temp = cards[i];
        cards[i] = cards[num];
        cards[num] = temp;
    }
}

// get the total value of cards
int cards_value(const Card deck[], const int size) {
    int total = 0;
    int ace_count = 0;
    int value = 0;

    // calculate every card with ace being 11 and count aces
    for (int i = 0; i < size; i++) {
        value = num_to_value(deck[i].num);
        total += value;
        if (value == 11) {
            ace_count++;
        }
    }

    // if total exceeds 21 and there were aces
    // make the aces from 11 to 1 one by one
    // until total is less than 21 there are no more aces
    while (total > 21 && ace_count > 0) {
        total -= 10;
        ace_count--;
    }

    return total;
}

// check for blackjack and print if was blackjack
void check_blackjack(Hand *hand, char *name, FILE *file) {
    if (cards_value(hand->cards, hand->count) == 21) {
        hand->bjack = true;
        printf("%*s hit a BlackJack!\n", NAME_PADDING, name);
        fprintf(file, "%*s hit a BlackJack!\n", NAME_PADDING, name);
    }
}

// print all cards in hand
void print_cards(Hand hand) {
    for (int i = 0; i < hand.count; i++) {
        if (i == hand.count-1) {
            print_a_card(hand.cards[i].suit, hand.cards[i].num);
        }

        else {
            print_a_card(hand.cards[i].suit, hand.cards[i].num);
            printf("\n");
        }
    }
}

// print all decks, used for testing purposes
void print_decks(Deck deck[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < deck[i].count; j++) {
            print_a_card(deck[i].cards[j].suit, deck[i].cards[j].num);
            printf("\n");
        }
    }
}

// print last card of the hand
void print_last_card(Hand hand) {
    print_a_card(hand.cards[hand.count-1].suit, hand.cards[hand.count-1].num);
}

// print first card of the hand
void print_first_card(Hand hand) {
    print_a_card(hand.cards[0].suit, hand.cards[0].num);
}

// print a card by suit and number
void print_a_card(int suit, int num) {
    printf(CARD_FORMAT,
            SUIT_PADDING, suit_to_str(suit),
            NUM_PADDING, num_to_str(num)
          );
}

// print empty card, (used for dealer's second card)
void print_empty_card() {
    printf(CARD_FORMAT,
            SUIT_PADDING, " ",
            NUM_PADDING, ""
          );
}

// double the hand bet and remove chips from player
void double_hand(Player *p) {
    int bet = p->hands[0].bet;

    p->hands[0].dbl = true;
    p->hands[0].bet = bet * 2;
    p->chips -= bet;
}

// split Player's hand by given index, into two hands
void split_hand(Player *p, int index) {
    int new_num = p->hands[p->hand_count-1].num + 1;
    int new_index  = p->hand_count;
    p->hands[new_index].cards = (Card *) malloc(sizeof(Card));
    if (p->hands[new_index].cards  == NULL) {
        printf("Couldn't allocate space for split hand, not splitting the hand\n");
    }

    else {
        // every hand has a number starting from 1, new number will be the last hand's number + 1
        p->hand_count++;

        reset_hand(&p->hands[new_index]); // reset any values in the new hand
        p->hands[new_index].cards[0] = p->hands[index].cards[1]; // new hand's card will be old_hand's first card
        p->hands[new_index].count++;
        p->hands[new_index].num = new_num;

        remove_card_hand(p->hands[index].cards, &p->hands[index].count, 1); // remove old_hand's first card

        // add the bet to new hand and remove chips from player
        int bet = p->hands[index].bet;
        p->hands[new_index].bet = bet;
        p->chips -= bet;
    }
}

// deal one card to player's given hand and print the card
void hit_player_hand(Player *p, int index, Deck deck[], FILE *file) {
    deal_card(&p->hands[index], deck);
    print_last_card(p->hands[index]);
}

// deal and print a card
void hit_hand(Hand *dealer, Deck deck[]) {
    deal_card(dealer, deck);
    print_last_card(*dealer);
    printf("\n");
}


// print the total value of player's hand by given hand index
void print_hand_total(Player *p, int index, FILE *file) {
    if (cards_value(p->hands[index].cards, p->hands[index].count) > 21) {;
        printf("\n");
        print_player_format(*p, index);
        print_player_format_file(*p, index, file);
        printf(" went over\n");
        fprintf(file, " went over\n");
    }

    else {
        print_player_format(*p, index);
        print_player_format_file(*p, index, file);
        printf(" total: %d\n", cards_value(p->hands[index].cards, p->hands[index].count));
        fprintf(file, " total: %d\n", cards_value(p->hands[index].cards, p->hands[index].count));
    }
}

// reset players every possible hands values
// because at the beginning there is garbage in these memory spots
// malloc space for first hand, so can be reallocated later
void reset_hands(Player *p) {
    for (int i = 0; i < MAX_HANDS; i++) {
        reset_hand(&p->hands[i]);
    }

    p->hands[0].cards = (Card *) malloc(sizeof(Card));
    p->hand_count = 1;
    p->hands[0].num = 1; // first hand will be numbered as 1
}

// reset every value in hand
void reset_hand(Hand *hand) {
    hand->count = 0;
    hand->bet = 0;
    hand->bjack = false;
    hand->over = false;
    hand->dbl = false;
    hand->result = 0;
}

// free player's every hand that had been allocated memory
// everytime player's hand_count is increased a memory allocation is made
// so we loop for player's hand_count
void free_hands(Player *p) {
    for (int i = 0; i < p->hand_count; i++) {
        free(p->hands[i].cards);
    }
}

const char *num_to_str(int num) {
    switch(num) {
        case 1:
            return "A";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        case 10:
            return "10";
        case 11:
            return "J";
        case 12:
            return "Q";
        case 13:
            return "K";

        default:
            return "";
    }
}

const char *suit_to_str(int num) {
    switch(num) {
        case 1:
            return "♠";
        case 2:
            return "♥";
        case 3:
            return "♦";
        case 4:
            return "♣";

        default:
            return "";
    }
}

const char *suit_to_str_mcu(int num) {
    switch(num) {
        case 1:
            return "spades";
        case 2:
            return "hearts";
        case 3:
            return "diamonds";
        case 4:
            return "clubs";

        default:
            return "";
    }
}

// as default ace will be returned as 11
int num_to_value(int num) {
    switch (num) {
        case 1:
            return 11;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 6;
        case 7:
            return 7;
        case 8:
            return 8;
        case 9:
            return 9;
        case 10:
            return 10;
        case 11:
            return 10;
        case 12:
            return 10;
        case 13:
            return 10;
    }

    return 0;
}

int decks_size(Deck deck[], int siz) {
    int count = 0;;
    for (int i = 0; i < siz; i++) {
        for (int j = 0; j < deck[i].count; j++) {
            count++;
        }
    }
    return count;
}
