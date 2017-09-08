# 1 "/usr/include/stdio.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdio.h"
# 64 "/usr/include/stdio.h"
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 587 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/sys/_symbol_aliasing.h" 1 3 4
# 588 "/usr/include/sys/cdefs.h" 2 3 4
# 653 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/sys/_posix_availability.h" 1 3 4
# 654 "/usr/include/sys/cdefs.h" 2 3 4
# 65 "/usr/include/stdio.h" 2
# 1 "/usr/include/Availability.h" 1 3 4
# 190 "/usr/include/Availability.h" 3 4
# 1 "/usr/local/Cellar/gcc/7.2.0/lib/gcc/7/gcc/x86_64-apple-darwin16.7.0/7.2.0/include-fixed/AvailabilityInternal.h" 1 3 4
# 191 "/usr/include/Availability.h" 2 3 4
# 66 "/usr/include/stdio.h" 2

# 1 "/usr/include/_types.h" 1 3 4
# 27 "/usr/include/_types.h" 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 32 "/usr/include/machine/_types.h" 3 4
# 1 "/usr/include/i386/_types.h" 1 3 4
# 37 "/usr/include/i386/_types.h" 3 4

# 37 "/usr/include/i386/_types.h" 3 4
typedef signed char __int8_t;



typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long long __int64_t;
typedef unsigned long long __uint64_t;

typedef long __darwin_intptr_t;
typedef unsigned int __darwin_natural_t;
# 70 "/usr/include/i386/_types.h" 3 4
typedef int __darwin_ct_rune_t;





typedef union {
 char __mbstate8[128];
 long long _mbstateL;
} __mbstate_t;

typedef __mbstate_t __darwin_mbstate_t;


typedef long int __darwin_ptrdiff_t;







typedef long unsigned int __darwin_size_t;





typedef __builtin_va_list __darwin_va_list;





typedef int __darwin_wchar_t;




typedef __darwin_wchar_t __darwin_rune_t;


typedef int __darwin_wint_t;




typedef unsigned long __darwin_clock_t;
typedef __uint32_t __darwin_socklen_t;
typedef long __darwin_ssize_t;
typedef long __darwin_time_t;
# 33 "/usr/include/machine/_types.h" 2 3 4
# 34 "/usr/include/sys/_types.h" 2 3 4
# 55 "/usr/include/sys/_types.h" 3 4
typedef __int64_t __darwin_blkcnt_t;
typedef __int32_t __darwin_blksize_t;
typedef __int32_t __darwin_dev_t;
typedef unsigned int __darwin_fsblkcnt_t;
typedef unsigned int __darwin_fsfilcnt_t;
typedef __uint32_t __darwin_gid_t;
typedef __uint32_t __darwin_id_t;
typedef __uint64_t __darwin_ino64_t;

typedef __darwin_ino64_t __darwin_ino_t;



typedef __darwin_natural_t __darwin_mach_port_name_t;
typedef __darwin_mach_port_name_t __darwin_mach_port_t;
typedef __uint16_t __darwin_mode_t;
typedef __int64_t __darwin_off_t;
typedef __int32_t __darwin_pid_t;
typedef __uint32_t __darwin_sigset_t;
typedef __int32_t __darwin_suseconds_t;
typedef __uint32_t __darwin_uid_t;
typedef __uint32_t __darwin_useconds_t;
typedef unsigned char __darwin_uuid_t[16];
typedef char __darwin_uuid_string_t[37];

# 1 "/usr/include/sys/_pthread/_pthread_types.h" 1 3 4
# 57 "/usr/include/sys/_pthread/_pthread_types.h" 3 4
struct __darwin_pthread_handler_rec {
 void (*__routine)(void *);
 void *__arg;
 struct __darwin_pthread_handler_rec *__next;
};

struct _opaque_pthread_attr_t {
 long __sig;
 char __opaque[56];
};

struct _opaque_pthread_cond_t {
 long __sig;
 char __opaque[40];
};

struct _opaque_pthread_condattr_t {
 long __sig;
 char __opaque[8];
};

