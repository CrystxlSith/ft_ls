#include "ft_ls.h"

void	fill_tab(t_list **entries, int a, const char *str) {
	DIR				*dir;
	struct dirent	*de;

	dir = opendir(str);
	while ((de = readdir(dir)) != NULL) {
		if (!a && de->d_name[0] == '.')
			continue;
		ft_lstadd_back(entries, ft_lstnew(new_entry(de->d_name, str)));
	}
	closedir(dir);
}

void	a_filter(t_list **entries, t_options *opts) {
	int			i;
	struct stat	st;

	i = 0;
	if (opts->paths[0]) {
		while (opts->paths[i]) {
			if (lstat(opts->paths[i], &st) < 0)
				perror(opts->paths[i]);
			else if (S_ISDIR(st.st_mode))
				fill_tab(entries, opts->a, opts->paths[i]);
			else
				ft_lstadd_back(entries,
					ft_lstnew(new_entry(opts->paths[i], ".")));
			i++;
		}
	}
	else
		fill_tab(entries, opts->a, "./");
}

void	list_directories(t_options *opts) {
	t_list	*entries;
	t_list	*tmp;
	int		max_nlink;
	int		max_size;

	entries = NULL;
	a_filter(&entries, opts);
	entries_sort(&entries, opts);
	if (opts->l)
		get_max_widths(entries, &max_nlink, &max_size);
	tmp = entries;
	while (tmp)
	{
		print_entry((t_entry *)tmp->content, opts, max_nlink, max_size);
		tmp = tmp->next;
	}
	ft_lstclear(&entries, free_entry);
}
