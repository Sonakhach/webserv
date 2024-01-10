#include "AllServers.hpp"
#include "Server.hpp"

Server::Server()
{
    // directiv_list={"server_name", "listen", "root", "index", "autoindex", "error_page", "client_max_body_size", "cgi", "allow_methods", "upload_dir", "return"};
    // directiv_list = _directiv_list;

}


Server::~Server()
{

}

void Server::add_locations(std::pair<std::string, Directives> &p)
{
    _locations.push_back(p);
}



std::vector<std::pair<std::string, Directives>> const & Server::get_locations() const
{
    return (_locations);
}


