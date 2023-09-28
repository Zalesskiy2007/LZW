if (DictSize >= pow(2, CurBit) - 1)
{
  CurBit++;
  Next <<= 1;
  Next |= In.ReadBit();
}

if (Next == CODE_CLEAR)
{
  CurBit = 9;
  DictSize = 258;
  for (INT z = 0; z < MAX; z++)
  {
    Dict[z] = {-1, -1};
    Stack[z] = 0;
  }
  continue;
}