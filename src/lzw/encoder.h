/* FILE NAME   : encoder.h
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : LZW compression project.
 *               Encoder header file.
 */

#ifndef __encoder_h_
#define __encoder_h_

#include "def.h"
#include "bitrw/write.h"

#include <map>

/* Project namespace */
namespace lzw
{
  /* Encoder class */
  class encoder
  {
  private:
      std::fstream In;
      write Out;
  public:
    /* Class constructor */ 
    encoder( VOID )
    {
    }
    /* Class destructor */ 
    ~encoder( VOID )
    {
    }

    /* Open function.
     * ARGUMENTS:
     *	  - File In name:
     *	      const std::string &FnIn;
     * 	  - File Out name:
     *	      const std::string &FnOut;
     * RETURNS:
     *	(BOOL) success of opening.
     */
    BOOL Open( const std::string &FnIn, const std::string &FnOut )
    {
      Close();

      In = std::fstream(FnIn, std::fstream::in | std::fstream::binary);
      BOOL g = Out.Open(FnOut);

      return ( ((bool)In) && g );
    } /* End of 'Open' function */

    /* Compress function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Compress( VOID )
    {
      std::map<std::pair<INT, INT>, INT> DictMap;
      INT Cur, Next;
      INT DictSize = 258;
      INT CurBit = 9;
      
      Cur = In.get();
      if (Cur == EOF)
        return;
      Next = In.get();

      while (Next != EOF)
      {
        if (DictMap.find({Cur, Next}) == DictMap.end())
        {
          // Not found - add new
          DictMap[{Cur, Next}] = DictSize;
          DictSize++;

          if (DictSize >= pow(2, CurBit))
            if (CurBit < MAX_BITS)
              CurBit++;

          /*if (CurBit >= MAX_BITS)
          {
            WriteAsBits(CODE_CLEAR, CurBit);
            CurBit = 9;
            DictSize = 258;
            DictMap.clear();
          }*/

          WriteAsBits(Cur, CurBit);
          Cur = Next;
        }
        else
          Cur = DictMap[{Cur, Next}];  

        Next = In.get();
      }
      
      WriteAsBits(Cur, CurBit);
      WriteAsBits(CODE_TERM, CurBit);
    } /* End of 'Compress' function */


    /* Write as bits function.
     * ARGUMENTS:
     *	  - Number to write:
     *	      INT N;
     * 	  - Count of bits:
     *	      INT B;
     * RETURNS: None.
     */
    VOID WriteAsBits( INT N, INT B )
    {
      for (INT i = B - 1; i >= 0; i--)
        Out.WriteBit((N >> i) & 1);
    } /* End of 'WriteAsBits' function */

    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID )
    {
      if (In.is_open())
        In.close();
      Out.Close();
    } /* End of 'Close' function */

  }; /* End of 'encoder' class */
} /* end of 'lzw' namespace */


#endif /* __encoder_h_ */

/* END OF 'encoder.h' FILE */