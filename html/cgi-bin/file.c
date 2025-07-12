/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrmarqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:58:40 by adrmarqu          #+#    #+#             */
/*   Updated: 2025/07/10 17:58:58 by adrmarqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;

    char *data = malloc(len + 1);
    fread(data, 1, len, stdin);
    data[len] = '\0';

    printf("Content-Type: text/plain\n\n");

    printf("POST recibido: %s\n", data);

    free(data);
    return 0;
}
