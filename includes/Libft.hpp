/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Libft.hpp                                            :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/25 15:13:23 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/06 19:52:15 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_HPP
# define LIBFT_HPP

# include <vector>
# include <utility>
# include <string>

# define SPACE "\t\v\r\f\n "

typedef std::vector< std::string >				VecStr;
typedef std::pair< std::string, std::string >	PairStr;
typedef std::string								String;

namespace	Libft
{
	std::string	&trim(std::string &str, std::string const set);
	PairStr		splitInTwo(std::string const &str, std::string const set);
	VecStr		split(std::string const &str,
			std::string const set, bool skip = true);
}

#endif
