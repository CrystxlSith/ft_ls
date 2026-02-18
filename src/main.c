#include "ft_ls.h"





int main(int argc, char const *argv[])
{
    t_options opts;
    parseArgs(argc, argv, &opts);
    list_directories(&opts);
    free(opts.paths);
    return 0;
}