struct _opaque_pthread_mutex_t {
 long __sig;
 char __opaque[56];
};

struct _opaque_pthread_mutexattr_t {
 long __sig;
 char __opaque[8];
};

struct _opaque_pthread_once_t {
 long __sig;
 char __opaque[8];
};

struct _opaque_pthread_rwlock_t {
 long __sig;
 char __opaque[192];
};

struct _opaque_pthread_rwlockattr_t {
 long __sig;
 char __opaque[16];
};

struct _opaque_pthread_t {
 long __sig;
 struct __darwin_pthread_handler_rec *__cleanup_stack;
 char __opaque[8176];
};

typedef struct _opaque_pthread_attr_t __darwin_pthread_attr_t;
typedef struct _opaque_pthread_cond_t __darwin_pthread_cond_t;
typedef struct _opaque_pthread_condattr_t __darwin_pthread_condattr_t;
typedef unsigned long __darwin_pthread_key_t;
typedef struct _opaque_pthread_mutex_t __darwin_pthread_mutex_t;
typedef struct _opaque_pthread_mutexattr_t __darwin_pthread_mutexattr_t;
typedef struct _opaque_pthread_once_t __darwin_pthread_once_t;
typedef struct _opaque_pthread_rwlock_t __darwin_pthread_rwlock_t;
typedef struct _opaque_pthread_rwlockattr_t __darwin_pthread_rwlockattr_t;
typedef struct _opaque_pthread_t *__darwin_pthread_t;
# 81 "/usr/include/sys/_types.h" 2 3 4
# 28 "/usr/include/_types.h" 2 3 4
# 39 "/usr/include/_types.h" 3 4
typedef int __darwin_nl_item;
typedef int __darwin_wctrans_t;

typedef __uint32_t __darwin_wctype_t;
# 68 "/usr/include/stdio.h" 2



# 1 "/usr/include/sys/_types/_va_list.h" 1 3 4
# 31 "/usr/include/sys/_types/_va_list.h" 3 4
typedef __darwin_va_list va_list;
# 72 "/usr/include/stdio.h" 2
# 1 "/usr/include/sys/_types/_size_t.h" 1 3 4
# 30 "/usr/include/sys/_types/_size_t.h" 3 4
typedef __darwin_size_t size_t;
# 73 "/usr/include/stdio.h" 2
# 1 "/usr/include/sys/_types/_null.h" 1 3 4
# 74 "/usr/include/stdio.h" 2

# 1 "/usr/include/sys/stdio.h" 1 3 4
# 37 "/usr/include/sys/stdio.h" 3 4


int renameat(int, const char *, int, const char *) ;






int renamex_np(const char *, const char *, unsigned int) ;
int renameatx_np(int, const char *, int, const char *, unsigned int) ;




# 76 "/usr/include/stdio.h" 2


# 77 "/usr/include/stdio.h"
typedef __darwin_off_t fpos_t;
# 88 "/usr/include/stdio.h"
struct __sbuf {
 unsigned char *_base;
 int _size;
};


struct __sFILEX;
# 122 "/usr/include/stdio.h"
typedef struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 void *_cookie;
 int (* _close)(void *);
 int (* _read) (void *, char *, int);
 fpos_t (* _seek) (void *, fpos_t, int);
 int (* _write)(void *, const char *, int);


 struct __sbuf _ub;
 struct __sFILEX *_extra;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 fpos_t _offset;
} FILE;


extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

# 230 "/usr/include/stdio.h"

void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * , fpos_t *);
char *fgets(char * , int, FILE *);



FILE *fopen(const char * __filename, const char * __mode) 
# 242 "/usr/include/stdio.h" 3 4
                                                                               __asm("_" 
# 242 "/usr/include/stdio.h"
                                                                               "fopen" 
# 242 "/usr/include/stdio.h" 3 4
                                                                               )
# 242 "/usr/include/stdio.h"
                                                                                                                                                       ;

