#include <stdio.h>
#include <getopt.h>
#include <string.h>
// #include <pcre.h>



typedef struct {
    int e;              // шаблон
    int i;              // игнорирует наличие регистра
    int v;              // выводит строки, в которых образец не встречается 
    int c;              // выводит количество строк, в которых обнаружен образец
    int l;              // выводит имена файлов, содержащих образец
    int n;              // выводит строки содержащие образец с номерами
} options;

void parser(int argc, char *argv[], options *opt);

int main(int argc, char *argv[]) {
    options opt = {0};
    parser(argc, argv, &opt);
    if (opt.e == 0) {
        char *patt;
        patt = argv[optind];
        // ++optind;                       // переставляем указатель на первое имя файла
        printf("\tpattern: %s\n", patt);
        // работа с ключами по паттерну
    } else {
        char *pattern[opt.e];
        int e_count = 0;
        for (int i = 1; i < optind; ++i) {
            const char e_flag = 'e';
            char *pattern_temp;
            pattern_temp = strchr(argv[i], e_flag);
            int pattern_temp_length = strlen(pattern_temp);
            if (pattern_temp_length == 1) {
                pattern[e_count] = argv[i + 1];
                ++i;
            } else {
                pattern[e_count] = pattern_temp + 1;
            }
            ++e_count;
        }
        for (int i = 0; i < e_count; ++i) {
            printf("\tpattern #%d'e': %s\n", i, pattern[i]);
        }
    }


    printf("\tСчитано: e = %d; i = %d; v = %d; c = %d; l = %d; n = %d\n", opt.e, opt.i, opt.v, opt.c, opt.l, opt.n);
    printf("\toptind = %d\n", optind);
    for (int i = 0; i < argc; ++i) {
        printf("\tArgument №%d: %s\n", i, argv[i]);
    }
  
    return 0;
}

void parser(int argc, char *argv[], options *opt) {
    int rez = 0;
    while ((rez = getopt(argc, argv, "e:ivcln")) != -1) {
        switch (rez) {
        case 'e':
            opt->e++;
            break;
        case 'i':
            opt->i = 1;
            break;
        case 'v':
            opt->v = 1;
            break;
        case 'c':
            opt->c = 1;
            break;
        case 'l':
            opt->l = 1;
            break;
        case 'n':
            opt->n = 1;
            break;
        default:
            break;
        }
    }
}
