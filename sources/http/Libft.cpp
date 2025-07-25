/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*  Libft.cpp                                            :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*  By: mvelazqu <mvelazqu@student.42barcelona.c     +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*  Created: 2025/04/25 12:36:07 by mvelazqu            #+#    #+#            */
/*  Updated: 2025/06/05 16:31:51 by mvelazqu           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Libft.hpp"
//#include <iostream>
#include <fstream>

std::string	Libft::itos(int integer)
{
	std::string	num;
	int			nega;

	if (integer == 0)
		num.append(1, '0');
	else if (integer == -2147483648)
		num.append("-2147483648");
	else
	{
		if (integer < 0)
		{
			nega = 1;
			integer *= -1;
		}
		else
			nega = 0;
		while (integer)
		{
			num.insert(0, 1, integer % 10 + '0');
			integer /= 10;
		}
		if (nega)
			num.insert(0, 1, '-');
	}
	return (num);
}

VecStr	Libft::split(std::string const &str, std::string const set, bool skip)
{
	VecStr	vecStr;
	size_t	len;
	size_t	n;
	size_t	i;

	n = str.length();
	i = 0;
	while (i < n)
	{
		if (skip)
			while (i < n && set.find(str[i]) != std::string::npos)
				i++;
		else
			if (i < n && set.find(str[i]) != std::string::npos)
				i++;
		len = 0;
		while (i + len < n && set.find(str[i + len]) == std::string::npos)
			len++;
		if (len != 0)
			vecStr.push_back(str.substr(i, len));
		i += len;
	}
	return (vecStr);
}

PairStr	Libft::splitInTwo(std::string const &str, std::string const set)
{
	PairStr	ret;
	size_t	pos;

	pos = str.find(set);
	if (pos == std::string::npos)
	{
		ret.first = str;
		return (ret);
	}
	ret.first = str.substr(0, pos);
	ret.second = str.substr(pos + set.length());
	return (ret);
}

std::string	&Libft::trim(std::string &str, std::string const set)
{
	if (str.empty())
		return (str);
	for (int i = str.length() - 1; i >= 0; --i)
	{
		if (set.find(str[i]) == std::string::npos)
		{
			if (i != static_cast<int>(str.length()))
				str.erase(i + 1);
			break ;
		}
	}
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (set.find(str[i]) == std::string::npos)
		{
			if (i != 0)
				str.erase(0, i);
			break ;
		}
	}
	return (str);
}

std::string	Libft::readFile(std::string const &filename)
{
	std::ifstream	file(filename.c_str(), std::ios::binary);
	std::string		fileText;
	int				size;
	char			*textBuf;

	file.seekg(0, file.end);
	size = file.tellg();
	file.seekg(0, file.beg);
	if (file.fail() || size == -1)
		throw (std::ios_base::failure("Failed to manage file"));
	textBuf = new char[size];
	file.read(textBuf, size);
	fileText.assign(textBuf, size);
	delete[] textBuf;
	file.close();
	return (fileText);
}

//int	main(void)
//{
//	for (std::
//}
