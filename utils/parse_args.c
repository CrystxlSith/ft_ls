#include "ft_ls.h"


void    set_options(int c, t_options *opts) {

    switch (c)
    {
        default:
            ft_printf("ft_ls: illegal option -- %c\n", c);
            free(opts->paths);
            exit(1);
        case 'l': opts->l = 1; break;
        case 'R': opts->R = 1; break;
        case 'r': opts->r = 1; break;
        case 't': opts->t = 1; break;
        case 'a': opts->a = 1; break;
    }
}

void    parseArgs(int ac, char const *str[], t_options *opts) {
    
    ft_memset(opts, 0, sizeof(t_options));
    opts->paths = malloc(sizeof(char *) * ac);
    int i = 1;
    int j;
    int p = 0;
    while (str[i])
    {
        if (str[i][0] == '-') {
            j = 1;
            while (str[i][j])
            {
                set_options(str[i][j], opts);
               j++;
            }
            
        }
        else {
            opts->paths[p] = str[i];
            p++;
        }

        i++;
    }
    if (p == 0)
        opts->paths[p] = ".";
    opts->paths[p] = NULL;
}