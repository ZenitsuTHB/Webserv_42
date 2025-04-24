/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:09:31 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/04/24 14:12:32 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParserConfig.hpp"
#include "../includes/ServerConfig.hpp"
#include <fstream>

ParserConfig::~ParserConfig() {}

ParserConfig::ParserConfig(std::string input)
{
	//std::ifstream	file(input);
	std::cout << input << std::endl;
}
