/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:51:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/06 20:30:32 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <vector>

class Config
{

	public:

		Config( Config const & src );
		Config( std::string const & file_name );
		~Config();

		Config &		operator=( Config const & rhs );

	private:
		Config();

		std::vector<std::string>	split_server( std::string const & fileStr ) const;

};

std::ostream &			operator<<( std::ostream & o, Config const & i );

#endif /* ********************************************************** CONFIG_H */