int fprintf(FILE * , const char * , ...) 
# 244 "/usr/include/stdio.h" 3 4
                                                             __attribute__((__format__ (__printf__, 
# 244 "/usr/include/stdio.h"
                                                             2
# 244 "/usr/include/stdio.h" 3 4
                                                             , 
# 244 "/usr/include/stdio.h"
                                                             3
# 244 "/usr/include/stdio.h" 3 4
                                                             )))
# 244 "/usr/include/stdio.h"
                                                                               ;
int fputc(int, FILE *);
int fputs(const char * , FILE * ) 
# 246 "/usr/include/stdio.h" 3 4
                                                      __asm("_" 
# 246 "/usr/include/stdio.h"
                                                      "fputs" 
# 246 "/usr/include/stdio.h" 3 4
                                                      )
# 246 "/usr/include/stdio.h"
                                                                           ;
size_t fread(void * __ptr, size_t __size, size_t __nitems, FILE * __stream);
FILE *freopen(const char * , const char * ,
                 FILE * ) 
# 249 "/usr/include/stdio.h" 3 4
                                   __asm("_" 
# 249 "/usr/include/stdio.h"
                                   "freopen" 
# 249 "/usr/include/stdio.h" 3 4
                                   )
# 249 "/usr/include/stdio.h"
                                                          ;
int fscanf(FILE * , const char * , ...) 
# 250 "/usr/include/stdio.h" 3 4
                                                            __attribute__((__format__ (__scanf__, 
# 250 "/usr/include/stdio.h"
                                                            2
# 250 "/usr/include/stdio.h" 3 4
                                                            , 
# 250 "/usr/include/stdio.h"
                                                            3
# 250 "/usr/include/stdio.h" 3 4
                                                            )))
# 250 "/usr/include/stdio.h"
                                                                             ;
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * __ptr, size_t __size, size_t __nitems, FILE * __stream) 
# 254 "/usr/include/stdio.h" 3 4
                                                                                                         __asm("_" 
# 254 "/usr/include/stdio.h"
                                                                                                         "fwrite" 
# 254 "/usr/include/stdio.h" 3 4
                                                                                                         )
# 254 "/usr/include/stdio.h"
                                                                                                                               ;
int getc(FILE *);
int getchar(void);
char *gets(char *);
void perror(const char *);
int printf(const char * , ...) 
# 259 "/usr/include/stdio.h" 3 4
                                         __attribute__((__format__ (__printf__, 
# 259 "/usr/include/stdio.h"
                                         1
# 259 "/usr/include/stdio.h" 3 4
                                         , 
# 259 "/usr/include/stdio.h"
                                         2
# 259 "/usr/include/stdio.h" 3 4
                                         )))
# 259 "/usr/include/stdio.h"
                                                           ;
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename (const char *__old, const char *__new);
void rewind(FILE *);
int scanf(const char * , ...) 
# 266 "/usr/include/stdio.h" 3 4
                                        __attribute__((__format__ (__scanf__, 
# 266 "/usr/include/stdio.h"
                                        1
# 266 "/usr/include/stdio.h" 3 4
                                        , 
# 266 "/usr/include/stdio.h"
                                        2
# 266 "/usr/include/stdio.h" 3 4
                                        )))
# 266 "/usr/include/stdio.h"
                                                         ;
void setbuf(FILE * , char * );
int setvbuf(FILE * , char * , int, size_t);
int sprintf(char * , const char * , ...) 
# 269 "/usr/include/stdio.h" 3 4
                                                             __attribute__((__format__ (__printf__, 
# 269 "/usr/include/stdio.h"
                                                             2
# 269 "/usr/include/stdio.h" 3 4
                                                             , 
# 269 "/usr/include/stdio.h"
                                                             3
# 269 "/usr/include/stdio.h" 3 4
                                                             ))) 
# 269 "/usr/include/stdio.h"
                                                                                                                            ;
int sscanf(const char * , const char * , ...) 
# 270 "/usr/include/stdio.h" 3 4
                                                                  __attribute__((__format__ (__scanf__, 
# 270 "/usr/include/stdio.h"
                                                                  2
# 270 "/usr/include/stdio.h" 3 4
                                                                  , 
# 270 "/usr/include/stdio.h"
                                                                  3
# 270 "/usr/include/stdio.h" 3 4
                                                                  )))
