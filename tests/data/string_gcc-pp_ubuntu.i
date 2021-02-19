# 1 "/usr/include/string.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/usr/include/string.h"
# 25 "/usr/include/string.h"
# 1 "/usr/include/features.h" 1 3 4
# 367 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 410 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 411 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 368 "/usr/include/features.h" 2 3 4
# 391 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 392 "/usr/include/features.h" 2 3 4
# 26 "/usr/include/string.h" 2






# 1 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 1 3 4
# 216 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 3 4

# 216 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 33 "/usr/include/string.h" 2










# 42 "/usr/include/string.h"
extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
       size_t __n) 
# 43 "/usr/include/string.h" 3 4
                  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 43 "/usr/include/string.h"
                          (1, 2)
# 43 "/usr/include/string.h" 3 4
                          ))
# 43 "/usr/include/string.h"
                                            ;


extern void *memmove (void *__dest, const void *__src, size_t __n)
     
# 47 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 47 "/usr/include/string.h"
            (1, 2)
# 47 "/usr/include/string.h" 3 4
            ))
# 47 "/usr/include/string.h"
                              ;

# 60 "/usr/include/string.h"


extern void *memset (void *__s, int __c, size_t __n) 
# 62 "/usr/include/string.h" 3 4
                                                    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 62 "/usr/include/string.h"
                                                            (1)
# 62 "/usr/include/string.h" 3 4
                                                            ))
# 62 "/usr/include/string.h"
                                                                           ;


extern int memcmp (const void *__s1, const void *__s2, size_t __n)
     
# 66 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 66 "/usr/include/string.h"
                               (1, 2)
# 66 "/usr/include/string.h" 3 4
                               ))
# 66 "/usr/include/string.h"
                                                 ;
# 92 "/usr/include/string.h"
extern void *memchr (const void *__s, int __c, size_t __n)
      
# 93 "/usr/include/string.h" 3 4
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 93 "/usr/include/string.h"
                                (1)
# 93 "/usr/include/string.h" 3 4
                                ))
# 93 "/usr/include/string.h"
                                               ;


# 123 "/usr/include/string.h"


extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     
# 126 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 126 "/usr/include/string.h"
            (1, 2)
# 126 "/usr/include/string.h" 3 4
            ))
# 126 "/usr/include/string.h"
                              ;

extern char *strncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     
# 130 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 130 "/usr/include/string.h"
            (1, 2)
# 130 "/usr/include/string.h" 3 4
            ))
# 130 "/usr/include/string.h"
                              ;


extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     
# 134 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 134 "/usr/include/string.h"
            (1, 2)
# 134 "/usr/include/string.h" 3 4
            ))
# 134 "/usr/include/string.h"
                              ;

extern char *strncat (char *__restrict __dest, const char *__restrict __src,
        size_t __n) 
# 137 "/usr/include/string.h" 3 4
                   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 137 "/usr/include/string.h"
                           (1, 2)
# 137 "/usr/include/string.h" 3 4
                           ))
# 137 "/usr/include/string.h"
                                             ;


extern int strcmp (const char *__s1, const char *__s2)
     
# 141 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 141 "/usr/include/string.h"
                               (1, 2)
# 141 "/usr/include/string.h" 3 4
                               ))
# 141 "/usr/include/string.h"
                                                 ;

extern int strncmp (const char *__s1, const char *__s2, size_t __n)
     
# 144 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 144 "/usr/include/string.h"
                               (1, 2)
# 144 "/usr/include/string.h" 3 4
                               ))
# 144 "/usr/include/string.h"
                                                 ;


extern int strcoll (const char *__s1, const char *__s2)
     
# 148 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 148 "/usr/include/string.h"
                               (1, 2)
# 148 "/usr/include/string.h" 3 4
                               ))
# 148 "/usr/include/string.h"
                                                 ;

extern size_t strxfrm (char *__restrict __dest,
         const char *__restrict __src, size_t __n)
     
# 152 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 152 "/usr/include/string.h"
            (2)
# 152 "/usr/include/string.h" 3 4
            ))
# 152 "/usr/include/string.h"
                           ;

# 206 "/usr/include/string.h"

# 231 "/usr/include/string.h"
extern char *strchr (const char *__s, int __c)
     
# 232 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 232 "/usr/include/string.h"
                               (1)
# 232 "/usr/include/string.h" 3 4
                               ))
# 232 "/usr/include/string.h"
                                              ;
# 258 "/usr/include/string.h"
extern char *strrchr (const char *__s, int __c)
     
# 259 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 259 "/usr/include/string.h"
                               (1)
# 259 "/usr/include/string.h" 3 4
                               ))
# 259 "/usr/include/string.h"
                                              ;


# 277 "/usr/include/string.h"



extern size_t strcspn (const char *__s, const char *__reject)
     
# 281 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 281 "/usr/include/string.h"
                               (1, 2)
# 281 "/usr/include/string.h" 3 4
                               ))
# 281 "/usr/include/string.h"
                                                 ;


extern size_t strspn (const char *__s, const char *__accept)
     
# 285 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 285 "/usr/include/string.h"
                               (1, 2)
# 285 "/usr/include/string.h" 3 4
                               ))
# 285 "/usr/include/string.h"
                                                 ;
# 310 "/usr/include/string.h"
extern char *strpbrk (const char *__s, const char *__accept)
     
# 311 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 311 "/usr/include/string.h"
                               (1, 2)
# 311 "/usr/include/string.h" 3 4
                               ))
# 311 "/usr/include/string.h"
                                                 ;
# 337 "/usr/include/string.h"
extern char *strstr (const char *__haystack, const char *__needle)
     
# 338 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 338 "/usr/include/string.h"
                               (1, 2)
# 338 "/usr/include/string.h" 3 4
                               ))
# 338 "/usr/include/string.h"
                                                 ;




extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     
# 344 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 344 "/usr/include/string.h"
            (2)
# 344 "/usr/include/string.h" 3 4
            ))
# 344 "/usr/include/string.h"
                           ;




extern char *__strtok_r (char *__restrict __s,
    const char *__restrict __delim,
    char **__restrict __save_ptr)
     
# 352 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 352 "/usr/include/string.h"
            (2, 3)
# 352 "/usr/include/string.h" 3 4
            ))
# 352 "/usr/include/string.h"
                              ;
# 392 "/usr/include/string.h"


extern size_t strlen (const char *__s)
     
# 395 "/usr/include/string.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 395 "/usr/include/string.h"
                               (1)
# 395 "/usr/include/string.h" 3 4
                               ))
# 395 "/usr/include/string.h"
                                              ;

# 406 "/usr/include/string.h"


extern char *strerror (int __errnum) 
# 408 "/usr/include/string.h" 3 4
                                    __attribute__ ((__nothrow__ , __leaf__))
# 408 "/usr/include/string.h"
                                           ;

# 446 "/usr/include/string.h"
extern void __bzero (void *__s, size_t __n) 
# 446 "/usr/include/string.h" 3 4
                                           __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 446 "/usr/include/string.h"
                                                   (1)
# 446 "/usr/include/string.h" 3 4
                                                   ))
# 446 "/usr/include/string.h"
                                                                  ;
# 658 "/usr/include/string.h"

