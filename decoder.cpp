/* FILE NAME   : decoder.cpp
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : LZW compression project.
 *               Decoder header file.
 */

#include "lzw.h"

#include <map>

/* Open function.
 * ARGUMENTS:
 *	  - File In name:
 *	      const std::string &FnIn;
 * 	  - File Out name:
 *	      const std::string &FnOut;
 * RETURNS:
 *	(BOOL) success of opening.
 */
BOOL lzw::decoder::Open( const std::string &FnIn, const std::string &FnOut )
{
  Close();

  BOOL i = In.Open(FnIn);
  Out = std::fstream(FnOut, std::fstream::out | std::fstream::binary);

  return (i && (bool)Out);
} /* End of 'Open' function */

/* Read as bits function.
 * ARGUMENTS:
 * 	  - Count of bits:
 *	      INT B;
 * RETURNS: Readed Data.
 */
INT lzw::decoder::ReadAsBits( INT B )
{
  INT x = 0;

  for (INT i = B - 1; i >= 0; i--)
  {
    INT z = In.ReadBit();
    if (z == 1)
      x |= (1 << i);
    else if (z == 0)
      x &= ~(1 << i);
  }
  return x;
} /* End of 'ReadAsBits' function */

/* Close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID lzw::decoder::Close( VOID )
{
  In.Close();
  if (Out.is_open())
    Out.close();
} /* End of 'Close' function */

struct _dict
{
  INT P, S;
} Dict[MAX];

BYTE Stack[MAX];

/* Decompress function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID lzw::decoder::Decompress( VOID )
{
  INT Cur, Next;
  INT DictSize = 258;
  INT CurBit = 9;
  INT i = 0;
  INT code = 0;

  std::map<std::pair<INT, INT>, INT> DictMap;
  
  Cur = ReadAsBits(CurBit);
  if (Cur == CODE_TERM)
    return;

  while ((Next = ReadAsBits(CurBit)) != CODE_TERM)
  {
    if (Next < 256)
    {
      Out.put(Next);
      Dict[DictSize++] = {Cur, Next};
      Cur = Next;
      continue;
    }
    if (Next < DictSize)
    {
      i = 0;
      code = Next;

      while (code > 257)
      {
        Stack[i++] = Dict[code].S;
        code = Dict[code].P;
      }
      Stack[i++] = code;
      while (i--)
        Out.put(code);
      Dict[DictSize++] = {Cur, code};
      Cur = Next;
    }
    else
    {
      INT FirstCur = Cur;
      while (FirstCur > 255)
        FirstCur = Dict[FirstCur].P;
      Dict[DictSize++] = {Cur, FirstCur};

      i = 0;
      code = Next;

      while (code > 257)
      {
        Stack[i++] = Dict[code].S;
        code = Dict[code].P;
      }
      Stack[i++] = code;
      while (i--)
        Out.put(code);
      Dict[DictSize++] = {Cur, code};
      Cur = Next;
    }
  }
} /* End of 'Decompress' function */


/* END OF 'decoder.cpp' FILE */