/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 15:56:11 by adpachec          #+#    #+#             */
/*   Updated: 2022/10/03 09:58:13 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_write_nbr(char *s_nbr, long temp, int sz)
{
	if (temp < 0)
	{
		temp *= -1;
		s_nbr[0] = '-';
		while (sz > 1)
		{
			s_nbr[(sz--) - 1] = temp % 10 + '0';
			temp /= 10;
		}
	}
	else
	{
		while (sz >= 0)
		{
			s_nbr[(sz--) - 1] = temp % 10 + '0';
			temp /= 10;
		}
	}
}

char	*ft_itoa(int n)
{
	long	temp;
	int		len;
	char	*s_nbr;

	temp = n;
	if (temp < 0)
		temp *= -1;
	len = 0;
	while (temp > 0)
	{
		temp /= 10;
		++len;
	}
	if (len == 0 || n < 0)
		len += 1;
	s_nbr = (char *) malloc(len + 1);
	if (!s_nbr)
		return (NULL);
	ft_write_nbr(s_nbr, n, len);
	s_nbr[len] = '\0';
	return (s_nbr);
}
