
bool
cat (char *outbuf, size_t outsize)
{
    char *bpout;
    char *wp = outbuf;
    size_t remaining_bytes;
    do
    {
        remaining_bytes = bpout - wp;
        T x = bpout - wp;
    }
    while (outsize <= remaining_bytes);

    memmove (outbuf, wp, remaining_bytes);
    bpout = outbuf + remaining_bytes;
    return 1;
}