# 270 "/usr/include/stdio.h"
                                                                                   ;
FILE *tmpfile(void);




# 275 "/usr/include/stdio.h" 3 4
__attribute__((deprecated(
# 275 "/usr/include/stdio.h"
"This function is provided for compatibility reasons only.  Due to security concerns inherent in the design of tmpnam(3), it is highly recommended that you use mkstemp(3) instead."
# 275 "/usr/include/stdio.h" 3 4
)))


# 277 "/usr/include/stdio.h"
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * , const char * , va_list) 
# 279 "/usr/include/stdio.h" 3 4
                                                                  __attribute__((__format__ (__printf__, 
# 279 "/usr/include/stdio.h"
                                                                  2
# 279 "/usr/include/stdio.h" 3 4
                                                                  , 
# 279 "/usr/include/stdio.h"
                                                                  0
# 279 "/usr/include/stdio.h" 3 4
                                                                  )))
# 279 "/usr/include/stdio.h"
                                                                                    ;
int vprintf(const char * , va_list) 
# 280 "/usr/include/stdio.h" 3 4
                                              __attribute__((__format__ (__printf__, 
# 280 "/usr/include/stdio.h"
                                              1
# 280 "/usr/include/stdio.h" 3 4
                                              , 
# 280 "/usr/include/stdio.h"
                                              0
# 280 "/usr/include/stdio.h" 3 4
                                              )))
# 280 "/usr/include/stdio.h"
                                                                ;
int vsprintf(char * , const char * , va_list) 
# 281 "/usr/include/stdio.h" 3 4
                                                                  __attribute__((__format__ (__printf__, 
# 281 "/usr/include/stdio.h"
                                                                  2
# 281 "/usr/include/stdio.h" 3 4
                                                                  , 
# 281 "/usr/include/stdio.h"
                                                                  0
# 281 "/usr/include/stdio.h" 3 4
                                                                  ))) 
# 281 "/usr/include/stdio.h"
                                                                                                                                  ;

# 293 "/usr/include/stdio.h"




char *ctermid(char *);





FILE *fdopen(int, const char *) 
# 303 "/usr/include/stdio.h" 3 4
                               __asm("_" 
# 303 "/usr/include/stdio.h"
                               "fdopen" 
# 303 "/usr/include/stdio.h" 3 4
                               )
# 303 "/usr/include/stdio.h"
                                                                                                        ;

int fileno(FILE *);

# 320 "/usr/include/stdio.h"

int pclose(FILE *) ;



FILE *popen(const char *, const char *) 
# 325 "/usr/include/stdio.h" 3 4
                                       __asm("_" 
# 325 "/usr/include/stdio.h"
                                       "popen" 
# 325 "/usr/include/stdio.h" 3 4
                                       ) 
# 325 "/usr/include/stdio.h"
                                                                                                                                                                                                                     ;


# 341 "/usr/include/stdio.h"

int __srget(FILE *);
int __svfscanf(FILE *, const char *, va_list) 
# 343 "/usr/include/stdio.h" 3 4
                                             __attribute__((__format__ (__scanf__, 
# 343 "/usr/include/stdio.h"
                                             2
# 343 "/usr/include/stdio.h" 3 4
                                             , 
# 343 "/usr/include/stdio.h"
                                             0
# 343 "/usr/include/stdio.h" 3 4
                                             )))
# 343 "/usr/include/stdio.h"
                                                              ;
int __swbuf(int, FILE *);









# 353 "/usr/include/stdio.h" 3 4
extern __inline __attribute__((__gnu_inline__)) __attribute__ ((__always_inline__)) 
# 353 "/usr/include/stdio.h"
                      int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
# 378 "/usr/include/stdio.h"

void flockfile(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);



int getw(FILE *);
int putw(int, FILE *);





# 395 "/usr/include/stdio.h" 3 4
__attribute__((deprecated(
# 395 "/usr/include/stdio.h"
"This function is provided for compatibility reasons only.  Due to security concerns inherent in the design of tempnam(3), it is highly recommended that you use mkstemp(3) instead."
# 395 "/usr/include/stdio.h" 3 4
)))


