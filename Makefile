CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror -I.
RM = rm -rf
EXEC = ft_ls

RED    = \033[31m
GREEN  = \033[32m
YELLOW = \033[33m
CYAN   = \033[36m
RESET  = \033[0m

# ------------------------------------------------------------------------------
# 									FILES
# ------------------------------------------------------------------------------

SRC_DIR = src/
UTL_DIR = utils/
OBJ_DIR = obj/
DEP_DIR = dep/

SRC_FILES = $(wildcard $(SRC_DIR)*.c)
UTL_FILES = $(wildcard $(UTL_DIR)*.c)
OBJ = $(SRC_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o) $(UTL_FILES:$(UTL_DIR)%.c=$(OBJ_DIR)%.o)
DEP = $(OBJ:%.o=%.d)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# ------------------------------------------------------------------------------
# 									COMPILING
# ------------------------------------------------------------------------------

all: $(EXEC)

$(LIBFT):
	@echo "$(CYAN)Compiling libft...$(RESET)"
	@$(MAKE) --no-print-dir -C $(LIBFT_DIR) bonus

$(EXEC): $(OBJ) $(LIBFT)
	@echo "$(CYAN)Linking ft_ls...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(EXEC)
	@echo "$(GREEN)Success!$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR) $(DEP_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -MF $(DEP_DIR)$*.d

$(OBJ_DIR)%.o: $(UTL_DIR)%.c | $(OBJ_DIR) $(DEP_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -MF $(DEP_DIR)$*.d

-include $(DEP)

# ------------------------------------------------------------------------------
# 									DIR GENERATION
# ------------------------------------------------------------------------------

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

# ------------------------------------------------------------------------------
# 									CLEANING
# ------------------------------------------------------------------------------

clean:
	@echo "$(RED)Cleaning...$(RESET)"
	@$(MAKE) clean --no-print-dir -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR) $(DEP_DIR)
	@echo "$(GREEN)Success!$(RESET)"

fclean: clean
	@$(MAKE) fclean --no-print-dir -C $(LIBFT_DIR)
	@$(RM) $(EXEC)

re: fclean all

.PHONY: all clean fclean re
