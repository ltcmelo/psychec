# 1 "/usr/include/stdlib.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/usr/include/stdlib.h"
# 24 "/usr/include/stdlib.h"
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
# 25 "/usr/include/stdlib.h" 2







# 1 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 1 3 4
# 216 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 3 4

# 216 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 328 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 3 4
typedef int wchar_t;
# 33 "/usr/include/stdlib.h" 2








# 1 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 1 3 4
# 50 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4
typedef enum
{
  P_ALL,
  P_PID,
  P_PGID
} idtype_t;
# 42 "/usr/include/stdlib.h" 2
# 1 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 1 3 4
# 64 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 36 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 2 3 4
# 60 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;







typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
# 121 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 1 3 4
# 122 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;
# 28 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 29 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 2 3 4






# 1 "/usr/include/x86_64-linux-gnu/bits/byteswap-16.h" 1 3 4
# 36 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 2 3 4
# 44 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
static __inline unsigned int
__bswap_32 (unsigned int __bsx)
{
  return __builtin_bswap32 (__bsx);
}
# 108 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{
  return __builtin_bswap64 (__bsx);
}
# 61 "/usr/include/endian.h" 2 3 4
# 65 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 2 3 4

union wait
  {
    int w_status;
    struct
      {

 unsigned int __w_termsig:7;
 unsigned int __w_coredump:1;
 unsigned int __w_retcode:8;
 unsigned int:16;







      } __wait_terminated;
    struct
      {

 unsigned int __w_stopval:8;
 unsigned int __w_stopsig:8;
 unsigned int:16;






      } __wait_stopped;
  };
# 43 "/usr/include/stdlib.h" 2
# 67 "/usr/include/stdlib.h"

# 67 "/usr/include/stdlib.h"
typedef union
  {
    union wait *__uptr;
    int *__iptr;
  } __WAIT_STATUS __attribute__ ((__transparent_union__));
# 95 "/usr/include/stdlib.h"


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;







 typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;


# 139 "/usr/include/stdlib.h"
extern size_t __ctype_get_mb_cur_max (void) 
# 139 "/usr/include/stdlib.h" 3 4
                                           __attribute__ ((__nothrow__ , __leaf__)) 
# 139 "/usr/include/stdlib.h"
                                                        ;




extern double atof (const char *__nptr)
     
# 145 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 145 "/usr/include/stdlib.h"
                               (1)
# 145 "/usr/include/stdlib.h" 3 4
                               )) 
# 145 "/usr/include/stdlib.h"
                                                    ;

extern int atoi (const char *__nptr)
     
# 148 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 148 "/usr/include/stdlib.h"
                               (1)
# 148 "/usr/include/stdlib.h" 3 4
                               )) 
# 148 "/usr/include/stdlib.h"
                                                    ;

extern long int atol (const char *__nptr)
     
# 151 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 151 "/usr/include/stdlib.h"
                               (1)
# 151 "/usr/include/stdlib.h" 3 4
                               )) 
# 151 "/usr/include/stdlib.h"
                                                    ;





 extern long long int atoll (const char *__nptr)
     
# 158 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 158 "/usr/include/stdlib.h"
                               (1)
# 158 "/usr/include/stdlib.h" 3 4
                               )) 
# 158 "/usr/include/stdlib.h"
                                                    ;





extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     
# 166 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 166 "/usr/include/stdlib.h"
            (1)
# 166 "/usr/include/stdlib.h" 3 4
            ))
# 166 "/usr/include/stdlib.h"
                           ;





extern float strtof (const char *__restrict __nptr,
       char **__restrict __endptr) 
# 173 "/usr/include/stdlib.h" 3 4
                                  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 173 "/usr/include/stdlib.h"
                                          (1)
# 173 "/usr/include/stdlib.h" 3 4
                                          ))
# 173 "/usr/include/stdlib.h"
                                                         ;

extern long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr)
     
# 177 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 177 "/usr/include/stdlib.h"
            (1)
# 177 "/usr/include/stdlib.h" 3 4
            ))
# 177 "/usr/include/stdlib.h"
                           ;





extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     
# 185 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 185 "/usr/include/stdlib.h"
            (1)
