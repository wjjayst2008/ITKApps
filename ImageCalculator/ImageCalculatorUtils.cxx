#include "ImageCalculatorUtils.h"
#include <iostream>

void PrintDataTypeStrings(void)
{
    //Prints the Input and output data type strings.
    std::cout << "UCHAR" << std::endl;
    std::cout << "SHORT" << std::endl;
    std::cout << "USHORT" << std::endl;
    std::cout << "INT" << std::endl;
    std::cout << "UINT" << std::endl;
    std::cout << "FLOAT" << std::endl;
    std::cout << "DOUBLE" << std::endl;
}


int CompareNoCase( const std::string &s, const std::string& s2 )
{
    //Compare strings.
    std::string::const_iterator p = s.begin();
    std::string::const_iterator p2 = s2.begin();

    while ( p != s.end() && p2 != s2.end() )
        {
        if ( toupper(*p) != toupper(*p2) ) return (toupper(*p) < toupper(*p2)) ? -1 : 1;
        p++;
        p2++;
        }

    return ( s2.size() == s.size() ) ? 0 : (s.size() < s2.size()) ? -1 : 1;
}