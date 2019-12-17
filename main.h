#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "log_file.txt"
#define LINE_SIZE 128
#define LINE_SPACE 64

#define DECK_SIZE 52
#define DECK_COUNT 6

#define MAX_SUIT 4
#define MIN_SUIT 1

#define MAX_NUM 13
#define NUM_MIN 1

#define START_CHIPS 100
#define MIN_BET 25

#define MAX_HANDS 2
#define MAX_PLAYER 7
#define MIN_PLAYER 1

#define BJACK 3
#define WIN 2
#define TIE 1
#define LOSS 0

#define HIT 1
#define STAND 2
#define DOUBLE 3
#define SPLIT 4

#define PLAY 1
#define PRINT 2
#define QUIT 3

#define DELAY 2

#define NAME_LENGTH 16
#define NAME_PADDING -15

#define CARD_FORMAT "|%*s|%*s|"
#define NUM_PADDING 2
#define SUIT_PADDING -1

#define FILE_BREAK  "____________________________________\n\n"

typedef struct card {
    int suit;
    int num;
} Card;

typedef struct deck {
    Card cards[DECK_SIZE];
    int count;
} Deck;

typedef struct hand {
    int num;
    Card *cards;
    int count;
    int bet;
    bool bjack;
    bool over;
    bool dbl;
    int result;
} Hand;

typedef struct player {
    char name[NAME_LENGTH];
    int chips;

    Hand hands[MAX_HANDS];
    int hand_count;
} Player;

#include "func.h"
#include "player.h"
#include "card.h"

#endif