# 185 "/usr/include/stdlib.h" 3 4
            ))
# 185 "/usr/include/stdlib.h"
                           ;

extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     
# 189 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 189 "/usr/include/stdlib.h"
            (1)
# 189 "/usr/include/stdlib.h" 3 4
            ))
# 189 "/usr/include/stdlib.h"
                           ;





extern long long int strtoq (const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     
# 197 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 197 "/usr/include/stdlib.h"
            (1)
# 197 "/usr/include/stdlib.h" 3 4
            ))
# 197 "/usr/include/stdlib.h"
                           ;


extern unsigned long long int strtouq (const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     
# 202 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 202 "/usr/include/stdlib.h"
            (1)
# 202 "/usr/include/stdlib.h" 3 4
            ))
# 202 "/usr/include/stdlib.h"
                           ;






extern long long int strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     
# 211 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 211 "/usr/include/stdlib.h"
            (1)
# 211 "/usr/include/stdlib.h" 3 4
            ))
# 211 "/usr/include/stdlib.h"
                           ;


extern unsigned long long int strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     
# 216 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 216 "/usr/include/stdlib.h"
            (1)
# 216 "/usr/include/stdlib.h" 3 4
            ))
# 216 "/usr/include/stdlib.h"
                           ;

# 305 "/usr/include/stdlib.h"
extern char *l64a (long int __n) 
# 305 "/usr/include/stdlib.h" 3 4
                                __attribute__ ((__nothrow__ , __leaf__)) 
# 305 "/usr/include/stdlib.h"
                                             ;


extern long int a64l (const char *__s)
     
# 309 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ 
# 309 "/usr/include/stdlib.h"
                               (1)
# 309 "/usr/include/stdlib.h" 3 4
                               )) 
# 309 "/usr/include/stdlib.h"
                                                    ;




# 1 "/usr/include/x86_64-linux-gnu/sys/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4







# 33 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
# 60 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
# 98 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 132 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 57 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



# 73 "/usr/include/time.h" 3 4


typedef __time_t time_t;



# 91 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;
# 103 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;
# 133 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
# 146 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 1 3 4
# 147 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 194 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 219 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/select.h" 1 3 4
# 30 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/select.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/select.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/select.h" 2 3 4
# 31 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/sigset.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 34 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





# 1 "/usr/include/time.h" 1 3 4
# 120 "/usr/include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    __syscall_slong_t tv_nsec;
  };
# 44 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/time.h" 1 3 4
# 30 "/usr/include/x86_64-linux-gnu/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 46 "/usr/include/x86_64-linux-gnu/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 64 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 96 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4

# 106 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 118 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 131 "/usr/include/x86_64-linux-gnu/sys/select.h" 3 4

# 220 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/sys/sysmacros.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/sys/sysmacros.h" 3 4



extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));

extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));

extern unsigned long long int gnu_dev_makedev (unsigned int __major,
            unsigned int __minor)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 58 "/usr/include/x86_64-linux-gnu/sys/sysmacros.h" 3 4

# 223 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 270 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 1 3 4
# 21 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 22 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 2 3 4
# 60 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


union pthread_attr_t
{
  char __size[56];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;





typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
# 90 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;

    unsigned int __nusers;



    int __kind;

    short __spins;
    short __elision;
    __pthread_list_t __list;
# 125 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
  } __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    unsigned long long int __total_seq;
    unsigned long long int __wakeup_seq;
    unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{

  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
    int __writer;
    int __shared;
    signed char __rwelision;




    unsigned char __pad1[7];


    unsigned long int __pad2;


    unsigned int __flags;

  } __data;
# 220 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h" 3 4
  char __size[56];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[32];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 271 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4



# 315 "/usr/include/stdlib.h" 2







# 321 "/usr/include/stdlib.h"
extern long int random (void) 
# 321 "/usr/include/stdlib.h" 3 4
                             __attribute__ ((__nothrow__ , __leaf__))
# 321 "/usr/include/stdlib.h"
                                    ;


extern void srandom (unsigned int __seed) 
# 324 "/usr/include/stdlib.h" 3 4
                                         __attribute__ ((__nothrow__ , __leaf__))
