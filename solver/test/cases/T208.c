void y()
{
    size_t outsize;

    if (outbuf + outsize <= bpout)
    {
        char *wp = outbuf;
        size_t remaining_bytes;
        wp += outsize;
        remaining_bytes = bpout - wp;
    }
}
