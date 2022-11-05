#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcre.h>

typedef struct {
    int e;              // шаблон
    int i;              // игнорирует наличие регистра
    int v;              // выводит строки, в которых образец не встречается 
    int c;              // выводит количество строк, в которых обнаружен образец
    int l;              // выводит имена файлов, содержащих образец
    int n;              // выводит строки содержащие образец с номерами
} options;

int opt_parser(int argc, char *argv[], options *opt);

int main(int argc, char *argv[]) {
    options opt = {0};
    if (opt_parser(argc, argv, &opt)) {
        printf("\tError flags!\n");
    } else {
        char *pattern[argc];            // массив для хранения шаблонов поиска
        int exc[opt.e];                 // массив для хранения номеров шаблонов для их исключения из имён файлов
        
        if (opt.e == 0) {
            for (int i = 1; i < argc; i++) {
                if (argv[i][0] != '-') {
                    pattern[0] = argv[i];
                    break;
                }
            }
        } else {
            int exc_counter = 0;
            int e_pat_count = 0;
            for (int i = 1; i < argc; i++) {
                if (argv[i][0] == '-') {
                    const char e_flag = 'e';
                    char *pattern_temp;
                    pattern_temp = strchr(argv[i], e_flag);
                    if (pattern_temp != NULL) {
                        int pattern_temp_length = strlen(pattern_temp);
                        if (pattern_temp_length == 1) {
                            ++i;
                            exc[exc_counter] = i;
                            ++exc_counter;
                            pattern[e_pat_count] = argv[i];
                            ++e_pat_count;
                            continue;
                        } else {
                            pattern[e_pat_count] = pattern_temp + 1;
                            ++e_pat_count;
                            exc[exc_counter] = 0;
                            ++exc_counter;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < opt.e; ++i) {
            if (pattern[i] == NULL) {
                printf("option requires an argument -- e\n");
                exit(1);
            }
        }


        if (opt.e == 0) {
            printf("\t\tшаблон: %s\n", pattern[0]);
        } else {
            for (int i = 0; i < opt.e; ++i) {
                printf("\t\tномер аргумента для исключения: %d\n", exc[i]);
            }
        }

        for (int i = 0; i < opt.e; ++i) {
            printf("\t\tномер аргумента для исключения: %d\n", exc[i]);
        }
    }

    printf("\tСчитано: e = %d; i = %d; v = %d; c = %d; l = %d; n = %d\n", opt.e, opt.i, opt.v, opt.c, opt.l, opt.n);
    for (int i = 0; i < argc; ++i) {
        printf("\tArgument №%d: %s\n", i, argv[i]);
    }
    return 0;
}

int opt_parser(int argc, char *argv[], options *opt) {
    int err = 0;
    for (int i = 1; i < argc; i++) {        // i - для пересчёта аргументов
        char *scan = argv[i];
        if (scan[0] == '-') {
            size_t j = 1;
            while (j < strlen(scan)) {      // j - для пересчёта символов аргумента
                char c = scan[j];
                switch (c) {
                case 'e':
                    if ((argv[i-2] != NULL) && (!strcmp(argv[i-1], "-e")) && (!strcmp(argv[i-2], "-e"))) {
                        break;              // чтобы не увеличивать счётчик е, если шаблон -e это '-e'
                    } else {
                        ++opt->e;
                        j = strlen(scan);
                        break;
                    }
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
                    if ((strchr(argv[i - 1], 'e') != NULL) && (strlen(strchr(argv[i - 1], 'e')) == 1)) {
                        break;      // чтобы не было ошибки, если шаблон -е начинается с '-'
                    } else {
                        ++err;
                        break;
                    }
                }
                ++j;
            }
        }
    }
    return err;
}