# 324 "/usr/include/stdlib.h"
                                                ;





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) 
# 331 "/usr/include/stdlib.h" 3 4
                     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 331 "/usr/include/stdlib.h"
                             (2)
# 331 "/usr/include/stdlib.h" 3 4
                             ))
# 331 "/usr/include/stdlib.h"
                                            ;



extern char *setstate (char *__statebuf) 
# 335 "/usr/include/stdlib.h" 3 4
                                        __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 335 "/usr/include/stdlib.h"
                                                (1)
# 335 "/usr/include/stdlib.h" 3 4
                                                ))
# 335 "/usr/include/stdlib.h"
                                                               ;







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) 
# 355 "/usr/include/stdlib.h" 3 4
                                    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 355 "/usr/include/stdlib.h"
                                            (1, 2)
# 355 "/usr/include/stdlib.h" 3 4
                                            ))
# 355 "/usr/include/stdlib.h"
                                                              ;

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     
# 358 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 358 "/usr/include/stdlib.h"
            (2)
# 358 "/usr/include/stdlib.h" 3 4
            ))
# 358 "/usr/include/stdlib.h"
                           ;

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     
# 363 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 363 "/usr/include/stdlib.h"
            (2, 4)
# 363 "/usr/include/stdlib.h" 3 4
            ))
# 363 "/usr/include/stdlib.h"
                              ;

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     
# 367 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 367 "/usr/include/stdlib.h"
            (1, 2)
# 367 "/usr/include/stdlib.h" 3 4
            ))
# 367 "/usr/include/stdlib.h"
                              ;






extern int rand (void) 
# 374 "/usr/include/stdlib.h" 3 4
                      __attribute__ ((__nothrow__ , __leaf__))
# 374 "/usr/include/stdlib.h"
                             ;

extern void srand (unsigned int __seed) 
# 376 "/usr/include/stdlib.h" 3 4
                                       __attribute__ ((__nothrow__ , __leaf__))
# 376 "/usr/include/stdlib.h"
                                              ;




extern int rand_r (unsigned int *__seed) 
# 381 "/usr/include/stdlib.h" 3 4
                                        __attribute__ ((__nothrow__ , __leaf__))
# 381 "/usr/include/stdlib.h"
                                               ;







extern double drand48 (void) 
# 389 "/usr/include/stdlib.h" 3 4
                            __attribute__ ((__nothrow__ , __leaf__))
# 389 "/usr/include/stdlib.h"
                                   ;
extern double erand48 (unsigned short int __xsubi[3]) 
# 390 "/usr/include/stdlib.h" 3 4
                                                     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 390 "/usr/include/stdlib.h"
                                                             (1)
# 390 "/usr/include/stdlib.h" 3 4
                                                             ))
# 390 "/usr/include/stdlib.h"
                                                                            ;


extern long int lrand48 (void) 
# 393 "/usr/include/stdlib.h" 3 4
                              __attribute__ ((__nothrow__ , __leaf__))
# 393 "/usr/include/stdlib.h"
                                     ;
extern long int nrand48 (unsigned short int __xsubi[3])
     
# 395 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 395 "/usr/include/stdlib.h"
            (1)
# 395 "/usr/include/stdlib.h" 3 4
            ))
# 395 "/usr/include/stdlib.h"
                           ;


extern long int mrand48 (void) 
# 398 "/usr/include/stdlib.h" 3 4
                              __attribute__ ((__nothrow__ , __leaf__))
# 398 "/usr/include/stdlib.h"
                                     ;
extern long int jrand48 (unsigned short int __xsubi[3])
     
# 400 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 400 "/usr/include/stdlib.h"
            (1)
# 400 "/usr/include/stdlib.h" 3 4
            ))
# 400 "/usr/include/stdlib.h"
                           ;


extern void srand48 (long int __seedval) 
# 403 "/usr/include/stdlib.h" 3 4
                                        __attribute__ ((__nothrow__ , __leaf__))
# 403 "/usr/include/stdlib.h"
                                               ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     
# 405 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 405 "/usr/include/stdlib.h"
            (1)
# 405 "/usr/include/stdlib.h" 3 4
            ))
