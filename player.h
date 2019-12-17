#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "func.h"
#include "card.h"

void reset_deal_dealer(Hand *dealer, Deck deck[]);
void deal_starting_hands(Player players[], int player_count, Deck deck[], FILE *file);
void ask_for_bets(Player players[], int *player_count, const int min_bet,  FILE *file);

void init_player(Player *p, int index);
void remove_player(Player Players[], int *size, int index);
void ask_player(Hand dealer, Player *p, Deck deck[], FILE *file);
void ask_hands(Player *p, Deck deck[], FILE *file);

void dealers_turn(Hand *dealer, Deck deck[], FILE *file);
void player_results(Player *p, Hand dealer);
void transfer_player_bets(Player *p, FILE *file);

void print_player_format_file(const Player p, int index, FILE *file);
void print_player_format(const Player p, int index);
int ask_options(const int array[], int size);
int print_options(const int options[], int size);
const char* option_to_str(int option);
#endif
