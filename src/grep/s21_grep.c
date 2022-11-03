#include <stdio.h>
#include <getopt.h>
#include <pcre.h>


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