# 405 "/usr/include/stdlib.h"
                           ;
extern void lcong48 (unsigned short int __param[7]) 
# 406 "/usr/include/stdlib.h" 3 4
                                                   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 406 "/usr/include/stdlib.h"
                                                           (1)
# 406 "/usr/include/stdlib.h" 3 4
                                                           ))
# 406 "/usr/include/stdlib.h"
                                                                          ;





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;

  };


extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) 
# 424 "/usr/include/stdlib.h" 3 4
                                    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 424 "/usr/include/stdlib.h"
                                            (1, 2)
# 424 "/usr/include/stdlib.h" 3 4
                                            ))
# 424 "/usr/include/stdlib.h"
                                                              ;
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) 
# 427 "/usr/include/stdlib.h" 3 4
                                    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 427 "/usr/include/stdlib.h"
                                            (1, 2)
# 427 "/usr/include/stdlib.h" 3 4
                                            ))
# 427 "/usr/include/stdlib.h"
                                                              ;


extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     
# 432 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 432 "/usr/include/stdlib.h"
            (1, 2)
# 432 "/usr/include/stdlib.h" 3 4
            ))
# 432 "/usr/include/stdlib.h"
                              ;
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     
# 436 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 436 "/usr/include/stdlib.h"
            (1, 2)
# 436 "/usr/include/stdlib.h" 3 4
            ))
# 436 "/usr/include/stdlib.h"
                              ;


extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     
# 441 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 441 "/usr/include/stdlib.h"
            (1, 2)
# 441 "/usr/include/stdlib.h" 3 4
            ))
# 441 "/usr/include/stdlib.h"
                              ;
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     
# 445 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 445 "/usr/include/stdlib.h"
            (1, 2)
# 445 "/usr/include/stdlib.h" 3 4
            ))
# 445 "/usr/include/stdlib.h"
                              ;


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     
# 449 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 449 "/usr/include/stdlib.h"
            (2)
# 449 "/usr/include/stdlib.h" 3 4
            ))
# 449 "/usr/include/stdlib.h"
                           ;

extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) 
# 452 "/usr/include/stdlib.h" 3 4
                                     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 452 "/usr/include/stdlib.h"
                                             (1, 2)
# 452 "/usr/include/stdlib.h" 3 4
                                             ))
# 452 "/usr/include/stdlib.h"
                                                               ;

extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer)
     
# 456 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 456 "/usr/include/stdlib.h"
            (1, 2)
# 456 "/usr/include/stdlib.h" 3 4
            ))
# 456 "/usr/include/stdlib.h"
                              ;









extern void *malloc (size_t __size) 
# 466 "/usr/include/stdlib.h" 3 4
                                   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) 
# 466 "/usr/include/stdlib.h"
                                                                     ;

extern void *calloc (size_t __nmemb, size_t __size)
     
# 469 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) 
# 469 "/usr/include/stdlib.h"
                                      ;










extern void *realloc (void *__ptr, size_t __size)
     
# 481 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__))
# 481 "/usr/include/stdlib.h"
                                            ;

extern void free (void *__ptr) 
# 483 "/usr/include/stdlib.h" 3 4
                              __attribute__ ((__nothrow__ , __leaf__))
# 483 "/usr/include/stdlib.h"
                                     ;




extern void cfree (void *__ptr) 
# 488 "/usr/include/stdlib.h" 3 4
                               __attribute__ ((__nothrow__ , __leaf__))
# 488 "/usr/include/stdlib.h"
                                      ;



# 1 "/usr/include/alloca.h" 1 3 4
# 24 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h" 1 3 4
# 25 "/usr/include/alloca.h" 2 3 4








# 32 "/usr/include/alloca.h" 3 4
extern void *alloca (size_t __size) __attribute__ ((__nothrow__ , __leaf__));






# 493 "/usr/include/stdlib.h" 2






# 498 "/usr/include/stdlib.h"
extern void *valloc (size_t __size) 
# 498 "/usr/include/stdlib.h" 3 4
                                   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) 
# 498 "/usr/include/stdlib.h"
                                                                     ;




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     
# 504 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 504 "/usr/include/stdlib.h"
            (1)
