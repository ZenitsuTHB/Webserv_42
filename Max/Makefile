#<-------------------------------|VARIABLES|---------------------------------->#

NAME = address
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98# -fsanitize=address

#<---------------------------------|FILES|------------------------------------>#

SRC_F = main.cpp

OBJ_F = $(SRC_F:.cpp=.o)
OBJ_D = ./objects/
OBJ = $(addprefix $(OBJ_D), $(OBJ_F))

DEP_F = $(SRC_F:.cpp=.d)
DEP_D = ./dependencies/
DEP = $(addprefix $(DEP_D), $(DEP_F))

#<---------------------------------|COLORS|----------------------------------->#

DF = \033[0;39m#                Default
RED= \033[0;31m#                Red
BRED= \033[1;31m#               Bold Red
BBLUE= \033[1;34m#              Bold Blue
BYELLOW= \033[1;33m#    Bold Yellow
BIPRPL = \033[1;95m#    Purple
BCYAN= \033[1;36m#              Bold Cyan
BIRED= \033[1;91m#              Bold Intense Red
BIGREEN= \033[1;92m#    Bold Intense Green
BBLACK= \033[1;30m#             Bold Black

#<---------------------------------|RULES|------------------------------------>#

all: $(NAME)

$(NAME): $(OBJ_D) $(DEP_D) $(OBJ)
	@echo "\n$(RED)Compiling program:$(DF)"
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) $(MLXFLAGS) \
	$(BIGREEN)$(OBJ_F) $(BCYAN)-o $(RED)$(NAME)$(DF)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

${OBJ_D}%.o: %.cpp Makefile
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c \
	$(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

$(OBJ_D):
	@mkdir $(OBJ_D)
	@echo "$(BCYAN)mkdir $(BCYAN)$(OBJ_D)$(DF)"

$(DEP_D):
	@mkdir $(DEP_D)
	@echo "$(BCYAN)mkdir $(BCYAN)$(DEP_D)$(DF)"

#<---------------------------------|PHONY|------------------------------------>#

clean:
	@rm -rf $(OBJ_D) $(DEP_D)
	@echo "$(RED)rm -rf $(BIGREEN)OBJECTS: $(OBJ_F)$(DF)"
	@echo "$(RED)rm -rf $(BYELLOW)DEPENDENCIES: $(DEP_F)$(DF)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)rm -rf $(RED)PROGRAM: $(NAME)$(DF)\n"

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
#<---------------------------------------------------------------------------->#
