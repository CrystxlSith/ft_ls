#include "ft_ls.h"

void fill_tab(t_list **entries, int a, const char *str) {
    DIR *dir;
    dir = opendir(str);
    struct dirent *de;
    int i = 0;
    while ((de = readdir(dir)) != NULL) {
        if (!a && de->d_name[0] == '.')
        continue;
        ft_lstadd_back(entries, ft_lstnew(ft_strdup(de->d_name)));
    }

    
    closedir(dir);
    i++;
}

void a_filter(t_list **entries, t_options *opts) {

   
    

    int i = 0;
    struct stat st;
    if (opts->paths[0]) {
        while(opts->paths[i]) {

            if (lstat(opts->paths[i], &st) < 0) {
                perror(opts->paths[i]);
            }
            else if (S_ISDIR(st.st_mode)) {
                fill_tab(entries, opts->a, opts->paths[i]);
            }
            else {
                ft_lstadd_back(entries, ft_lstnew(ft_strdup(opts->paths[i])));
            }
            i++;
        }
    }
    else {
        fill_tab(entries, opts->a ,"./");
    }

}

void    list_directories(t_options *opts) {
    // char **str;
    t_list *entries = NULL;

    a_filter(&entries, opts);
    t_list *tmp = entries;
    while (tmp)
    {
        
        ft_printf("%s\n", (char *)tmp->content);
        tmp = tmp->next;
    }
    ft_lstclear(&entries, free);
}