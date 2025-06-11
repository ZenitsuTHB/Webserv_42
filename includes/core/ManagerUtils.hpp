/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManagerUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:46:19 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/08 14:51:53 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_UTILS
# define MANAGER_UTILS

#include <iostream>

namespace ManUtils
{
    bool readClientData(int client_fd, std::string& clientBuffer);
}

#endif 