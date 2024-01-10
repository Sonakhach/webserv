#include "AllServers.hpp"

AllServers::AllServers()
{
    _directiv_list={"server_name", "listen", "root", "index", "autoindex", "error_page", "client_max_body_size", "cgi", "allow_methods", "upload_dir", "return", "upload_path", "methods"};
    _servsCount = 0;

}

AllServers::~AllServers() {
    std::map<int, Server *>::iterator it = _AllServs.begin();
    while(it != _AllServs.end())
    {
        delete it->second;
        *it++;
    }
};

size_t AllServers::find_server_end(std::string str)
{
    // std::stringstream ss(str);
    // std::string tok;
    size_t sz = str.size();
    size_t posible_end = str.find("}");
    // if (getline(ss, tok, ' '))
    // {
    //     if (tok != "server")
    // }
    
    while(posible_end != sz - 1 && str[posible_end + 2] != 's')
    {
        posible_end = str.find_first_of("}",  posible_end + 1);
    }

    return posible_end;
}

void AllServers::make_location(std::string &s, Server &S)
{
    Directives *m = new Directives;
    std::string tok;
    std::string key;
    std::string mkey;
    std::vector <std::string> value;
    std::vector<std::string> fv;
    std::stringstream ss(s);

    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "location")
            {
                ss >> mkey; // to pass from "location" to mkey 
                ss >> tok; // to pass from "mkey" to "{"
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            if(key != "}")
            {
                dir_is_valid(key, 2);

                std::pair<std::string,  std::vector<std::string> > p {key, value};
                m->add_directives(p);
            }
               
        }
    }
    std::pair<std::string, Directives> p {mkey, *m};
    S.add_locations(p);
}

Server *AllServers::makeServer(std::string &s)
{

    Server *S = new Server;

    size_t ilocation = s.find("location");
    while(ilocation != std::string::npos)
    {
        size_t i_end_location = s.find("}");
        std::string str = s.substr(ilocation, i_end_location - ilocation + 1);
        make_location(str, *S);
        s.erase(ilocation, i_end_location - ilocation + 1);
        ilocation = s.find("location");
    }


    std::stringstream ss(s);
    int servisopen = 0;
    int lockisopen = 0;
    std::string tok;
    std::string key;
    std::vector <std::string> value;
    std::vector<std::string> fv;


    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "server")
            {
                ss >> key; // to pass from "server" to "{" 
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            if(key != "}")
            {
                // std::cout << "\n\n\n\n" << key << "\n\n\n\n";
                dir_is_valid(key, 0);
                std::pair<std::string,  std::vector<std::string> > p {key, value};
                S->add_directives(p);
                // std::cout << "key = " << key << std::endl;
            }
        }
    }
    return S;
            
}

void AllServers::dir_is_valid(std::string dir, int from)
{
    std::vector<std::string>::iterator it = std::find(next(_directiv_list.begin(), from), _directiv_list.end(), dir);
    if(it == _directiv_list.end())
    {
        throw std::invalid_argument("invalid directiv");
    }

}
void AllServers::readConff()
{
    std::ifstream fin("config2.conf");
    std::string line;
    std::string full;
    std::string path;
    std::vector<std::string> fv;
    fin >> line;  ///
    full = line;
    if(line != "server")
    {
        // std::cout << "ERROR" << std::endl;
        throw std::invalid_argument("Not server");
    }   ///
    while(fin >> line)
    {
        full = full + " " + line;
    }

    check_validity(full);

    while(full.size() > 0)
    {
        
        _servsCount++;
        size_t serv_end = find_server_end(full);
        std::string serv = full.substr(0, serv_end + 1);
        _AllServs.insert(std::pair<int, Server *>(_servsCount, makeServer(serv)));
        full.erase(0, serv_end + 1);
    }   
    // std::cout << "_AllServs = " << _AllServs.size() << std::endl;
}


void AllServers::print_vect(std::vector<std::string> &v)
{
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

}


void AllServers::print_map(Vec m)
{
    std::cout << " LOCATION SIZE " << m.size() << " \n ";
    Vec::iterator it = m.begin();
    for(; it != m.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }
}



void AllServers::print_serv(int n)
{
    Vec m = _AllServs[n]->get_directives();

    Vec::iterator it = m.begin();
    for(; it != m.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }
    std::vector<std::pair<std::string, Directives> > loc = _AllServs[n]->get_locations();
    std::vector<std::pair<std::string, Directives> >::iterator i = loc.begin();
    for(; i != loc.end(); ++i)
    {
        std::cout << "Locationnn  " << i->first << std::endl;
        Directives d = i->second;
        Vec m = d.get_directives();
        print_map(m);
    }
}

void AllServers::print_AllServs()
{
    // std::cout << "_servsCount = " << _servsCount << std::endl;
    for(int i = 0; i < _servsCount; ++i)
    {
        std::cout << "Server N - " << i+1 <<" is :\n";
        print_serv(i + 1);
    }
}


void    AllServers::check_validity(std::string const &full)const
{
    check_parentheses(full);
}

void    AllServers::check_parentheses(std::string const &full)const
{
    int p = 0;
    for(int i = 0; i < full.size(); ++i)
    {
        if(full[i] == '{')
        {
            p++;
        }
        else if(full[i] == '}')
        {
            p--;
            if(p < 0)
            {
                throw std::invalid_argument("\nInvalid parentheses 1 !!!\n");
            }
        }
    }
    if(p != 0)
    {
        throw std::invalid_argument("\nInvalid parentheses!!!\n");
    }
}
