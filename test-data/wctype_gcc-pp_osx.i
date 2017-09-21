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
typedef int __darwin_nl_item;
typedef int __darwin_wctrans_t;
typedef __uint32_t __darwin_wctype_t;
typedef __darwin_wctrans_t wctrans_t;
typedef __darwin_wint_t wint_t;
typedef __darwin_wctype_t wctype_t;
typedef __darwin_size_t size_t;
typedef __darwin_ct_rune_t ct_rune_t;
typedef __darwin_rune_t rune_t;
typedef __darwin_wchar_t wchar_t;
typedef struct {
 __darwin_rune_t __min;
 __darwin_rune_t __max;
 __darwin_rune_t __map;
 __uint32_t *__types;
} _RuneEntry;
typedef struct {
 int __nranges;
 _RuneEntry *__ranges;
} _RuneRange;
typedef struct {
 char __name[14];
 __uint32_t __mask;
} _RuneCharClass;
typedef struct {
 char __magic[8];
 char __encoding[32];
 __darwin_rune_t (*__sgetrune)(const char *, __darwin_size_t, char const **);
 int (*__sputrune)(__darwin_rune_t, char *, __darwin_size_t, char **);
 __darwin_rune_t __invalid_rune;
 __uint32_t __runetype[(1 <<8 )];
 __darwin_rune_t __maplower[(1 <<8 )];
 __darwin_rune_t __mapupper[(1 <<8 )];
 _RuneRange __runetype_ext;
 _RuneRange __maplower_ext;
 _RuneRange __mapupper_ext;
 void *__variable;
 int __variable_len;
 int __ncharclasses;
 _RuneCharClass *__charclasses;
} _RuneLocale;

extern _RuneLocale _DefaultRuneLocale;
extern _RuneLocale *_CurrentRuneLocale;


unsigned long ___runetype(__darwin_ct_rune_t);
__darwin_ct_rune_t ___tolower(__darwin_ct_rune_t);
__darwin_ct_rune_t ___toupper(__darwin_ct_rune_t);

extern __inline __attribute__((__gnu_inline__)) int
isascii(int _c)
{
 return ((_c & ~0x7F) == 0);
}

int __maskrune(__darwin_ct_rune_t, unsigned long);

extern __inline __attribute__((__gnu_inline__)) int
__istype(__darwin_ct_rune_t _c, unsigned long _f)
{
 return (isascii(_c) ? !!(_DefaultRuneLocale.__runetype[_c] & _f)
  : !!__maskrune(_c, _f));
}
extern __inline __attribute__((__gnu_inline__)) __darwin_ct_rune_t
__isctype(__darwin_ct_rune_t _c, unsigned long _f)
{
 return (_c < 0 || _c >= (1 <<8 )) ? 0 :
  !!(_DefaultRuneLocale.__runetype[_c] & _f);
}

__darwin_ct_rune_t __toupper(__darwin_ct_rune_t);
__darwin_ct_rune_t __tolower(__darwin_ct_rune_t);

