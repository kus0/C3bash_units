#ifndef S21_CAT_LIB_H
#define S21_CAT_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef struct {
        int b;          // нумерует непустые строки
        int e;          // показывает непечатаемые символы плюс символ конца строки '$'; -E то же, но без -v
        int n;          // нумерует все строки
        int s;          // сжимает пустые строки
        int t;          // показывает непечатаемые символы плюс символ табуляции '^I'; -T то же, но без -v
        int v;          // показывает непечатаемые символы кроме табуляции и конца строки
    } options;

int parser(int argc, char *argv[], options *opt);
void fileReader(char *argv[], options opt);
void cook_cat(FILE *f, options opt);
void raw_cat(int fd);

#endif  // S21_CAT_LIB_H
