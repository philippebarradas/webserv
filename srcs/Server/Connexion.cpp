
#include "Connexion.hpp"
#include "../Cgi/Cgi.hpp"
#include "../Parse_request/parse_request.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Connexion::Connexion()
{
	recv_len = 0;
	request_header_size = 0;
	fill_request = "";
}

void	Connexion::reinit_obj()
{
	recv_len = 0;
	request_header_size = 0;
	fill_request = "";
}


Connexion::Connexion(Connexion const & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Connexion::~Connexion()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connexion&				Connexion::operator=( Connexion const & rhs )
{

	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
