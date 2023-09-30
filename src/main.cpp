#include "lzw.h"

INT main( INT ArgC, CHAR **ArgV )
{
    CHAR m;
    std::string from, to;

    lzw::decoder e;
    lzw::encoder c;
    
    std::cout << "Inpute mode: ";
    std::cin >> m;
    std::cout << "Inpute from: ";
    std::cin >> from;
    std::cout << "Inpute to: ";
    std::cin >> to;

    if (m == 'c')
    {
      if (!c.Open(from, to))
        return 0;
      c.Compress();
      c.Close();    
    }
    else if (m == 'd')
    {
      if (!e.Open(from, to))
        return 0;
      e.Decompress();
      e.Close();
    }
  return 0;
}