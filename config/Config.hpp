/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/01 09:18:27 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <fstream>

class Config
{

	public:

		Config( Config const & src );
		Config( std::string const & file_name );
		~Config();

		Config &		operator=( Config const & rhs );

	private:
		Config();

};

std::ostream &			operator<<( std::ostream & o, Config const & i );

#endif /* ********************************************************** CONFIG_H */
