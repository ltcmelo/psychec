static int vn(const u8 *x,const u8 *y,int n)
{
  u32 i,d = 0;
  for (i = 0; i < n; ++i) d |= x[i]^y[i];
  return (1 & ((d - 1) >> 8)) - 1;
}

int crypto_verify_16(const u8 *x,const u8 *y)
{
  return vn(x,y,16);
}
