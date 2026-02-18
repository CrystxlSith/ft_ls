#include "ft_ls.h"

t_entry	*new_entry(const char *name, const char *dir_path) {
	t_entry	*entry;
	char	*full_path;
	char	*tmp;

	entry = malloc(sizeof(t_entry));
	entry->name = ft_strdup(name);
	full_path = ft_strjoin(dir_path, "/");
	tmp = ft_strjoin(full_path, name);
	free(full_path);
	entry->path = tmp;
	lstat(tmp, &entry->st);
	return (entry);
}

void	free_entry(void *ptr) {
	t_entry	*entry;

	entry = (t_entry *)ptr;
	free(entry->name);
	free(entry->path);
	free(entry);
}
