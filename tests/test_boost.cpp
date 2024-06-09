#include <iostream>
#include <boost/lexical_cast.hpp>

int main()
{
    std::string s = "123";
    int i = boost::lexical_cast<int>(s);
    std::cout << i << std::endl;
    return 0;
}