# 504 "/usr/include/stdlib.h" 3 4
            )) 
# 504 "/usr/include/stdlib.h"
                                 ;




extern void *aligned_alloc (size_t __alignment, size_t __size)
     
# 510 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__alloc_size__ 
# 510 "/usr/include/stdlib.h"
                                 (2)
# 510 "/usr/include/stdlib.h" 3 4
                                 )) 
# 510 "/usr/include/stdlib.h"
                                                                     ;




extern void abort (void) 
# 515 "/usr/include/stdlib.h" 3 4
                        __attribute__ ((__nothrow__ , __leaf__)) 
# 515 "/usr/include/stdlib.h"
                                __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) 
# 519 "/usr/include/stdlib.h" 3 4
                                         __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 519 "/usr/include/stdlib.h"
                                                 (1)
# 519 "/usr/include/stdlib.h" 3 4
                                                 ))
# 519 "/usr/include/stdlib.h"
                                                                ;







extern int at_quick_exit (void (*__func) (void)) 
# 527 "/usr/include/stdlib.h" 3 4
                                                __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 527 "/usr/include/stdlib.h"
                                                        (1)
# 527 "/usr/include/stdlib.h" 3 4
                                                        ))
# 527 "/usr/include/stdlib.h"
                                                                       ;







extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     
# 536 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 536 "/usr/include/stdlib.h"
            (1)
# 536 "/usr/include/stdlib.h" 3 4
            ))
# 536 "/usr/include/stdlib.h"
                           ;






extern void exit (int __status) 
# 543 "/usr/include/stdlib.h" 3 4
                               __attribute__ ((__nothrow__ , __leaf__)) 
# 543 "/usr/include/stdlib.h"
                                       __attribute__ ((__noreturn__));





extern void quick_exit (int __status) 
# 549 "/usr/include/stdlib.h" 3 4
                                     __attribute__ ((__nothrow__ , __leaf__)) 
# 549 "/usr/include/stdlib.h"
                                             __attribute__ ((__noreturn__));







extern void _Exit (int __status) 
# 557 "/usr/include/stdlib.h" 3 4
                                __attribute__ ((__nothrow__ , __leaf__)) 
# 557 "/usr/include/stdlib.h"
                                        __attribute__ ((__noreturn__));






extern char *getenv (const char *__name) 
# 564 "/usr/include/stdlib.h" 3 4
                                        __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 564 "/usr/include/stdlib.h"
                                                (1)
# 564 "/usr/include/stdlib.h" 3 4
                                                )) 
# 564 "/usr/include/stdlib.h"
                                                                     ;

# 578 "/usr/include/stdlib.h"
extern int putenv (char *__string) 
# 578 "/usr/include/stdlib.h" 3 4
                                  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 578 "/usr/include/stdlib.h"
                                          (1)
# 578 "/usr/include/stdlib.h" 3 4
                                          ))
# 578 "/usr/include/stdlib.h"
                                                         ;





extern int setenv (const char *__name, const char *__value, int __replace)
     
# 585 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 585 "/usr/include/stdlib.h"
            (2)
# 585 "/usr/include/stdlib.h" 3 4
            ))
# 585 "/usr/include/stdlib.h"
                           ;


extern int unsetenv (const char *__name) 
# 588 "/usr/include/stdlib.h" 3 4
                                        __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 588 "/usr/include/stdlib.h"
                                                (1)
# 588 "/usr/include/stdlib.h" 3 4
                                                ))
# 588 "/usr/include/stdlib.h"
                                                               ;






extern int clearenv (void) 
# 595 "/usr/include/stdlib.h" 3 4
                          __attribute__ ((__nothrow__ , __leaf__))
# 595 "/usr/include/stdlib.h"
                                 ;
# 606 "/usr/include/stdlib.h"
extern char *mktemp (char *__template) 
# 606 "/usr/include/stdlib.h" 3 4
                                      __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 606 "/usr/include/stdlib.h"
                                              (1)
# 606 "/usr/include/stdlib.h" 3 4
                                              ))
# 606 "/usr/include/stdlib.h"
                                                             ;
