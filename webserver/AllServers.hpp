#ifndef ALLSERVERS_HPP
#define ALLSERVERS_HPP

#include "Server.hpp"

class AllServers
{
private:
    int _servsCount;
    std::map<int, Server *> _AllServs;
    std::vector<std::string> _directiv_list;

public:
    AllServers();
    ~AllServers();
    

    void readConff();
    size_t find_server_end(std::string str);
    void make_location(std::string &s, Server &S);
    Server *makeServer(std::string &s);
    void print_AllServs();
    void print_serv(int n);
    void print_vect(std::vector<std::string> &v);
    void print_map(Vec);

    

    //CHeck

    void    dir_is_valid(std::string dir, int from);
    void    check_validity(std::string const &full) const;
    void    check_parentheses(std::string const &full) const;


};


#endif