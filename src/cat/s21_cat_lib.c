#ifndef S21_CAT_LIB_C
#define S21_CAT_LIB_C

#include "s21_cat_lib.h"

int parser(int argc, char *argv[], options *opt) {
    int err = 0;
    int rez = 0;
    int option_index;
    const struct option long_options[] = {                  // структура для соответствия длинных параметров коротким
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {NULL, 0, NULL, 0}
    };
    while ((rez = getopt_long(argc, argv, "+benstvET", long_options, &option_index)) != -1) {
        switch(rez) {                   // если список коротких опций "benstvET", опции будут считываться из любого места как в linux
            case 'b':
                opt->b = 1;
                break;
            case 'e':
                opt->v = 1;
                opt->e = 1;
                break;
            case 'n':
                opt->n = 1;
                break;
            case 's':
                opt->s = 1;
                break;
            case 't':
                opt->v = 1;
                opt->t = 1;
                break;
            case 'v':
                opt->v = 1;
                break;
            case 'T':
                opt->t = 1;
                break;
            case 'E':
                opt->e = 1;
                break;
            default:
                err = 1;        // если встретятся 'кривые' флаги
                break;
        }
    }
    return err;
}

void fileReader(char *argv[], options opt) {
    int i = 0;
    int cooked = opt.b + opt.e + opt.n + opt.s + opt.t + opt.v;
    char *path;
    FILE *f;
    while ((path = argv[i]) != NULL || i == 0) {
        int fd;
        if (path == NULL || strcmp(path, "-") == 0) {
            fd = STDIN_FILENO;
        } else {
            fd = open(path, O_RDONLY);
        }
        if (fd < 0) {
            printf("s21_cat: %s: No such file or directory\n", argv[i]);
        } else if (cooked) {
            if (fd == STDIN_FILENO) {
                cook_cat(stdin, opt);
            } else {
                f = fdopen(fd, "r");
                cook_cat(f, opt);
                fclose(f);
            }
        } else {
            raw_cat(fd);
            if (fd != STDIN_FILENO) {
                close(fd);
            }
        }
        if (path == NULL)
            break;
        ++i;
    }
}

void cook_cat(FILE *f, options opt) {
    int ch, gobble, line, prev;
    line = gobble = 0;
    for (prev = '\n'; (ch = getc(f)) != EOF; prev = ch) {
        if (prev == '\n') {
            if (opt.s) {                                    // флаг s
                if (ch == '\n') {
                    if (gobble) {
                        continue;
                    }
                    gobble = 1;
                } else {
                    gobble = 0;
                }
            }
            if (opt.n && (!opt.b || ch != '\n')) {          // флаг n
                (void)fprintf(stdout, "%6d\t", ++line);
            } else if (opt.b && ch != '\n') {               // флаг b
                (void)fprintf(stdout, "%6d\t", ++line);
            }
        }
        if (ch == '\n') {
            if (opt.e && putchar('$') == EOF)       // флаг e
                break;                              // putchar возвращает EOF при неудаче
        } else if (ch == '\t') {                    // флаг t
            if (opt.t) {
                if (putchar('^') == EOF || putchar('I') == EOF)
                    break;
                continue;
            }
        } else if (opt.v) {                         // флаг v
            if (!isascii(ch) && !isprint(ch)) {
				if (putchar('M') == EOF || putchar('-') == EOF)
					break;
				ch = toascii(ch);
			}
			if (iscntrl(ch)) {
				if (putchar('^') == EOF || putchar(ch == '\177' ? '?' : ch | 0100) == EOF)
					break;
				continue;
			}
        }
        if (putchar(ch) == EOF)
            break;
    }
}

void raw_cat(int fd) {
    char buf[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buf, 4096))) {
        write(STDOUT_FILENO, buf, bytes_read);
    }
}

#endif  // S21_CAT_LIB_C
