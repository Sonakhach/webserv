#include "AllServers.hpp"

int main()
{
    
        try
        {
            AllServers k;
            k.readConff();
            k.print_AllServs();
        }
        catch(const std::exception& e)
        {
            // std::cout << "hhhhh" << std::endl;
            std::cerr << e.what() << std::endl;;
        }
        catch(...)
        {
            std::cout << "hhhhh" << std::endl;
            // std::cerr << e.what() << std::endl;;
        }
   
}