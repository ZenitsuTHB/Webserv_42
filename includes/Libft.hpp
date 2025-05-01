/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Libft.hpp                                            :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/25 15:13:23 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/04/25 15:23:07 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_HPP
# define LIBFT_HPP

# include <vector>
# include <utility>
# include <string>

namespace	Libft
{
	std::vector< std::string >	split(std::string const &str,
			std::string const delimiter, bool skip = true);
	std::pair< std::string, std::string > splitInTwo(std::string const &str,
			std::string const delimiter);
}

#endif
