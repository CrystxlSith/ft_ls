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

void	print_entries(t_list *entries, t_options *opts) {
	int	max_nlink;
	int	max_size;

	if (opts->l)
		get_max_widths(entries, &max_nlink, &max_size);
	while (entries) {
		print_entry((t_entry *)entries->content, opts, max_nlink, max_size);
		entries = entries->next;
	}
}

void	list_one_dir(const char *path, t_options *opts, int print_header) {
	t_list	*entries;
	t_list	*tmp;
	t_entry	*e;

	entries = NULL;
	if (print_header)
		ft_printf("%s:\n", path);
	fill_tab(&entries, opts->a, path);
	entries_sort(&entries, opts);
	if (opts->l) {
		long total = 0;
		tmp = entries;
		while (tmp) {
			total += ((t_entry *)tmp->content)->st.st_blocks;
			tmp = tmp->next;
		}
		ft_printf("total %d\n", total / 2);
	}
	print_entries(entries, opts);
	if (opts->R) {
		tmp = entries;
		while (tmp) {
			e = (t_entry *)tmp->content;
			if (S_ISDIR(e->st.st_mode)
				&& ft_strncmp(e->name, ".", 2) != 0
				&& ft_strncmp(e->name, "..", 3) != 0) {
				ft_printf("\n");
				list_one_dir(e->path, opts, 1);
			}
			tmp = tmp->next;
		}
	}
	ft_lstclear(&entries, free_entry);
}

void	list_directories(t_options *opts) {
	int			i;
	int			count;
	int			first;
	struct stat	st;
	t_list		*file_entries;

	if (!opts->paths[0]) {
		list_one_dir(".", opts, opts->R);
		return ;
	}
	count = 0;
	while (opts->paths[count])
		count++;
	file_entries = NULL;
	first = 1;
	i = 0;
	while (opts->paths[i]) {
		if (lstat(opts->paths[i], &st) < 0) {
			ft_putstr_fd("ft_ls: cannot access '", 2);
			ft_putstr_fd((char *)opts->paths[i], 2);
			ft_putstr_fd("': ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
		}
		else if (!S_ISDIR(st.st_mode))
			ft_lstadd_back(&file_entries,
				ft_lstnew(new_entry(opts->paths[i], ".")));
		i++;
	}
	if (file_entries) {
		entries_sort(&file_entries, opts);
		print_entries(file_entries, opts);
		ft_lstclear(&file_entries, free_entry);
		first = 0;
	}
	i = 0;
	while (opts->paths[i]) {
		if (lstat(opts->paths[i], &st) >= 0 && S_ISDIR(st.st_mode)) {
			if (!first)
				ft_printf("\n");
			list_one_dir(opts->paths[i], opts, count > 1 || opts->R);
			first = 0;
		}
		i++;
	}
}
