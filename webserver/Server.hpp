#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include "Directives.hpp"

// typedef std::vector<std::pair<std::string,  std::vector<std::string>>> Vec;

class Server : public Directives
{
private:
    //Vec directives;

    std::vector<std::pair<std::string, Directives> > _locations;

public:
    Server();
    ~Server();

    std::vector<std::pair<std::string, Directives> > const & get_locations() const;
    void add_locations(std::pair<std::string, Directives> &p);
};







#endif