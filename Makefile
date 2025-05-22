#<-------------------------------|VARIABLES|---------------------------------->#

NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

#<---------------------------------|FILES|------------------------------------>#

# Source directories
SRC_CORE_D = sources/core/
SRC_HTTP_D = sources/http/
SRC_UTILS_D = sources/utils/
SRC_SOCKET_D = sources/socket/
SRC_CONFIG_D = sources/config/

# Source files
SRC_CORE_F = Server.cpp 
SRC_HTTP_F = Http.cpp HttpRequest.cpp HttpResponse.cpp Libft.cpp
SRC_UTILS_F = Client.cpp
SRC_SOCKET_F = BaseSocket.cpp ConnectSocket.cpp ListenSocket.cpp
SRC_CONFIG_F = BaseConfig.cpp ParserConfig.cpp RouteConfig.cpp ServerConfig.cpp 
MAIN_F = main.cpp

# Combine all sources
SRC_F = $(MAIN_F) \
         $(addprefix $(SRC_CORE_D), $(SRC_CORE_F)) \
         $(addprefix $(SRC_HTTP_D), $(SRC_HTTP_F)) \
         $(addprefix $(SRC_UTILS_D), $(SRC_UTILS_F)) \
         $(addprefix $(SRC_SOCKET_D), $(SRC_SOCKET_F))\
         $(addprefix $(SRC_CONFIG_D), $(SRC_CONFIG_F))

# Object files
OBJ_D = objects/
OBJ_F = $(notdir $(SRC_F:.cpp=.o))
OBJ = $(addprefix $(OBJ_D), $(OBJ_F))

# Dependency files
DEP_D = dependencies/
DEP_F = $(notdir $(SRC_F:.cpp=.d))
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

test: server.exe client.exe

server.exe: server_socket.c
	cc server_socket.c -o server.exe

client.exe: client_socket.c
	cc client_socket.c -o client.exe

$(NAME): $(OBJ_D) $(DEP_D) $(OBJ)
	@echo "\n$(RED)Compiling program:$(DF)"
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) $(BIGREEN)$(OBJ_F) $(BCYAN)-o $(RED)$(NAME)$(DF)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

# Pattern rule for compiling .cpp files
$(OBJ_D)%.o: sources/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

# Specific pattern rules for each subdirectory
# ... (el resto del Makefile permanece igual)

# Specific pattern rules for each subdirectory
$(OBJ_D)%.o: $(SRC_CORE_D)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

$(OBJ_D)%.o: $(SRC_HTTP_D)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

$(OBJ_D)%.o: $(SRC_UTILS_D)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

$(OBJ_D)%.o: $(SRC_SOCKET_D)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

$(OBJ_D)%.o: $(SRC_CONFIG_D)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@mv $(@:.o=.d) $(DEP_D)
	@echo "$(BCYAN)$(CC) $(BBLUE)$(CFLAGS) -MMD $(BCYAN)-c $(BIRED)$< $(BCYAN)-o $(BIGREEN)$@$(DF)"
	@echo "$(BCYAN)mv $(BYELLOW)$(@:.o=.d) $(BCYAN)$(DEP_D)$(DF)"

# ... (el resto del Makefile permanece igual)

$(OBJ_D):
	@mkdir -p $(OBJ_D)
	@echo "$(BCYAN)mkdir -p $(BCYAN)$(OBJ_D)$(DF)"

$(DEP_D):
	@mkdir -p $(DEP_D)
	@echo "$(BCYAN)mkdir -p $(BCYAN)$(DEP_D)$(DF)"

#<---------------------------------|PHONY|------------------------------------>#

clean:
	@rm -rf $(OBJ_D) $(DEP_D)
	@echo "$(RED)rm -rf $(BIGREEN)OBJECTS: $(OBJ_F)$(DF)"
	@echo "$(RED)rm -rf $(BYELLOW)DEPENDENCIES: $(DEP_F)$(DF)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)rm -rf $(RED)PROGRAM: $(NAME)$(DF)\n"

cleanTest:
	rm -rf client.exe server.exe

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re test cleanTest
