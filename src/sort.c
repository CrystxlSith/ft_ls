#include "ft_ls.h"

static int	should_swap(t_entry *a, t_entry *b, t_options *opts) {
	if (opts->t) {
		if (a->st.st_mtime != b->st.st_mtime)
			return (a->st.st_mtime < b->st.st_mtime);
		return (ft_strncmp(a->name, b->name, 256) > 0);
	}
	return (ft_strncmp(a->name, b->name, 256) > 0);
}

void	entries_sort(t_list **entries, t_options *opts) {
	int		swapped;
	t_list	*tmp;
	void	*tmp2;

	swapped = 1;
	while (swapped) {
		swapped = 0;
		tmp = *entries;
		while (tmp && tmp->next) {
			if (should_swap(tmp->content, tmp->next->content, opts)) {
				tmp2 = tmp->content;
				tmp->content = tmp->next->content;
				tmp->next->content = tmp2;
				swapped = 1;
			}
			tmp = tmp->next;
		}
	}
	if (opts->r) {
		t_list	*prev = NULL;
		t_list	*curr = *entries;
		t_list	*next;

		while (curr) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		*entries = prev;
	}
}
