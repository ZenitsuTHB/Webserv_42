/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Libft.cpp                                            :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/25 12:36:07 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/05/01 16:14:08 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Libft.hpp"

std::vector< std::string >	Libft::split(std::string const &str,
		std::string const delimiter, bool skip)
{
	std::vector< std::string >	vecStr;
	size_t						len;
	size_t						n;
	size_t						i;

	n = str.length();
	i = 0;
	while (i < n)
	{
		if (skip)
		{
			while (i < n && delimiter.find(str[n]))
				i++;
		}
		else
		{
			if (i < n && delimiter.find(str[n]))
				i++;
		}
		len = 0;
		while (i < n && !delimiter.find(str[n]))
			len++;
		if (len != 0)
			vecStr.push_back(str.substr(i, len));
		i += len;
	}
	return (vecStr);
}

std::pair< std::string, std::string > Libft::splitInTwo(std::string const &str,
		std::string const delimiter)
{
	std::pair< std::string, std::string >	ret;
	size_t									pos;

	pos = str.find(delimiter);
	if (pos == std::string::npos)
	{
		ret.first = str;
		return (ret);
	}
	ret.first = str.substr(0, pos);
	ret.second = str.substr(pos + 1);
	return (ret);
}