# 619 "/usr/include/stdlib.h"
extern int mkstemp (char *__template) 
# 619 "/usr/include/stdlib.h" 3 4
                                     __attribute__ ((__nonnull__ 
# 619 "/usr/include/stdlib.h"
                                     (1)
# 619 "/usr/include/stdlib.h" 3 4
                                     )) 
# 619 "/usr/include/stdlib.h"
                                                          ;
# 641 "/usr/include/stdlib.h"
extern int mkstemps (char *__template, int __suffixlen) 
# 641 "/usr/include/stdlib.h" 3 4
                                                       __attribute__ ((__nonnull__ 
# 641 "/usr/include/stdlib.h"
                                                       (1)
# 641 "/usr/include/stdlib.h" 3 4
                                                       )) 
# 641 "/usr/include/stdlib.h"
                                                                            ;
# 662 "/usr/include/stdlib.h"
extern char *mkdtemp (char *__template) 
# 662 "/usr/include/stdlib.h" 3 4
                                       __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 662 "/usr/include/stdlib.h"
                                               (1)
# 662 "/usr/include/stdlib.h" 3 4
                                               )) 
# 662 "/usr/include/stdlib.h"
                                                                    ;
# 711 "/usr/include/stdlib.h"





extern int system (const char *__command) ;

# 733 "/usr/include/stdlib.h"
extern char *realpath (const char *__restrict __name,
         char *__restrict __resolved) 
# 734 "/usr/include/stdlib.h" 3 4
                                     __attribute__ ((__nothrow__ , __leaf__)) 
# 734 "/usr/include/stdlib.h"
                                                  ;






typedef int (*__compar_fn_t) (const void *, const void *);
# 751 "/usr/include/stdlib.h"



extern void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     
# 756 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nonnull__ 
# 756 "/usr/include/stdlib.h"
    (1, 2, 5)
# 756 "/usr/include/stdlib.h" 3 4
    )) 
# 756 "/usr/include/stdlib.h"
                               ;







extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) 
# 765 "/usr/include/stdlib.h" 3 4
                            __attribute__ ((__nonnull__ 
# 765 "/usr/include/stdlib.h"
                            (1, 4)
# 765 "/usr/include/stdlib.h" 3 4
                            ))
# 765 "/usr/include/stdlib.h"
                                              ;
# 774 "/usr/include/stdlib.h"
extern int abs (int __x) 
# 774 "/usr/include/stdlib.h" 3 4
                        __attribute__ ((__nothrow__ , __leaf__)) 
# 774 "/usr/include/stdlib.h"
                                __attribute__ ((__const__)) ;
extern long int labs (long int __x) 
# 775 "/usr/include/stdlib.h" 3 4
                                   __attribute__ ((__nothrow__ , __leaf__)) 
# 775 "/usr/include/stdlib.h"
                                           __attribute__ ((__const__)) ;



 extern long long int llabs (long long int __x)
     
# 780 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) 
# 780 "/usr/include/stdlib.h"
            __attribute__ ((__const__)) ;







extern div_t div (int __numer, int __denom)
     
# 789 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) 
# 789 "/usr/include/stdlib.h"
            __attribute__ ((__const__)) ;
extern ldiv_t ldiv (long int __numer, long int __denom)
     
# 791 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) 
# 791 "/usr/include/stdlib.h"
            __attribute__ ((__const__)) ;




 extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     
# 798 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) 
# 798 "/usr/include/stdlib.h"
            __attribute__ ((__const__)) ;

# 811 "/usr/include/stdlib.h"
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) 
# 812 "/usr/include/stdlib.h" 3 4
                            __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 812 "/usr/include/stdlib.h"
                                    (3, 4)
# 812 "/usr/include/stdlib.h" 3 4
                                    )) 
# 812 "/usr/include/stdlib.h"
                                                            ;




extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) 
# 818 "/usr/include/stdlib.h" 3 4
                            __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 818 "/usr/include/stdlib.h"
                                    (3, 4)
# 818 "/usr/include/stdlib.h" 3 4
                                    )) 
# 818 "/usr/include/stdlib.h"
                                                            ;




extern char *gcvt (double __value, int __ndigit, char *__buf)
     
