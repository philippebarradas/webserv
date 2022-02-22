
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
	is_parsed = false;
	is_sendable = false;
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
	//std::cout << GREEN << "----------------- End of Connec -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connexion&				Connexion::operator=( Connexion const & rhs )
{
	this->recv_len = rhs.recv_len;
	this->is_sendable = rhs.is_sendable;
	this->is_parsed = rhs.is_parsed;
	this->request_header_size = rhs.request_header_size;
	this->fill_request = rhs.fill_request;
	this->b = rhs.b;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