extern __inline __attribute__((__gnu_inline__)) int
__wcwidth(__darwin_ct_rune_t _c)
{
 unsigned int _x;
 if (_c == 0)
  return (0);
 _x = (unsigned int)__maskrune(_c, 0xe0000000L|0x00040000L);
 if ((_x & 0xe0000000L) != 0)
  return ((_x & 0xe0000000L) >> 30);
 return ((_x & 0x00040000L) != 0 ? 1 : -1);
}
extern __inline __attribute__((__gnu_inline__)) int
isalnum(int _c)
{
 return (__istype(_c, 0x00000100L|0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
isalpha(int _c)
{
 return (__istype(_c, 0x00000100L));
}
extern __inline __attribute__((__gnu_inline__)) int
isblank(int _c)
{
 return (__istype(_c, 0x00020000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iscntrl(int _c)
{
 return (__istype(_c, 0x00000200L));
}
extern __inline __attribute__((__gnu_inline__)) int
isdigit(int _c)
{
 return (__isctype(_c, 0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
isgraph(int _c)
{
 return (__istype(_c, 0x00000800L));
}
extern __inline __attribute__((__gnu_inline__)) int
islower(int _c)
{
 return (__istype(_c, 0x00001000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isprint(int _c)
{
 return (__istype(_c, 0x00040000L));
}
extern __inline __attribute__((__gnu_inline__)) int
ispunct(int _c)
{
 return (__istype(_c, 0x00002000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isspace(int _c)
{
 return (__istype(_c, 0x00004000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isupper(int _c)
{
 return (__istype(_c, 0x00008000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isxdigit(int _c)
{
 return (__isctype(_c, 0x00010000L));
}
extern __inline __attribute__((__gnu_inline__)) int
toascii(int _c)
{
 return (_c & 0x7F);
}
extern __inline __attribute__((__gnu_inline__)) int
tolower(int _c)
{
        return (__tolower(_c));
}
extern __inline __attribute__((__gnu_inline__)) int
toupper(int _c)
{
        return (__toupper(_c));
}
extern __inline __attribute__((__gnu_inline__)) int
digittoint(int _c)
{
 return (__maskrune(_c, 0x0F));
}
extern __inline __attribute__((__gnu_inline__)) int
ishexnumber(int _c)
{
 return (__istype(_c, 0x00010000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isideogram(int _c)
{
 return (__istype(_c, 0x00080000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isnumber(int _c)
{
 return (__istype(_c, 0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
isphonogram(int _c)
{
 return (__istype(_c, 0x00200000L));
}
extern __inline __attribute__((__gnu_inline__)) int
isrune(int _c)
{
 return (__istype(_c, 0xFFFFFFF0L));
}
extern __inline __attribute__((__gnu_inline__)) int
isspecial(int _c)
{
 return (__istype(_c, 0x00100000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswalnum(wint_t _wc)
{
 return (__istype(_wc, 0x00000100L|0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswalpha(wint_t _wc)
{
 return (__istype(_wc, 0x00000100L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswcntrl(wint_t _wc)
{
 return (__istype(_wc, 0x00000200L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswctype(wint_t _wc, wctype_t _charclass)
{
 return (__istype(_wc, _charclass));
}
extern __inline __attribute__((__gnu_inline__)) int
iswdigit(wint_t _wc)
{
 return (__isctype(_wc, 0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswgraph(wint_t _wc)
{
 return (__istype(_wc, 0x00000800L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswlower(wint_t _wc)
{
 return (__istype(_wc, 0x00001000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswprint(wint_t _wc)
{
 return (__istype(_wc, 0x00040000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswpunct(wint_t _wc)
{
 return (__istype(_wc, 0x00002000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswspace(wint_t _wc)
{
 return (__istype(_wc, 0x00004000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswupper(wint_t _wc)
{
 return (__istype(_wc, 0x00008000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswxdigit(wint_t _wc)
{
 return (__isctype(_wc, 0x00010000L));
}
extern __inline __attribute__((__gnu_inline__)) wint_t
towlower(wint_t _wc)
{
        return (__tolower(_wc));
}
extern __inline __attribute__((__gnu_inline__)) wint_t
towupper(wint_t _wc)
{
        return (__toupper(_wc));
}

wctype_t
 wctype(const char *);

extern __inline __attribute__((__gnu_inline__)) int
iswblank(wint_t _wc)
{
 return (__istype(_wc, 0x00020000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswascii(wint_t _wc)
{
 return ((_wc & ~0x7F) == 0);
}
extern __inline __attribute__((__gnu_inline__)) int
iswhexnumber(wint_t _wc)
{
 return (__istype(_wc, 0x00010000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswideogram(wint_t _wc)
{
 return (__istype(_wc, 0x00080000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswnumber(wint_t _wc)
{
 return (__istype(_wc, 0x00000400L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswphonogram(wint_t _wc)
{
 return (__istype(_wc, 0x00200000L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswrune(wint_t _wc)
{
 return (__istype(_wc, 0xFFFFFFF0L));
}
extern __inline __attribute__((__gnu_inline__)) int
iswspecial(wint_t _wc)
{
 return (__istype(_wc, 0x00100000L));
}

wint_t nextwctype(wint_t, wctype_t);
wint_t towctrans(wint_t, wctrans_t);
wctrans_t
 wctrans(const char *);

