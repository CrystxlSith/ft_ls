#ifndef FT_LS_H
# define FT_LS_H

# define OPTIONS "lRrta"
# define NO_FILE ""
#include <dirent.h>
#include <sys/stat.h>
#include "libft/libft.h"




typedef struct s_options
{

    int R;
    int r;
    int l;
    int t;
    int a;

    char const **paths;

} t_options;

/** UTILS */
void    set_options(int c, t_options *opts);
void    parseArgs(int ac, char const *str[], t_options *opts);


void    list_directories(t_options *opts);


#endif