# 397 "/usr/include/stdio.h"
char *tempnam(const char *__dir, const char *__prefix) 
# 397 "/usr/include/stdio.h" 3 4
                                                      __asm("_" 
# 397 "/usr/include/stdio.h"
                                                      "tempnam" 
# 397 "/usr/include/stdio.h" 3 4
                                                      )
# 397 "/usr/include/stdio.h"
                                                                             ;

# 417 "/usr/include/stdio.h"
# 1 "/usr/include/sys/_types/_off_t.h" 1 3 4
# 30 "/usr/include/sys/_types/_off_t.h" 3 4

# 30 "/usr/include/sys/_types/_off_t.h" 3 4
typedef __darwin_off_t off_t;
# 418 "/usr/include/stdio.h" 2



# 420 "/usr/include/stdio.h"
int fseeko(FILE * __stream, off_t __offset, int __whence);
off_t ftello(FILE * __stream);





int snprintf(char * __str, size_t __size, const char * __format, ...) 
# 427 "/usr/include/stdio.h" 3 4
                                                                                            __attribute__((__format__ (__printf__, 
# 427 "/usr/include/stdio.h"
                                                                                            3
# 427 "/usr/include/stdio.h" 3 4
                                                                                            , 
# 427 "/usr/include/stdio.h"
                                                                                            4
# 427 "/usr/include/stdio.h" 3 4
                                                                                            )))
# 427 "/usr/include/stdio.h"
                                                                                                              ;
int vfscanf(FILE * __stream, const char * __format, va_list) 
# 428 "/usr/include/stdio.h" 3 4
                                                                                   __attribute__((__format__ (__scanf__, 
# 428 "/usr/include/stdio.h"
                                                                                   2
# 428 "/usr/include/stdio.h" 3 4
                                                                                   , 
# 428 "/usr/include/stdio.h"
                                                                                   0
# 428 "/usr/include/stdio.h" 3 4
                                                                                   )))
# 428 "/usr/include/stdio.h"
                                                                                                    ;
int vscanf(const char * __format, va_list) 
# 429 "/usr/include/stdio.h" 3 4
                                                      __attribute__((__format__ (__scanf__, 
# 429 "/usr/include/stdio.h"
                                                      1
# 429 "/usr/include/stdio.h" 3 4
                                                      , 
# 429 "/usr/include/stdio.h"
                                                      0
# 429 "/usr/include/stdio.h" 3 4
                                                      )))
# 429 "/usr/include/stdio.h"
                                                                       ;
int vsnprintf(char * __str, size_t __size, const char * __format, va_list) 
# 430 "/usr/include/stdio.h" 3 4
                                                                                                 __attribute__((__format__ (__printf__, 
# 430 "/usr/include/stdio.h"
                                                                                                 3
# 430 "/usr/include/stdio.h" 3 4
                                                                                                 , 
# 430 "/usr/include/stdio.h"
                                                                                                 0
# 430 "/usr/include/stdio.h" 3 4
                                                                                                 )))
# 430 "/usr/include/stdio.h"
                                                                                                                   ;
int vsscanf(const char * __str, const char * __format, va_list) 
# 431 "/usr/include/stdio.h" 3 4
                                                                                      __attribute__((__format__ (__scanf__, 
# 431 "/usr/include/stdio.h"
                                                                                      2
# 431 "/usr/include/stdio.h" 3 4
                                                                                      , 
# 431 "/usr/include/stdio.h"
                                                                                      0
# 431 "/usr/include/stdio.h" 3 4
                                                                                      )))
# 431 "/usr/include/stdio.h"
                                                                                                       ;

# 442 "/usr/include/stdio.h"
# 1 "/usr/include/sys/_types/_ssize_t.h" 1 3 4
# 30 "/usr/include/sys/_types/_ssize_t.h" 3 4

# 30 "/usr/include/sys/_types/_ssize_t.h" 3 4
typedef __darwin_ssize_t ssize_t;
# 443 "/usr/include/stdio.h" 2



