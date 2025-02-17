# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 16:10:06 by adrmarqu          #+#    #+#              #
#    Updated: 2025/02/17 16:13:48 by adrmarqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

$(NAME) = webserv

$(CXX) = c++
$(CXXFLAGS) = -Wall -Wextra -Werror
$(STD) = -std=c++98



@$(CXX) $(CXXFLAGS) $(STD) $(OBJ) -o $(NAME)



.PHONY: all clean fclean re
