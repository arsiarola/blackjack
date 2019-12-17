#ifndef FUNC_H
#define FUNC_H

#include "main.h"

void play();
void init_game(Player players[], int *size, int min_player, int max_player,  FILE *file);

void print_log(char *f_name);
char *read_line(FILE *file, char *line, int line_size, int line_space);
bool read_int(int min, int max, int *num);
void delay(int number_of_seconds);
int random_number(const int min, const int max);
bool read_string(char *string, int size);
void fix_string(char *string);
int lowest_in_array(const int array[], int size);
int highest_in_array(const int array[], int size);

#endif