# 824 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 824 "/usr/include/stdlib.h"
            (3)
# 824 "/usr/include/stdlib.h" 3 4
            )) 
# 824 "/usr/include/stdlib.h"
                                 ;




extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     
# 831 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 831 "/usr/include/stdlib.h"
            (3, 4)
# 831 "/usr/include/stdlib.h" 3 4
            )) 
# 831 "/usr/include/stdlib.h"
                                    ;
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     
# 834 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 834 "/usr/include/stdlib.h"
            (3, 4)
# 834 "/usr/include/stdlib.h" 3 4
            )) 
# 834 "/usr/include/stdlib.h"
                                    ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf)
     
# 836 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 836 "/usr/include/stdlib.h"
            (3)
# 836 "/usr/include/stdlib.h" 3 4
            )) 
# 836 "/usr/include/stdlib.h"
                                 ;




extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) 
# 843 "/usr/include/stdlib.h" 3 4
                  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 843 "/usr/include/stdlib.h"
                          (3, 4, 5)
# 843 "/usr/include/stdlib.h" 3 4
                          ))
# 843 "/usr/include/stdlib.h"
                                               ;
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) 
# 846 "/usr/include/stdlib.h" 3 4
                  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 846 "/usr/include/stdlib.h"
                          (3, 4, 5)
# 846 "/usr/include/stdlib.h" 3 4
                          ))
# 846 "/usr/include/stdlib.h"
                                               ;

extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     
# 851 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 851 "/usr/include/stdlib.h"
            (3, 4, 5)
# 851 "/usr/include/stdlib.h" 3 4
            ))
# 851 "/usr/include/stdlib.h"
                                 ;
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     
# 855 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 855 "/usr/include/stdlib.h"
            (3, 4, 5)
# 855 "/usr/include/stdlib.h" 3 4
            ))
# 855 "/usr/include/stdlib.h"
                                 ;






extern int mblen (const char *__s, size_t __n) 
# 862 "/usr/include/stdlib.h" 3 4
                                              __attribute__ ((__nothrow__ , __leaf__))
# 862 "/usr/include/stdlib.h"
                                                     ;


extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, size_t __n) 
# 866 "/usr/include/stdlib.h" 3 4
                                            __attribute__ ((__nothrow__ , __leaf__))
# 866 "/usr/include/stdlib.h"
                                                   ;


extern int wctomb (char *__s, wchar_t __wchar) 
# 869 "/usr/include/stdlib.h" 3 4
                                              __attribute__ ((__nothrow__ , __leaf__))
# 869 "/usr/include/stdlib.h"
                                                     ;



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, size_t __n) 
# 874 "/usr/include/stdlib.h" 3 4
                                          __attribute__ ((__nothrow__ , __leaf__))
# 874 "/usr/include/stdlib.h"
                                                 ;

extern size_t wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n)
     
# 878 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__))
# 878 "/usr/include/stdlib.h"
           ;








extern int rpmatch (const char *__response) 
# 887 "/usr/include/stdlib.h" 3 4
                                           __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 887 "/usr/include/stdlib.h"
                                                   (1)
# 887 "/usr/include/stdlib.h" 3 4
                                                   )) 
# 887 "/usr/include/stdlib.h"
                                                                        ;
# 898 "/usr/include/stdlib.h"
extern int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep)
     
# 901 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 901 "/usr/include/stdlib.h"
            (1, 2, 3)
# 901 "/usr/include/stdlib.h" 3 4
            )) 
# 901 "/usr/include/stdlib.h"
                                       ;
# 950 "/usr/include/stdlib.h"
extern int getloadavg (double __loadavg[], int __nelem)
     
# 951 "/usr/include/stdlib.h" 3 4
    __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ 
# 951 "/usr/include/stdlib.h"
            (1)
# 951 "/usr/include/stdlib.h" 3 4
            ))
# 951 "/usr/include/stdlib.h"
                           ;


# 1 "/usr/include/x86_64-linux-gnu/bits/stdlib-float.h" 1 3 4
# 955 "/usr/include/stdlib.h" 2
# 967 "/usr/include/stdlib.h"

