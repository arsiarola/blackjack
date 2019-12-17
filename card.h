#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "func.h"
#include "player.h"

void deal_card(Hand *hand, Deck deck[]);
void remove_card_deck(Deck deck[], int deck_index, int card_index);
void remove_card_hand(Card *deck, int *size, int index);
void reset_deck(Deck deck[]);
void shuffle_cards(Card cards[], int size);

int cards_value(const Card deck[], const int size);
void check_blackjack(Hand *hand, char *name, FILE *file);

void print_cards(Hand hand);
void print_last_card(Hand hand);
void print_first_card(Hand hand);
void print_a_card(int suit, int num);
void print_empty_card();

void hit_player_hand(Player *p, int index, Deck deck[], FILE *file);
void hit_hand(Hand *dealer, Deck deck[]);
void print_hand_total(Player *p, int index, FILE *file);
void double_hand(Player *p);
void split_hand(Player *p, int id);
void reset_hand(Hand *hand);
void reset_hands(Player *p);
void free_hands(Player *p);

void print_decks(Deck deck[], int size);
int decks_size(Deck deck[], int size);

const char *suit_to_str_mcu(int num);
const char *suit_to_str(int num);
const char *num_to_str(int num);
int num_to_value(int num);

#endif
