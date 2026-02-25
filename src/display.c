#include "ft_ls.h"

int	num_len(long n) {
	int	len;

	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n) {
		n /= 10;
		len++;
	}
	return (len);
}

void	print_pad(int width, int len) {
	while (len < width) {
		write(1, " ", 1);
		len++;
	}
}

void	print_permissions(unsigned int mode) {
	if (S_ISDIR(mode))
		ft_printf("d");
	else if (S_ISLNK(mode))
		ft_printf("l");
	else
		ft_printf("-");
	ft_printf("%c", (mode & S_IRUSR) ? 'r' : '-');
	ft_printf("%c", (mode & S_IWUSR) ? 'w' : '-');
	ft_printf("%c", (mode & S_IXUSR) ? 'x' : '-');
	ft_printf("%c", (mode & S_IRGRP) ? 'r' : '-');
	ft_printf("%c", (mode & S_IWGRP) ? 'w' : '-');
	ft_printf("%c", (mode & S_IXGRP) ? 'x' : '-');
	ft_printf("%c", (mode & S_IROTH) ? 'r' : '-');
	ft_printf("%c", (mode & S_IWOTH) ? 'w' : '-');
	ft_printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

void	get_max_widths(t_list *entries, int *max_nlink, int *max_size) {
	t_entry	*entry;

	*max_nlink = 0;
	*max_size = 0;
	while (entries) {
		entry = (t_entry *)entries->content;
		if (num_len(entry->st.st_nlink) > *max_nlink)
			*max_nlink = num_len(entry->st.st_nlink);
		if (num_len(entry->st.st_size) > *max_size)
			*max_size = num_len(entry->st.st_size);
		entries = entries->next;
	}
}

void	print_entry(t_entry *e, t_options *opts, int max_nlink, int max_size) {
	if (opts->l) {
		print_permissions(e->st.st_mode);
		print_pad(max_nlink, num_len(e->st.st_nlink));
		ft_printf(" %d", e->st.st_nlink);
		ft_printf(" %s", getpwuid(e->st.st_uid)->pw_name);
		ft_printf(" %s", getgrgid(e->st.st_gid)->gr_name);
		print_pad(max_size, num_len(e->st.st_size));
		ft_printf(" %d ", e->st.st_size);
		write(1, ctime(&e->st.st_mtime) + 4, 12);
	}
	if (opts->l && S_ISLNK(e->st.st_mode)) {
		char	buf[1024];
		ssize_t	len;

		len = readlink(e->path, buf, sizeof(buf) - 1);
		if (len > 0)
			buf[len] = '\0';
		else
			buf[0] = '\0';
		ft_printf(" %s -> %s\n", e->name, buf);
	}
	else if (opts->l)
		ft_printf(" %s\n", e->name);
	else
		ft_printf("%s\n", e->name);
}
