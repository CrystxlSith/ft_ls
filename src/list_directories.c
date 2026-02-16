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
    t_list *tmp = *entries;
    while (tmp)
    {
        
        ft_printf("%s\n", (char *)tmp->content);
        tmp = tmp->next;
    }
    
    closedir(dir);
    i++;
}

void a_filter(t_list **entries, t_options *opts) {

   
    

    
    int i = 0;
    if (opts->paths[0]) {
        while(opts->paths[i]) {
            fill_tab(entries, opts->a, opts->paths[i]);
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
    ft_lstclear(&entries, free);
}