#include "s21_cat_lib.h"

int main(int argc, char *argv[]) {
    options opt = {0};             // инициализация и зануление структуры опций
    if (parser(argc, argv, &opt)) {
        printf("s21_cat: invalid options, -benstv usage.\n");
    } else {
        argv += optind;
        fileReader(argv, opt);
    }
    return 0;
}
