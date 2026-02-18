#ifndef FT_LS_H
# define FT_LS_H

# define OPTIONS "lRrta"
# define NO_FILE ""
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
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

typedef struct s_entry
{
    char        *name;
    char        *path;
    struct stat st;
}   t_entry;

/** UTILS */
void	set_options(int c, t_options *opts);
void	parseArgs(int ac, char const *str[], t_options *opts);

/** ENTRY */
t_entry	*new_entry(const char *name, const char *dir_path);
void	free_entry(void *ptr);

/** SORT */
void	entries_sort(t_list **entries, t_options *opts);

/** DISPLAY */
int		num_len(long n);
void	print_pad(int width, int len);
void	print_permissions(unsigned int mode);
void	get_max_widths(t_list *entries, int *max_nlink, int *max_size);
void	print_entry(t_entry *e, t_options *opts, int max_nlink, int max_size);

/** LIST */
void	list_directories(t_options *opts);


#endif