# 445 "/usr/include/stdio.h"
int dprintf(int, const char * , ...) 
# 445 "/usr/include/stdio.h" 3 4
                                              __attribute__((__format__ (__printf__, 
# 445 "/usr/include/stdio.h"
                                              2
# 445 "/usr/include/stdio.h" 3 4
                                              , 
# 445 "/usr/include/stdio.h"
                                              3
# 445 "/usr/include/stdio.h" 3 4
                                              ))) 
# 445 "/usr/include/stdio.h"
                                                                                                                   ;
int vdprintf(int, const char * , va_list) 
# 446 "/usr/include/stdio.h" 3 4
                                                   __attribute__((__format__ (__printf__, 
# 446 "/usr/include/stdio.h"
                                                   2
# 446 "/usr/include/stdio.h" 3 4
                                                   , 
# 446 "/usr/include/stdio.h"
                                                   0
# 446 "/usr/include/stdio.h" 3 4
                                                   ))) 
# 446 "/usr/include/stdio.h"
                                                                                                                        ;
ssize_t getdelim(char ** __linep, size_t * __linecapp, int __delimiter, FILE * __stream) ;
ssize_t getline(char ** __linep, size_t * __linecapp, FILE * __stream) ;









extern 
# 458 "/usr/include/stdio.h" 3 4
      const 
# 458 "/usr/include/stdio.h"
              int sys_nerr;
extern 
# 459 "/usr/include/stdio.h" 3 4
      const 
# 459 "/usr/include/stdio.h"
              char *
# 459 "/usr/include/stdio.h" 3 4
                    const 
# 459 "/usr/include/stdio.h"
                            sys_errlist[];

int asprintf(char ** , const char * , ...) 
# 461 "/usr/include/stdio.h" 3 4
                                                               __attribute__((__format__ (__printf__, 
# 461 "/usr/include/stdio.h"
                                                               2
# 461 "/usr/include/stdio.h" 3 4
                                                               , 
# 461 "/usr/include/stdio.h"
                                                               3
# 461 "/usr/include/stdio.h" 3 4
                                                               )))
# 461 "/usr/include/stdio.h"
                                                                                 ;
char *ctermid_r(char *);
char *fgetln(FILE *, size_t *);

# 464 "/usr/include/stdio.h" 3 4
const 
# 464 "/usr/include/stdio.h"
       char *fmtcheck(const char *, const char *);
int fpurge(FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);
int vasprintf(char ** , const char * , va_list) 
# 468 "/usr/include/stdio.h" 3 4
                                                                    __attribute__((__format__ (__printf__, 
# 468 "/usr/include/stdio.h"
                                                                    2
# 468 "/usr/include/stdio.h" 3 4
                                                                    , 
# 468 "/usr/include/stdio.h"
                                                                    0
# 468 "/usr/include/stdio.h" 3 4
                                                                    )))
# 468 "/usr/include/stdio.h"
                                                                                      ;
FILE *zopen(const char *, const char *, int);





FILE *funopen(const void *,
                 int (* )(void *, char *, int),
                 int (* )(void *, const char *, int),
                 fpos_t (* )(void *, fpos_t, int),
                 int (* )(void *));

# 498 "/usr/include/stdio.h"
# 1 "/usr/include/secure/_stdio.h" 1 3 4
# 31 "/usr/include/secure/_stdio.h" 3 4
# 1 "/usr/include/secure/_common.h" 1 3 4
# 32 "/usr/include/secure/_stdio.h" 2 3 4
# 42 "/usr/include/secure/_stdio.h" 3 4

# 42 "/usr/include/secure/_stdio.h" 3 4
extern int __sprintf_chk (char * , int, size_t,
     const char * , ...);
# 52 "/usr/include/secure/_stdio.h" 3 4
extern int __snprintf_chk (char * , size_t, int, size_t,
      const char * , ...);







extern int __vsprintf_chk (char * , int, size_t,
      const char * , va_list);







extern int __vsnprintf_chk (char * , size_t, int, size_t,
       const char * , va_list);
# 499 "/usr/include/stdio.h" 2
