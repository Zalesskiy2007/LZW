#include "lzw.h"

INT main( VOID )
{
  lzw::decoder e;
  lzw::encoder c;

  std::string from;
  
  std::cout << "FROM: ";
  std::cin >> from;

  c.Open("from/" + from + ".txt", "result/encode.lzw");
  c.Compress();
  c.Close();

  e.Open("result/encode.lzw", "result/decode.txt");
  e.Decompress();
  e.Close();
  return 0;
}