#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#define ZC_SIMD(T, N) T __attribute__((vector_size(N * sizeof(T))))
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO auto
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO __auto_type
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#define ZC_CAST(T, x) ((T)(x))
#ifdef __TINYC__
#undef ZC_AUTO
#define ZC_AUTO __auto_type
#undef ZC_AUTO_INIT
#define ZC_AUTO_INIT(var, init) __typeof__((init)) var = (init)

#ifndef __builtin_expect
#define __builtin_expect(x, v) (x)
#endif

#ifndef __builtin_unreachable
#define __builtin_unreachable()
#endif
#else
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#endif
static inline const char* _z_bool_str(_Bool b) { return b ? "true" : "false"; }
#ifdef __OBJC__
#define _z_objc_map ,id: "%s", Class: "%s", SEL: "%s"
#define _z_objc_arg_map(x) ,id: [(id)(x) description].UTF8String, Class: class_getName((Class)(x)), SEL: sel_getName((SEL)(x))
#else
#define _z_objc_map
#define _z_objc_arg_map(x)
#endif

#define _z_str(x) _Generic((x), _Bool: "%s", char: "%c", signed char: "%c", unsigned char: "%u", short: "%d", unsigned short: "%u", int: "%d", unsigned int: "%u", long: "%ld", unsigned long: "%lu", long long: "%lld", unsigned long long: "%llu", float: "%f", double: "%f", char*: "%s", const char*: "%s", void*: "%p" _z_objc_map)
#define _z_arg(x) _Generic((x), _Bool: _z_bool_str(x) _z_objc_arg_map(x), default: (x))
typedef size_t usize;
typedef char* string;
typedef struct { void *func; void *ctx; } z_closure_T;
typedef void U0;
typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;
#define F32 float
#define F64 double
#define z_malloc malloc
#define z_realloc realloc
#define z_free free
#define z_print printf
void z_panic(const char* msg) { fprintf(stderr, "Panic: %s\n", msg); exit(1); }
#if defined(__APPLE__)
#define _ZC_SEC __attribute__((used,section("__DATA,__zarch")))
#elif defined(_WIN32)
#define _ZC_SEC __attribute__((used))
#else
#define _ZC_SEC __attribute__((used,section(".note.zarch")))
#endif
static const unsigned char _zc_abi_v1[] _ZC_SEC = {0x07,0xd5,0x59,0x30,0x7c,0x7f,0x66,0x75,0x30,0x69,0x7f,0x65,0x3c,0x30,0x59,0x7c,0x79,0x7e,0x73,0x71};
void _z_autofree_impl(void *p) { void **pp = (void**)p; if(*pp) { z_free(*pp); *pp = NULL; } }
#define assert(cond, ...) if (!(cond)) { fprintf(stderr, "Assertion failed: " __VA_ARGS__); exit(1); }
string _z_readln_raw() { size_t cap = 64; size_t len = 0; char *line = z_malloc(cap); if(!line) return NULL; int c; while((c = fgetc(stdin)) != EOF) { if(c == '\n') break; if(len + 1 >= cap) { cap *= 2; char *n = z_realloc(line, cap); if(!n) { z_free(line); return NULL; } line = n; } line[len++] = c; } if(len == 0 && c == EOF) { z_free(line); return NULL; } line[len] = 0; return line; }
int _z_scan_helper(const char *fmt, ...) { char *l = _z_readln_raw(); if(!l) return 0; va_list ap; va_start(ap, fmt); int r = vsscanf(l, fmt, ap); va_end(ap); z_free(l); return r; }
int _z_orig_stdout = -1;
void _z_suppress_stdout() {
    fflush(stdout);
    if (_z_orig_stdout == -1) _z_orig_stdout = dup(STDOUT_FILENO);
    int nullfd = open("/dev/null", O_WRONLY);
    dup2(nullfd, STDOUT_FILENO);
    close(nullfd);
}
void _z_restore_stdout() {
    fflush(stdout);
    if (_z_orig_stdout != -1) {
        dup2(_z_orig_stdout, STDOUT_FILENO);
        close(_z_orig_stdout);
        _z_orig_stdout = -1;
    }
}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
typedef char* string;
typedef struct { void **data; int len; int cap; } Vec;
#define Vec_new() (Vec){.data=0, .len=0, .cap=0}
void _z_vec_push(Vec *v, void *item) { if(v->len >= v->cap) { v->cap = v->cap?v->cap*2:8; v->data = z_realloc(v->data, v->cap * sizeof(void*)); } v->data[v->len++] = item; }
static inline Vec _z_make_vec(int count, ...) { Vec v = {0}; v.cap = count > 8 ? count : 8; v.data = z_malloc(v.cap * sizeof(void*)); v.len = 0; va_list args; va_start(args, count); for(int i=0; i<count; i++) { v.data[v.len++] = va_arg(args, void*); } va_end(args); return v; }
#define Vec_push(v, i) _z_vec_push(&(v), (void*)(long)(i))
static inline long _z_check_bounds(long index, long limit) { if(index < 0 || index >= limit) { fprintf(stderr, "Index out of bounds: %ld (limit %ld)\n", index, limit); exit(1); } return index; }

typedef struct VecIterResult_DirEntryPtr VecIterResult_DirEntryPtr;
typedef struct VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr;
typedef struct Option_DirEntryPtr Option_DirEntryPtr;
typedef struct VecIter_DirEntryPtr VecIter_DirEntryPtr;
typedef struct Vec_DirEntryPtr Vec_DirEntryPtr;
typedef struct VecIterResult_DirEntry VecIterResult_DirEntry;
typedef struct VecIterRef_DirEntry VecIterRef_DirEntry;
typedef struct VecIter_DirEntry VecIter_DirEntry;
typedef struct Vec_DirEntry Vec_DirEntry;
typedef struct Result_bool Result_bool;
typedef struct VecIterResult_String VecIterResult_String;
typedef struct VecIterRef_String VecIterRef_String;
typedef struct VecIter_String VecIter_String;
typedef struct Vec_String Vec_String;
typedef struct Option_size_t Option_size_t;
typedef struct VecIterResult_char VecIterResult_char;
typedef struct VecIterRef_char VecIterRef_char;
typedef struct Option_char Option_char;
typedef struct VecIter_char VecIter_char;
typedef struct Vec_char Vec_char;
typedef struct Metadata Metadata;
typedef struct File File;
typedef struct String String;
typedef struct FileType FileType;
typedef struct Result_FileType Result_FileType;
typedef struct Result_Vec_DirEntry Result_Vec_DirEntry;
typedef struct Result_Metadata Result_Metadata;
typedef struct Result_String Result_String;
typedef struct Result_File Result_File;
typedef struct Option_String Option_String;
typedef struct DirEntry DirEntry;
typedef struct Option_DirEntry Option_DirEntry;
FileType FileType_CPP();
FileType FileType_HPP();
typedef struct Clone_VTable {
    void* (*clone)(void* self);
} Clone_VTable;
typedef struct Clone { void *self; Clone_VTable *vtable; } Clone;
Clone Clone__clone(Clone* self) {
    void* ret = self->vtable->clone(self->self);
    return (Clone){.self = ret, .vtable = self->vtable};
}


typedef struct Copy_VTable {
} Copy_VTable;
typedef struct Copy { void *self; Copy_VTable *vtable; } Copy;

typedef struct Drop_VTable {
    void (*drop)(void* self);
} Drop_VTable;
typedef struct Drop { void *self; Drop_VTable *vtable; } Drop;
void Drop__drop(Drop* self) {
    return self->vtable->drop(self->self);
}


#define panic(msg) _zen_panic(__FILE__, __LINE__, __func__, msg)


    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    typedef struct DirEntry* DirEntryPtr;
    
    // FILE* wrappers - fopen/fclose/etc use FILE* which conflicts with void*
    void* _z_fs_fopen(const char* path, const char* mode) {
        return fopen(path, mode);
    }

    int _z_fs_fclose(void* stream) {
        return fclose((FILE*)stream);
    }

    size_t _z_fs_fread(void* ptr, size_t size, size_t nmemb, void* stream) {
        return fread(ptr, size, nmemb, (FILE*)stream);
    }

    size_t _z_fs_fwrite(void* ptr, size_t size, size_t nmemb, void* stream) {
        return fwrite(ptr, size, nmemb, (FILE*)stream);
    }

    int _z_fs_fseek(void* stream, int64_t offset, int whence) {
        return fseek((FILE*)stream, (long)offset, whence);
    }

    int64_t _z_fs_ftell(void* stream) {
        return (int64_t)ftell((FILE*)stream);
    }
    
    // DIR* wrappers - opendir/closedir/readdir use DIR* which conflicts with void*
    void* _z_fs_opendir(const char* name) {
        return opendir(name);
    }

    int _z_fs_closedir(void* dir) {
        return closedir((DIR*)dir);
    }

    // struct stat access - cannot define matching Zen-C struct for stat
    int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file) {
        struct stat st;
        if (stat(path, &st) != 0) return -1;
        *size = st.st_size;
        *is_dir = S_ISDIR(st.st_mode);
        *is_file = S_ISREG(st.st_mode);
        return 0;
    }
    
    // struct dirent access - readdir returns struct dirent*
    int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir) {
        struct dirent* ent = readdir((DIR*)dir);
        if (!ent) return 0;
        strncpy(out_name, ent->d_name, buf_size - 1);
        out_name[buf_size - 1] = 0;
#if defined(_DIRENT_HAVE_D_TYPE) || defined(DT_DIR)
        *is_dir = (ent->d_type == DT_DIR);
#else
        *is_dir = 0; // Fallback for Windows/MinGW if d_type missing
#endif
        return 1;
    }

    // mkdir has different signatures on Windows vs POSIX
    int _z_fs_mkdir(const char* path) {
        #ifdef _WIN32
            return mkdir(path);
        #else
            return mkdir(path, 0777);
        #endif
    }

    // Wrapper for getcwd to avoid signature conflicts on Windows
    char* _z_fs_getcwd(char* buf, size_t size) {
        return getcwd(buf, size);
    }

struct VecIterResult_DirEntryPtr {
    struct DirEntry** ptr;
};

struct VecIterRef_DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Option_DirEntryPtr {
    bool is_some;
    struct DirEntry* val;
};

struct VecIter_DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Vec_DirEntryPtr {
    DirEntry** data;
    size_t len;
    size_t cap;
};

struct VecIterResult_DirEntry {
    DirEntry* ptr;
};

struct VecIterRef_DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct VecIter_DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct Vec_DirEntry {
    DirEntry* data;
    size_t len;
    size_t cap;
};

struct Result_bool {
    bool is_ok;
    bool val;
    char* err;
};

struct VecIterResult_String {
    String* ptr;
};

struct VecIterRef_String {
    String* data;
    size_t count;
    size_t idx;
};

struct VecIter_String {
    String* data;
    size_t count;
    size_t idx;
};

struct Vec_String {
    String* data;
    size_t len;
    size_t cap;
};

struct Option_size_t {
    bool is_some;
    size_t val;
};

struct VecIterResult_char {
    char* ptr;
};

struct VecIterRef_char {
    char* data;
    size_t count;
    size_t idx;
};

struct Option_char {
    bool is_some;
    char val;
};

struct VecIter_char {
    char* data;
    size_t count;
    size_t idx;
};

struct Vec_char {
    char* data;
    size_t len;
    size_t cap;
};

struct Metadata {
    uint64_t size;
    bool is_dir;
    bool is_file;
};

struct File {
    void* handle;
};

struct String {
    Vec_char vec;
};

typedef enum { FileType_CPP_Tag, FileType_HPP_Tag, } FileType_Tag;
struct FileType { FileType_Tag tag; union { } data; };

FileType FileType_CPP() { return (FileType){.tag=FileType_CPP_Tag}; }
FileType FileType_HPP() { return (FileType){.tag=FileType_HPP_Tag}; }
struct Result_FileType {
    bool is_ok;
    FileType val;
    char* err;
};

struct Result_Vec_DirEntry {
    bool is_ok;
    Vec_DirEntry val;
    char* err;
};

struct Result_Metadata {
    bool is_ok;
    Metadata val;
    char* err;
};

struct Result_String {
    bool is_ok;
    String val;
    char* err;
};

struct Result_File {
    bool is_ok;
    File val;
    char* err;
};

struct Option_String {
    bool is_some;
    String val;
};

struct DirEntry {
    String name;
    bool is_dir;
};

struct Option_DirEntry {
    bool is_some;
    DirEntry val;
};

size_t __zen_hash_seed = 14695981039346656037ULL;
const int Z_SEEK_SET = 0;
const int Z_SEEK_END = 2;
const int Z_F_OK = 0;
Result_FileType Result_FileType__Ok(FileType v);
Result_FileType Result_FileType__Err(char* e);
bool Result_FileType__is_ok(Result_FileType* self);
bool Result_FileType__is_err(Result_FileType* self);
void Result_FileType__forget(Result_FileType* self);
FileType Result_FileType__unwrap(Result_FileType* self);
FileType* Result_FileType__unwrap_ref(Result_FileType* self);
FileType Result_FileType__expect(Result_FileType* self, char* msg);
Result_Vec_DirEntry Result_Vec_DirEntry__Ok(Vec_DirEntry v);
Result_Vec_DirEntry Result_Vec_DirEntry__Err(char* e);
bool Result_Vec_DirEntry__is_ok(Result_Vec_DirEntry* self);
bool Result_Vec_DirEntry__is_err(Result_Vec_DirEntry* self);
void Result_Vec_DirEntry__forget(Result_Vec_DirEntry* self);
Vec_DirEntry Result_Vec_DirEntry__unwrap(Result_Vec_DirEntry* self);
Vec_DirEntry* Result_Vec_DirEntry__unwrap_ref(Result_Vec_DirEntry* self);
Vec_DirEntry Result_Vec_DirEntry__expect(Result_Vec_DirEntry* self, char* msg);
Vec_DirEntryPtr Vec_DirEntryPtr__new(void);
Vec_DirEntryPtr Vec_DirEntryPtr__with_capacity(size_t cap);
void Vec_DirEntryPtr__grow(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__grow_to_fit(Vec_DirEntryPtr* self, size_t new_len);
VecIter_DirEntryPtr Vec_DirEntryPtr__iterator(Vec_DirEntryPtr* self);
VecIterRef_DirEntryPtr Vec_DirEntryPtr__iter_ref(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__push(Vec_DirEntryPtr* self, DirEntry* item);
void Vec_DirEntryPtr__insert(Vec_DirEntryPtr* self, size_t idx, DirEntry* item);
DirEntry* Vec_DirEntryPtr__pop(Vec_DirEntryPtr* self);
Option_DirEntryPtr Vec_DirEntryPtr__pop_opt(Vec_DirEntryPtr* self);
DirEntry* Vec_DirEntryPtr__remove(Vec_DirEntryPtr* self, size_t idx);
void Vec_DirEntryPtr__append(Vec_DirEntryPtr* self, Vec_DirEntryPtr other);
DirEntry* Vec_DirEntryPtr__get(Vec_DirEntryPtr* self, size_t idx);
DirEntry* Vec_DirEntryPtr__index(Vec_DirEntryPtr* self, size_t idx);
DirEntry** Vec_DirEntryPtr__get_ref(Vec_DirEntryPtr* self, size_t idx);
DirEntry* Vec_DirEntryPtr__last(Vec_DirEntryPtr* self);
size_t Vec_DirEntryPtr__length(Vec_DirEntryPtr* self);
bool Vec_DirEntryPtr__contains(Vec_DirEntryPtr* self, DirEntry* item);
bool Vec_DirEntryPtr__is_empty(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__clear(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__free(Vec_DirEntryPtr* self);
DirEntry* Vec_DirEntryPtr__first(Vec_DirEntryPtr* self);
void Vec_DirEntryPtr__set(Vec_DirEntryPtr* self, size_t idx, DirEntry* item);
void Vec_DirEntryPtr__reverse(Vec_DirEntryPtr* self);
bool Vec_DirEntryPtr__eq(Vec_DirEntryPtr* self, Vec_DirEntryPtr other);
void Vec_DirEntryPtr__forget(Vec_DirEntryPtr* self);
Vec_DirEntryPtr Vec_DirEntryPtr__clone(Vec_DirEntryPtr* self);
VecIterResult_DirEntryPtr VecIterRef_DirEntryPtr__next(VecIterRef_DirEntryPtr* self);
VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr__iterator(VecIterRef_DirEntryPtr* self);
bool VecIterResult_DirEntryPtr__is_none(VecIterResult_DirEntryPtr* self);
struct DirEntry** VecIterResult_DirEntryPtr__unwrap(VecIterResult_DirEntryPtr* self);
Option_DirEntryPtr VecIter_DirEntryPtr__next(VecIter_DirEntryPtr* self);
VecIter_DirEntryPtr VecIter_DirEntryPtr__iterator(VecIter_DirEntryPtr* self);
Option_DirEntryPtr Option_DirEntryPtr__Some(struct DirEntry* v);
Option_DirEntryPtr Option_DirEntryPtr__None(void);
bool Option_DirEntryPtr__is_some(Option_DirEntryPtr* self);
bool Option_DirEntryPtr__is_none(Option_DirEntryPtr* self);
void Option_DirEntryPtr__forget(Option_DirEntryPtr* self);
struct DirEntry* Option_DirEntryPtr__unwrap(Option_DirEntryPtr* self);
struct DirEntry** Option_DirEntryPtr__unwrap_ref(Option_DirEntryPtr* self);
struct DirEntry* Option_DirEntryPtr__unwrap_or(Option_DirEntryPtr* self, struct DirEntry* def_val);
struct DirEntry* Option_DirEntryPtr__expect(Option_DirEntryPtr* self, char* msg);
Option_DirEntryPtr Option_DirEntryPtr__or_else(Option_DirEntryPtr* self, Option_DirEntryPtr other);
void Vec_DirEntryPtr__Drop_drop(Vec_DirEntryPtr* self);
Vec_DirEntry Vec_DirEntry__new(void);
Vec_DirEntry Vec_DirEntry__with_capacity(size_t cap);
void Vec_DirEntry__grow(Vec_DirEntry* self);
void Vec_DirEntry__grow_to_fit(Vec_DirEntry* self, size_t new_len);
VecIter_DirEntry Vec_DirEntry__iterator(Vec_DirEntry* self);
VecIterRef_DirEntry Vec_DirEntry__iter_ref(Vec_DirEntry* self);
void Vec_DirEntry__push(Vec_DirEntry* self, DirEntry item);
void Vec_DirEntry__insert(Vec_DirEntry* self, size_t idx, DirEntry item);
DirEntry Vec_DirEntry__pop(Vec_DirEntry* self);
Option_DirEntry Vec_DirEntry__pop_opt(Vec_DirEntry* self);
DirEntry Vec_DirEntry__remove(Vec_DirEntry* self, size_t idx);
void Vec_DirEntry__append(Vec_DirEntry* self, Vec_DirEntry other);
DirEntry Vec_DirEntry__get(Vec_DirEntry* self, size_t idx);
DirEntry Vec_DirEntry__index(Vec_DirEntry* self, size_t idx);
DirEntry* Vec_DirEntry__get_ref(Vec_DirEntry* self, size_t idx);
DirEntry Vec_DirEntry__last(Vec_DirEntry* self);
size_t Vec_DirEntry__length(Vec_DirEntry* self);
bool Vec_DirEntry__contains(Vec_DirEntry* self, DirEntry item);
bool Vec_DirEntry__is_empty(Vec_DirEntry* self);
void Vec_DirEntry__clear(Vec_DirEntry* self);
void Vec_DirEntry__free(Vec_DirEntry* self);
DirEntry Vec_DirEntry__first(Vec_DirEntry* self);
void Vec_DirEntry__set(Vec_DirEntry* self, size_t idx, DirEntry item);
void Vec_DirEntry__reverse(Vec_DirEntry* self);
bool Vec_DirEntry__eq(Vec_DirEntry* self, Vec_DirEntry other);
void Vec_DirEntry__forget(Vec_DirEntry* self);
Vec_DirEntry Vec_DirEntry__clone(Vec_DirEntry* self);
VecIterResult_DirEntry VecIterRef_DirEntry__next(VecIterRef_DirEntry* self);
VecIterRef_DirEntry VecIterRef_DirEntry__iterator(VecIterRef_DirEntry* self);
bool VecIterResult_DirEntry__is_none(VecIterResult_DirEntry* self);
DirEntry* VecIterResult_DirEntry__unwrap(VecIterResult_DirEntry* self);
Option_DirEntry VecIter_DirEntry__next(VecIter_DirEntry* self);
VecIter_DirEntry VecIter_DirEntry__iterator(VecIter_DirEntry* self);
Option_DirEntry Option_DirEntry__Some(DirEntry v);
Option_DirEntry Option_DirEntry__None(void);
bool Option_DirEntry__is_some(Option_DirEntry* self);
bool Option_DirEntry__is_none(Option_DirEntry* self);
void Option_DirEntry__forget(Option_DirEntry* self);
DirEntry Option_DirEntry__unwrap(Option_DirEntry* self);
DirEntry* Option_DirEntry__unwrap_ref(Option_DirEntry* self);
DirEntry Option_DirEntry__unwrap_or(Option_DirEntry* self, DirEntry def_val);
DirEntry Option_DirEntry__expect(Option_DirEntry* self, char* msg);
Option_DirEntry Option_DirEntry__or_else(Option_DirEntry* self, Option_DirEntry other);
void Vec_DirEntry__Drop_drop(Vec_DirEntry* self);
Result_Metadata Result_Metadata__Ok(Metadata v);
Result_Metadata Result_Metadata__Err(char* e);
bool Result_Metadata__is_ok(Result_Metadata* self);
bool Result_Metadata__is_err(Result_Metadata* self);
void Result_Metadata__forget(Result_Metadata* self);
Metadata Result_Metadata__unwrap(Result_Metadata* self);
Metadata* Result_Metadata__unwrap_ref(Result_Metadata* self);
Metadata Result_Metadata__expect(Result_Metadata* self, char* msg);
Result_bool Result_bool__Ok(bool v);
Result_bool Result_bool__Err(char* e);
bool Result_bool__is_ok(Result_bool* self);
bool Result_bool__is_err(Result_bool* self);
void Result_bool__forget(Result_bool* self);
bool Result_bool__unwrap(Result_bool* self);
bool* Result_bool__unwrap_ref(Result_bool* self);
bool Result_bool__expect(Result_bool* self, char* msg);
Result_String Result_String__Ok(String v);
Result_String Result_String__Err(char* e);
bool Result_String__is_ok(Result_String* self);
bool Result_String__is_err(Result_String* self);
void Result_String__forget(Result_String* self);
String Result_String__unwrap(Result_String* self);
String* Result_String__unwrap_ref(Result_String* self);
String Result_String__expect(Result_String* self, char* msg);
Result_File Result_File__Ok(File v);
Result_File Result_File__Err(char* e);
bool Result_File__is_ok(Result_File* self);
bool Result_File__is_err(Result_File* self);
void Result_File__forget(Result_File* self);
File Result_File__unwrap(Result_File* self);
File* Result_File__unwrap_ref(Result_File* self);
File Result_File__expect(Result_File* self, char* msg);
Vec_String Vec_String__new(void);
Vec_String Vec_String__with_capacity(size_t cap);
void Vec_String__grow(Vec_String* self);
void Vec_String__grow_to_fit(Vec_String* self, size_t new_len);
VecIter_String Vec_String__iterator(Vec_String* self);
VecIterRef_String Vec_String__iter_ref(Vec_String* self);
void Vec_String__push(Vec_String* self, String item);
void Vec_String__insert(Vec_String* self, size_t idx, String item);
String Vec_String__pop(Vec_String* self);
Option_String Vec_String__pop_opt(Vec_String* self);
String Vec_String__remove(Vec_String* self, size_t idx);
void Vec_String__append(Vec_String* self, Vec_String other);
String Vec_String__get(Vec_String* self, size_t idx);
String Vec_String__index(Vec_String* self, size_t idx);
String* Vec_String__get_ref(Vec_String* self, size_t idx);
String Vec_String__last(Vec_String* self);
size_t Vec_String__length(Vec_String* self);
bool Vec_String__contains(Vec_String* self, String item);
bool Vec_String__is_empty(Vec_String* self);
void Vec_String__clear(Vec_String* self);
void Vec_String__free(Vec_String* self);
String Vec_String__first(Vec_String* self);
void Vec_String__set(Vec_String* self, size_t idx, String item);
void Vec_String__reverse(Vec_String* self);
bool Vec_String__eq(Vec_String* self, Vec_String other);
void Vec_String__forget(Vec_String* self);
Vec_String Vec_String__clone(Vec_String* self);
VecIterResult_String VecIterRef_String__next(VecIterRef_String* self);
VecIterRef_String VecIterRef_String__iterator(VecIterRef_String* self);
bool VecIterResult_String__is_none(VecIterResult_String* self);
String* VecIterResult_String__unwrap(VecIterResult_String* self);
Option_String VecIter_String__next(VecIter_String* self);
VecIter_String VecIter_String__iterator(VecIter_String* self);
Option_String Option_String__Some(String v);
Option_String Option_String__None(void);
bool Option_String__is_some(Option_String* self);
bool Option_String__is_none(Option_String* self);
void Option_String__forget(Option_String* self);
String Option_String__unwrap(Option_String* self);
String* Option_String__unwrap_ref(Option_String* self);
String Option_String__unwrap_or(Option_String* self, String def_val);
String Option_String__expect(Option_String* self, char* msg);
Option_String Option_String__or_else(Option_String* self, Option_String other);
void Vec_String__Drop_drop(Vec_String* self);
Option_size_t Option_size_t__Some(size_t v);
Option_size_t Option_size_t__None(void);
bool Option_size_t__is_some(Option_size_t* self);
bool Option_size_t__is_none(Option_size_t* self);
void Option_size_t__forget(Option_size_t* self);
size_t Option_size_t__unwrap(Option_size_t* self);
size_t* Option_size_t__unwrap_ref(Option_size_t* self);
size_t Option_size_t__unwrap_or(Option_size_t* self, size_t def_val);
size_t Option_size_t__expect(Option_size_t* self, char* msg);
Option_size_t Option_size_t__or_else(Option_size_t* self, Option_size_t other);
Vec_char Vec_char__new(void);
Vec_char Vec_char__with_capacity(size_t cap);
void Vec_char__grow(Vec_char* self);
void Vec_char__grow_to_fit(Vec_char* self, size_t new_len);
VecIter_char Vec_char__iterator(Vec_char* self);
VecIterRef_char Vec_char__iter_ref(Vec_char* self);
void Vec_char__push(Vec_char* self, char item);
void Vec_char__insert(Vec_char* self, size_t idx, char item);
char Vec_char__pop(Vec_char* self);
Option_char Vec_char__pop_opt(Vec_char* self);
char Vec_char__remove(Vec_char* self, size_t idx);
void Vec_char__append(Vec_char* self, Vec_char other);
char Vec_char__get(Vec_char* self, size_t idx);
char Vec_char__index(Vec_char* self, size_t idx);
char* Vec_char__get_ref(Vec_char* self, size_t idx);
char Vec_char__last(Vec_char* self);
size_t Vec_char__length(Vec_char* self);
bool Vec_char__contains(Vec_char* self, char item);
bool Vec_char__is_empty(Vec_char* self);
void Vec_char__clear(Vec_char* self);
void Vec_char__free(Vec_char* self);
char Vec_char__first(Vec_char* self);
void Vec_char__set(Vec_char* self, size_t idx, char item);
void Vec_char__reverse(Vec_char* self);
bool Vec_char__eq(Vec_char* self, Vec_char other);
void Vec_char__forget(Vec_char* self);
Vec_char Vec_char__clone(Vec_char* self);
VecIterResult_char VecIterRef_char__next(VecIterRef_char* self);
VecIterRef_char VecIterRef_char__iterator(VecIterRef_char* self);
bool VecIterResult_char__is_none(VecIterResult_char* self);
char* VecIterResult_char__unwrap(VecIterResult_char* self);
Option_char VecIter_char__next(VecIter_char* self);
VecIter_char VecIter_char__iterator(VecIter_char* self);
Option_char Option_char__Some(char v);
Option_char Option_char__None(void);
bool Option_char__is_some(Option_char* self);
bool Option_char__is_none(Option_char* self);
void Option_char__forget(Option_char* self);
char Option_char__unwrap(Option_char* self);
char* Option_char__unwrap_ref(Option_char* self);
char Option_char__unwrap_or(Option_char* self, char def_val);
char Option_char__expect(Option_char* self, char* msg);
Option_char Option_char__or_else(Option_char* self, Option_char other);
void Vec_char__Drop_drop(Vec_char* self);
int main(int32_t ac, string* av);
void gen_file(string file_name, string format);
Vec_char get_upper(string file_name);
Vec_char get_lower(string file_name);
Result_FileType check_ext(string file_name);
string hpp_format(void);
string cpp_format(void);
int _z_fs_closedir(void* dir);
void* _z_fs_opendir(const char* name);
char* _z_fs_getcwd(char* buf, size_t size);
int64_t _z_fs_ftell(void* stream);
int _z_fs_fseek(void* stream, int64_t offset, int whence);
size_t _z_fs_fwrite(void* ptr, size_t size, size_t nmemb, void* stream);
size_t _z_fs_fread(void* ptr, size_t size, size_t nmemb, void* stream);
int _z_fs_fclose(void* stream);
void* _z_fs_fopen(const char* path, const char* mode);
int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir);
int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file);
int _z_fs_mkdir(const char* path);
void free(void* ptr);
void* malloc(size_t size);
int rmdir(const char* pathname);
int unlink(const char* pathname);
int access(const char* pathname, int mode);
void _zen_panic(const char* file, int32_t line, const char* func, const char* msg);
void exit(int32_t code);
Result_File File__open(char* path, char* mode);
void File__close(File* self);
Result_String File__read_to_string(File* self);
Result_String File__read_all(char* path);
Result_bool File__write_string(File* self, char* content);
bool File__exists(char* path);
Result_Metadata File__metadata(char* path);
Result_bool File__create_dir(char* path);
Result_bool File__remove_file(char* path);
Result_bool File__remove_dir(char* path);
Result_Vec_DirEntry File__read_dir(char* path);
Result_String File__current_dir(void);
String String__new(char* s);
String String__from(char* s);
char* String__c_str(String* self);
void String__destroy(String* self);
void String__forget(String* self);
void String__append(String* self, String* other);
void String__append_c(String* self, char* s);
void String__append_c_ptr(String* ptr, char* s);
String String__add(String* self, String* other);
bool String__eq(String* self, String* other);
bool String__eq_str(String* self, char* s);
size_t String__length(String* self);
String String__substring(String* self, size_t start, size_t len);
Option_size_t String__find(String* self, char target);
void String__print(String* self);
void String__println(String* self);
bool String__is_empty(String* self);
bool String__contains(String* self, char target);
bool String__starts_with(String* self, char* prefix);
bool String__ends_with(String* self, char* suffix);
void String__reserve(String* self, size_t cap);
void String__free(String* self);
size_t String___utf8_seq_len(char first_byte);
size_t String__utf8_len(String* self);
String String__utf8_at(String* self, size_t idx);
String String__utf8_substr(String* self, size_t start_idx, size_t num_chars);
Vec_String String__split(String* self, char delim);
String String__trim(String* self);
String String__replace(String* self, char* target, char* replacement);
// Auto-Generated RAII Glue for Vec_DirEntryPtr
void Vec_DirEntryPtr__Drop_glue(Vec_DirEntryPtr *self) {
    Vec_DirEntryPtr__Drop_drop(self);
}

// Auto-Generated RAII Glue for Vec_DirEntry
void Vec_DirEntry__Drop_glue(Vec_DirEntry *self) {
    Vec_DirEntry__Drop_drop(self);
}

// Auto-Generated RAII Glue for Vec_String
void Vec_String__Drop_glue(Vec_String *self) {
    Vec_String__Drop_drop(self);
}

// Auto-Generated RAII Glue for Vec_char
void Vec_char__Drop_glue(Vec_char *self) {
    Vec_char__Drop_drop(self);
}

// Auto-Generated RAII Glue for String
void String__Drop_glue(String *self) {
    Vec_char__Drop_glue(&self->vec);
}

// Auto-Generated RAII Glue for DirEntry
void DirEntry__Drop_glue(DirEntry *self) {
    String__Drop_glue(&self->name);
}


Result_FileType Result_FileType__Ok(FileType v)
{
    {
    return (Result_FileType){.is_ok = true, .val = v};
    }
}

Result_FileType Result_FileType__Err(char* e)
{
    {
    return (Result_FileType){.is_ok = false, .err = e};
    }
}

bool Result_FileType__is_ok(Result_FileType* self)
{
    {
    return self->is_ok;
    }
}

bool Result_FileType__is_err(Result_FileType* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_FileType__forget(Result_FileType* self)
{
    {
memset((&self->val), 0, sizeof(FileType));
    }
}

FileType Result_FileType__unwrap(Result_FileType* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    FileType v = self->val;
memset((&self->val), 0, sizeof(FileType));
    return v;
    }
}

FileType* Result_FileType__unwrap_ref(Result_FileType* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

FileType Result_FileType__expect(Result_FileType* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Result_Vec_DirEntry Result_Vec_DirEntry__Ok(Vec_DirEntry v)
{
    int __z_drop_flag_v = 1;
    {
    { Result_Vec_DirEntry _z_ret = (Result_Vec_DirEntry){.is_ok = true, .val = ({ __typeof__(v) _mv = v; __z_drop_flag_v = 0; memset(&v, 0, sizeof(v)); _mv; })};     if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
return _z_ret; }
    }
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
}

Result_Vec_DirEntry Result_Vec_DirEntry__Err(char* e)
{
    {
    return (Result_Vec_DirEntry){.is_ok = false, .err = e};
    }
}

bool Result_Vec_DirEntry__is_ok(Result_Vec_DirEntry* self)
{
    {
    return self->is_ok;
    }
}

bool Result_Vec_DirEntry__is_err(Result_Vec_DirEntry* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_Vec_DirEntry__forget(Result_Vec_DirEntry* self)
{
    {
memset((&self->val), 0, sizeof(Vec_DirEntry));
    }
}

Vec_DirEntry Result_Vec_DirEntry__unwrap(Result_Vec_DirEntry* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_v = 1; Vec_DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));
memset((&self->val), 0, sizeof(Vec_DirEntry));
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
_z_ret_mv; });
    if (__z_drop_flag_v) Vec_DirEntry__Drop_glue(&v);
    }
}

Vec_DirEntry* Result_Vec_DirEntry__unwrap_ref(Result_Vec_DirEntry* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

Vec_DirEntry Result_Vec_DirEntry__expect(Result_Vec_DirEntry* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Vec_DirEntryPtr Vec_DirEntryPtr__new(void)
{
    {
    return (Vec_DirEntryPtr){};
    }
}

Vec_DirEntryPtr Vec_DirEntryPtr__with_capacity(size_t cap)
{
    {
if ((cap == 0))     {
    return (Vec_DirEntryPtr){};
    }
    return (Vec_DirEntryPtr){.data = ((DirEntry**)(malloc((cap * sizeof(DirEntry*))))), .cap = cap};
    }
}

void Vec_DirEntryPtr__grow(Vec_DirEntryPtr* self)
{
    {
if ((self->cap == 0))     {
(self->cap = 8);
    }
 else     {
(self->cap = (self->cap * 2));
    }
(self->data = ((DirEntry**)(realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

void Vec_DirEntryPtr__grow_to_fit(Vec_DirEntryPtr* self, size_t new_len)
{
    {
if ((self->cap >= new_len))     {
    return ;
    }
if ((self->cap == 0))     {
(self->cap = 8);
    }
while ((self->cap < new_len))     {
(self->cap = (self->cap * 2));
    }
(self->data = ((DirEntry**)(realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

VecIter_DirEntryPtr Vec_DirEntryPtr__iterator(Vec_DirEntryPtr* self)
{
    {
    return (VecIter_DirEntryPtr){.data = self->data, .count = self->len};
    }
}

VecIterRef_DirEntryPtr Vec_DirEntryPtr__iter_ref(Vec_DirEntryPtr* self)
{
    {
    return (VecIterRef_DirEntryPtr){.data = self->data, .count = self->len};
    }
}

void Vec_DirEntryPtr__push(Vec_DirEntryPtr* self, DirEntry* item)
{
    {
if ((self->len >= self->cap))     {
Vec_DirEntryPtr__grow(self);
    }
(self->data[self->len] = item);
(self->len = (self->len + 1));
    }
}

void Vec_DirEntryPtr__insert(Vec_DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {
if ((idx > self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
if ((self->len >= self->cap))     {
Vec_DirEntryPtr__grow(self);
    }
if ((idx < self->len))     {
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry*)));
    }
(self->data[idx] = item);
(self->len = (self->len + 1));
    }
}

DirEntry* Vec_DirEntryPtr__pop(Vec_DirEntryPtr* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->len = (self->len - 1));
    return self->data[self->len];
    }
}

Option_DirEntryPtr Vec_DirEntryPtr__pop_opt(Vec_DirEntryPtr* self)
{
    {
if ((self->len == 0))     {
    return Option_DirEntryPtr__None();
    }
(self->len = (self->len - 1));
    return Option_DirEntryPtr__Some(self->data[self->len]);
    }
}

DirEntry* Vec_DirEntryPtr__remove(Vec_DirEntryPtr* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    DirEntry* item = self->data[idx];
if ((idx < (self->len - 1)))     {
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry*)));
    }
(self->len = (self->len - 1));
    return item;
    }
}

void Vec_DirEntryPtr__append(Vec_DirEntryPtr* self, Vec_DirEntryPtr other)
{
    int __z_drop_flag_other = 1;
    {
    size_t new_len = (self->len + other.len);
Vec_DirEntryPtr__grow_to_fit(self, new_len);
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry*)));
(self->len = new_len);
    }
    if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
}

DirEntry* Vec_DirEntryPtr__get(Vec_DirEntryPtr* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[idx];
    }
}

DirEntry* Vec_DirEntryPtr__index(Vec_DirEntryPtr* self, size_t idx)
{
    {
    return Vec_DirEntryPtr__get(self, idx);
    }
}

DirEntry** Vec_DirEntryPtr__get_ref(Vec_DirEntryPtr* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->data[idx]);
    }
}

DirEntry* Vec_DirEntryPtr__last(Vec_DirEntryPtr* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[(self->len - 1)];
    }
}

size_t Vec_DirEntryPtr__length(Vec_DirEntryPtr* self)
{
    {
    return self->len;
    }
}

bool Vec_DirEntryPtr__contains(Vec_DirEntryPtr* self, DirEntry* item)
{
    {
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&item), sizeof(DirEntry*)) == 0))     {
    return true;
    }
(i++);
    }
    return false;
    }
}

bool Vec_DirEntryPtr__is_empty(Vec_DirEntryPtr* self)
{
    {
    return (self->len == 0);
    }
}

void Vec_DirEntryPtr__clear(Vec_DirEntryPtr* self)
{
    {
(self->len = 0);
    }
}

void Vec_DirEntryPtr__free(Vec_DirEntryPtr* self)
{
    {
if (self->data)     {
free(self->data);
    }
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

DirEntry* Vec_DirEntryPtr__first(Vec_DirEntryPtr* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[0];
    }
}

void Vec_DirEntryPtr__set(Vec_DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->data[idx] = item);
    }
}

void Vec_DirEntryPtr__reverse(Vec_DirEntryPtr* self)
{
    {
    size_t i = 0;
    size_t j = (self->len - 1);
while ((i < j))     {
    DirEntry* tmp = self->data[i];
(self->data[i] = self->data[j]);
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

bool Vec_DirEntryPtr__eq(Vec_DirEntryPtr* self, Vec_DirEntryPtr other)
{
    int __z_drop_flag_other = 1;
    {
if ((self->len != other.len))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
return _z_ret; }
    }
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&other.data[i]), sizeof(DirEntry*)) != 0))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
return _z_ret; }
    }
(i = (i + 1));
    }
    { bool _z_ret = true;     if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
return _z_ret; }
    }
    if (__z_drop_flag_other) Vec_DirEntryPtr__Drop_glue(&other);
}

void Vec_DirEntryPtr__forget(Vec_DirEntryPtr* self)
{
    {
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

Vec_DirEntryPtr Vec_DirEntryPtr__clone(Vec_DirEntryPtr* self)
{
    {
if ((self->len == 0))     {
    return (Vec_DirEntryPtr){};
    }
    DirEntry** new_data = ((DirEntry**)(malloc((self->len * sizeof(DirEntry*)))));
    size_t i = 0;
while ((i < self->len))     {
(new_data[i] = self->data[i]);
(i = (i + 1));
    }
    return (Vec_DirEntryPtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

VecIterResult_DirEntryPtr VecIterRef_DirEntryPtr__next(VecIterRef_DirEntryPtr* self)
{
    {
if ((self->idx < self->count))     {
    struct DirEntry** item = (&self->data[self->idx]);
(self->idx = (self->idx + 1));
    return (VecIterResult_DirEntryPtr){.ptr = item};
    }
    return (VecIterResult_DirEntryPtr){};
    }
}

VecIterRef_DirEntryPtr VecIterRef_DirEntryPtr__iterator(VecIterRef_DirEntryPtr* self)
{
    {
    return (*self);
    }
}

bool VecIterResult_DirEntryPtr__is_none(VecIterResult_DirEntryPtr* self)
{
    {
    return (self->ptr == 0);
    }
}

struct DirEntry** VecIterResult_DirEntryPtr__unwrap(VecIterResult_DirEntryPtr* self)
{
    {
if ((self->ptr == 0))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->ptr;
    }
}

Option_DirEntryPtr VecIter_DirEntryPtr__next(VecIter_DirEntryPtr* self)
{
    {
if ((self->idx < self->count))     {
    struct DirEntry* item = self->data[self->idx];
(self->idx = (self->idx + 1));
    return Option_DirEntryPtr__Some(item);
    }
    return Option_DirEntryPtr__None();
    }
}

VecIter_DirEntryPtr VecIter_DirEntryPtr__iterator(VecIter_DirEntryPtr* self)
{
    {
    return (*self);
    }
}

Option_DirEntryPtr Option_DirEntryPtr__Some(struct DirEntry* v)
{
    {
    return (Option_DirEntryPtr){.is_some = true, .val = v};
    }
}

Option_DirEntryPtr Option_DirEntryPtr__None(void)
{
    {
    return (Option_DirEntryPtr){.is_some = false};
    }
}

bool Option_DirEntryPtr__is_some(Option_DirEntryPtr* self)
{
    {
    return self->is_some;
    }
}

bool Option_DirEntryPtr__is_none(Option_DirEntryPtr* self)
{
    {
    return (!self->is_some);
    }
}

void Option_DirEntryPtr__forget(Option_DirEntryPtr* self)
{
    {
memset((&self->val), 0, sizeof(struct DirEntry*));
    }
}

struct DirEntry* Option_DirEntryPtr__unwrap(Option_DirEntryPtr* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    struct DirEntry* v = self->val;
memset((&self->val), 0, sizeof(struct DirEntry*));
    return v;
    }
}

struct DirEntry** Option_DirEntryPtr__unwrap_ref(Option_DirEntryPtr* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

struct DirEntry* Option_DirEntryPtr__unwrap_or(Option_DirEntryPtr* self, struct DirEntry* def_val)
{
    {
if (self->is_some)     {
    return self->val;
    }
    return def_val;
    }
}

struct DirEntry* Option_DirEntryPtr__expect(Option_DirEntryPtr* self, char* msg)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Option_DirEntryPtr Option_DirEntryPtr__or_else(Option_DirEntryPtr* self, Option_DirEntryPtr other)
{
    {
if (self->is_some)     {
    return (*self);
    }
    return other;
    }
}

void Vec_DirEntryPtr__Drop_drop(Vec_DirEntryPtr* self)
{
    {
Vec_DirEntryPtr__free(self);
    }
}

Vec_DirEntry Vec_DirEntry__new(void)
{
    {
    return (Vec_DirEntry){};
    }
}

Vec_DirEntry Vec_DirEntry__with_capacity(size_t cap)
{
    {
if ((cap == 0))     {
    return (Vec_DirEntry){};
    }
    return (Vec_DirEntry){.data = ((DirEntry*)(malloc((cap * sizeof(DirEntry))))), .cap = cap};
    }
}

void Vec_DirEntry__grow(Vec_DirEntry* self)
{
    {
if ((self->cap == 0))     {
(self->cap = 8);
    }
 else     {
(self->cap = (self->cap * 2));
    }
(self->data = ((DirEntry*)(realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

void Vec_DirEntry__grow_to_fit(Vec_DirEntry* self, size_t new_len)
{
    {
if ((self->cap >= new_len))     {
    return ;
    }
if ((self->cap == 0))     {
(self->cap = 8);
    }
while ((self->cap < new_len))     {
(self->cap = (self->cap * 2));
    }
(self->data = ((DirEntry*)(realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

VecIter_DirEntry Vec_DirEntry__iterator(Vec_DirEntry* self)
{
    {
    return (VecIter_DirEntry){.data = self->data, .count = self->len};
    }
}

VecIterRef_DirEntry Vec_DirEntry__iter_ref(Vec_DirEntry* self)
{
    {
    return (VecIterRef_DirEntry){.data = self->data, .count = self->len};
    }
}

void Vec_DirEntry__push(Vec_DirEntry* self, DirEntry item)
{
    int __z_drop_flag_item = 1;
    {
if ((self->len >= self->cap))     {
Vec_DirEntry__grow(self);
    }
(self->data[self->len] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
(self->len = (self->len + 1));
    }
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

void Vec_DirEntry__insert(Vec_DirEntry* self, size_t idx, DirEntry item)
{
    int __z_drop_flag_item = 1;
    {
if ((idx > self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
if ((self->len >= self->cap))     {
Vec_DirEntry__grow(self);
    }
if ((idx < self->len))     {
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry)));
    }
(self->data[idx] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
(self->len = (self->len + 1));
    }
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

DirEntry Vec_DirEntry__pop(Vec_DirEntry* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->len = (self->len - 1));
    return self->data[self->len];
    }
}

Option_DirEntry Vec_DirEntry__pop_opt(Vec_DirEntry* self)
{
    {
if ((self->len == 0))     {
    return Option_DirEntry__None();
    }
(self->len = (self->len - 1));
    return Option_DirEntry__Some(self->data[self->len]);
    }
}

DirEntry Vec_DirEntry__remove(Vec_DirEntry* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_item = 1; DirEntry item = self->data[idx];
if ((idx < (self->len - 1)))     {
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry)));
    }
(self->len = (self->len - 1));
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
_z_ret_mv; });
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
    }
}

void Vec_DirEntry__append(Vec_DirEntry* self, Vec_DirEntry other)
{
    int __z_drop_flag_other = 1;
    {
    size_t new_len = (self->len + other.len);
Vec_DirEntry__grow_to_fit(self, new_len);
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry)));
(self->len = new_len);
    }
    if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
}

DirEntry Vec_DirEntry__get(Vec_DirEntry* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[idx];
    }
}

DirEntry Vec_DirEntry__index(Vec_DirEntry* self, size_t idx)
{
    {
    return Vec_DirEntry__get(self, idx);
    }
}

DirEntry* Vec_DirEntry__get_ref(Vec_DirEntry* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->data[idx]);
    }
}

DirEntry Vec_DirEntry__last(Vec_DirEntry* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[(self->len - 1)];
    }
}

size_t Vec_DirEntry__length(Vec_DirEntry* self)
{
    {
    return self->len;
    }
}

bool Vec_DirEntry__contains(Vec_DirEntry* self, DirEntry item)
{
    int __z_drop_flag_item = 1;
    {
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&item), sizeof(DirEntry)) == 0))     {
    { bool _z_ret = true;     if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }
    }
(i++);
    }
    { bool _z_ret = false;     if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }
    }
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

bool Vec_DirEntry__is_empty(Vec_DirEntry* self)
{
    {
    return (self->len == 0);
    }
}

void Vec_DirEntry__clear(Vec_DirEntry* self)
{
    {
(self->len = 0);
    }
}

void Vec_DirEntry__free(Vec_DirEntry* self)
{
    {
if (self->data)     {
free(self->data);
    }
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

DirEntry Vec_DirEntry__first(Vec_DirEntry* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[0];
    }
}

void Vec_DirEntry__set(Vec_DirEntry* self, size_t idx, DirEntry item)
{
    int __z_drop_flag_item = 1;
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->data[idx] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
    }
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
}

void Vec_DirEntry__reverse(Vec_DirEntry* self)
{
    {
    size_t i = 0;
    size_t j = (self->len - 1);
while ((i < j))     {
    int __z_drop_flag_tmp = 1; DirEntry tmp = self->data[i];
(self->data[i] = self->data[j]);
(self->data[j] = ({ __typeof__(tmp) _mv = tmp; __z_drop_flag_tmp = 0; memset(&tmp, 0, sizeof(tmp)); _mv; }));
(i++);
(j--);
    if (__z_drop_flag_tmp) DirEntry__Drop_glue(&tmp);
    }
    }
}

bool Vec_DirEntry__eq(Vec_DirEntry* self, Vec_DirEntry other)
{
    int __z_drop_flag_other = 1;
    {
if ((self->len != other.len))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
return _z_ret; }
    }
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&other.data[i]), sizeof(DirEntry)) != 0))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
return _z_ret; }
    }
(i = (i + 1));
    }
    { bool _z_ret = true;     if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
return _z_ret; }
    }
    if (__z_drop_flag_other) Vec_DirEntry__Drop_glue(&other);
}

void Vec_DirEntry__forget(Vec_DirEntry* self)
{
    {
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

Vec_DirEntry Vec_DirEntry__clone(Vec_DirEntry* self)
{
    {
if ((self->len == 0))     {
    return (Vec_DirEntry){};
    }
    DirEntry* new_data = ((DirEntry*)(malloc((self->len * sizeof(DirEntry)))));
    size_t i = 0;
while ((i < self->len))     {
(new_data[i] = self->data[i]);
(i = (i + 1));
    }
    return (Vec_DirEntry){.data = new_data, .len = self->len, .cap = self->len};
    }
}

VecIterResult_DirEntry VecIterRef_DirEntry__next(VecIterRef_DirEntry* self)
{
    {
if ((self->idx < self->count))     {
    DirEntry* item = (&self->data[self->idx]);
(self->idx = (self->idx + 1));
    return (VecIterResult_DirEntry){.ptr = item};
    }
    return (VecIterResult_DirEntry){};
    }
}

VecIterRef_DirEntry VecIterRef_DirEntry__iterator(VecIterRef_DirEntry* self)
{
    {
    return (*self);
    }
}

bool VecIterResult_DirEntry__is_none(VecIterResult_DirEntry* self)
{
    {
    return (self->ptr == 0);
    }
}

DirEntry* VecIterResult_DirEntry__unwrap(VecIterResult_DirEntry* self)
{
    {
if ((self->ptr == 0))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->ptr;
    }
}

Option_DirEntry VecIter_DirEntry__next(VecIter_DirEntry* self)
{
    {
if ((self->idx < self->count))     {
    int __z_drop_flag_item = 1; DirEntry item = self->data[self->idx];
(self->idx = (self->idx + 1));
    { Option_DirEntry _z_ret = Option_DirEntry__Some(({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));     if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
return _z_ret; }
    if (__z_drop_flag_item) DirEntry__Drop_glue(&item);
    }
    return Option_DirEntry__None();
    }
}

VecIter_DirEntry VecIter_DirEntry__iterator(VecIter_DirEntry* self)
{
    {
    return (*self);
    }
}

Option_DirEntry Option_DirEntry__Some(DirEntry v)
{
    int __z_drop_flag_v = 1;
    {
    { Option_DirEntry _z_ret = (Option_DirEntry){.is_some = true, .val = ({ __typeof__(v) _mv = v; __z_drop_flag_v = 0; memset(&v, 0, sizeof(v)); _mv; })};     if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
return _z_ret; }
    }
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
}

Option_DirEntry Option_DirEntry__None(void)
{
    {
    return (Option_DirEntry){.is_some = false};
    }
}

bool Option_DirEntry__is_some(Option_DirEntry* self)
{
    {
    return self->is_some;
    }
}

bool Option_DirEntry__is_none(Option_DirEntry* self)
{
    {
    return (!self->is_some);
    }
}

void Option_DirEntry__forget(Option_DirEntry* self)
{
    {
memset((&self->val), 0, sizeof(DirEntry));
    }
}

DirEntry Option_DirEntry__unwrap(Option_DirEntry* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_v = 1; DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));
memset((&self->val), 0, sizeof(DirEntry));
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
_z_ret_mv; });
    if (__z_drop_flag_v) DirEntry__Drop_glue(&v);
    }
}

DirEntry* Option_DirEntry__unwrap_ref(Option_DirEntry* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

DirEntry Option_DirEntry__unwrap_or(Option_DirEntry* self, DirEntry def_val)
{
    int __z_drop_flag_def_val = 1;
    {
if (self->is_some)     {
    { DirEntry _z_ret = self->val;     if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
return _z_ret; }
    }
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
_z_ret_mv; });
    }
    if (__z_drop_flag_def_val) DirEntry__Drop_glue(&def_val);
}

DirEntry Option_DirEntry__expect(Option_DirEntry* self, char* msg)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Option_DirEntry Option_DirEntry__or_else(Option_DirEntry* self, Option_DirEntry other)
{
    {
if (self->is_some)     {
    return (*self);
    }
    return other;
    }
}

void Vec_DirEntry__Drop_drop(Vec_DirEntry* self)
{
    {
Vec_DirEntry__free(self);
    }
}

Result_Metadata Result_Metadata__Ok(Metadata v)
{
    {
    return (Result_Metadata){.is_ok = true, .val = v};
    }
}

Result_Metadata Result_Metadata__Err(char* e)
{
    {
    return (Result_Metadata){.is_ok = false, .err = e};
    }
}

bool Result_Metadata__is_ok(Result_Metadata* self)
{
    {
    return self->is_ok;
    }
}

bool Result_Metadata__is_err(Result_Metadata* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_Metadata__forget(Result_Metadata* self)
{
    {
memset((&self->val), 0, sizeof(Metadata));
    }
}

Metadata Result_Metadata__unwrap(Result_Metadata* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    Metadata v = self->val;
memset((&self->val), 0, sizeof(Metadata));
    return v;
    }
}

Metadata* Result_Metadata__unwrap_ref(Result_Metadata* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

Metadata Result_Metadata__expect(Result_Metadata* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Result_bool Result_bool__Ok(bool v)
{
    {
    return (Result_bool){.is_ok = true, .val = v};
    }
}

Result_bool Result_bool__Err(char* e)
{
    {
    return (Result_bool){.is_ok = false, .err = e};
    }
}

bool Result_bool__is_ok(Result_bool* self)
{
    {
    return self->is_ok;
    }
}

bool Result_bool__is_err(Result_bool* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_bool__forget(Result_bool* self)
{
    {
memset((&self->val), 0, sizeof(bool));
    }
}

bool Result_bool__unwrap(Result_bool* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    bool v = self->val;
memset((&self->val), 0, sizeof(bool));
    return v;
    }
}

bool* Result_bool__unwrap_ref(Result_bool* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

bool Result_bool__expect(Result_bool* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Result_String Result_String__Ok(String v)
{
    int __z_drop_flag_v = 1;
    {
    { Result_String _z_ret = (Result_String){.is_ok = true, .val = ({ __typeof__(v) _mv = v; __z_drop_flag_v = 0; memset(&v, 0, sizeof(v)); _mv; })};     if (__z_drop_flag_v) String__Drop_glue(&v);
return _z_ret; }
    }
    if (__z_drop_flag_v) String__Drop_glue(&v);
}

Result_String Result_String__Err(char* e)
{
    {
    return (Result_String){.is_ok = false, .err = e};
    }
}

bool Result_String__is_ok(Result_String* self)
{
    {
    return self->is_ok;
    }
}

bool Result_String__is_err(Result_String* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_String__forget(Result_String* self)
{
    {
memset((&self->val), 0, sizeof(String));
    }
}

String Result_String__unwrap(Result_String* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));
memset((&self->val), 0, sizeof(String));
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_v) String__Drop_glue(&v);
_z_ret_mv; });
    if (__z_drop_flag_v) String__Drop_glue(&v);
    }
}

String* Result_String__unwrap_ref(Result_String* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

String Result_String__expect(Result_String* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Result_File Result_File__Ok(File v)
{
    {
    return (Result_File){.is_ok = true, .val = v};
    }
}

Result_File Result_File__Err(char* e)
{
    {
    return (Result_File){.is_ok = false, .err = e};
    }
}

bool Result_File__is_ok(Result_File* self)
{
    {
    return self->is_ok;
    }
}

bool Result_File__is_err(Result_File* self)
{
    {
    return (!self->is_ok);
    }
}

void Result_File__forget(Result_File* self)
{
    {
memset((&self->val), 0, sizeof(File));
    }
}

File Result_File__unwrap(Result_File* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    File v = self->val;
memset((&self->val), 0, sizeof(File));
    return v;
    }
}

File* Result_File__unwrap_ref(Result_File* self)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

File Result_File__expect(Result_File* self, char* msg)
{
    {
if ((!self->is_ok))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Vec_String Vec_String__new(void)
{
    {
    return (Vec_String){};
    }
}

Vec_String Vec_String__with_capacity(size_t cap)
{
    {
if ((cap == 0))     {
    return (Vec_String){};
    }
    return (Vec_String){.data = ((String*)(malloc((cap * sizeof(String))))), .cap = cap};
    }
}

void Vec_String__grow(Vec_String* self)
{
    {
if ((self->cap == 0))     {
(self->cap = 8);
    }
 else     {
(self->cap = (self->cap * 2));
    }
(self->data = ((String*)(realloc(self->data, (self->cap * sizeof(String))))));
    }
}

void Vec_String__grow_to_fit(Vec_String* self, size_t new_len)
{
    {
if ((self->cap >= new_len))     {
    return ;
    }
if ((self->cap == 0))     {
(self->cap = 8);
    }
while ((self->cap < new_len))     {
(self->cap = (self->cap * 2));
    }
(self->data = ((String*)(realloc(self->data, (self->cap * sizeof(String))))));
    }
}

VecIter_String Vec_String__iterator(Vec_String* self)
{
    {
    return (VecIter_String){.data = self->data, .count = self->len};
    }
}

VecIterRef_String Vec_String__iter_ref(Vec_String* self)
{
    {
    return (VecIterRef_String){.data = self->data, .count = self->len};
    }
}

void Vec_String__push(Vec_String* self, String item)
{
    int __z_drop_flag_item = 1;
    {
if ((self->len >= self->cap))     {
Vec_String__grow(self);
    }
(self->data[self->len] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
(self->len = (self->len + 1));
    }
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

void Vec_String__insert(Vec_String* self, size_t idx, String item)
{
    int __z_drop_flag_item = 1;
    {
if ((idx > self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
if ((self->len >= self->cap))     {
Vec_String__grow(self);
    }
if ((idx < self->len))     {
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(String)));
    }
(self->data[idx] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
(self->len = (self->len + 1));
    }
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

String Vec_String__pop(Vec_String* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->len = (self->len - 1));
    return self->data[self->len];
    }
}

Option_String Vec_String__pop_opt(Vec_String* self)
{
    {
if ((self->len == 0))     {
    return Option_String__None();
    }
(self->len = (self->len - 1));
    return Option_String__Some(self->data[self->len]);
    }
}

String Vec_String__remove(Vec_String* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_item = 1; String item = self->data[idx];
if ((idx < (self->len - 1)))     {
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(String)));
    }
(self->len = (self->len - 1));
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_item) String__Drop_glue(&item);
_z_ret_mv; });
    if (__z_drop_flag_item) String__Drop_glue(&item);
    }
}

void Vec_String__append(Vec_String* self, Vec_String other)
{
    int __z_drop_flag_other = 1;
    {
    size_t new_len = (self->len + other.len);
Vec_String__grow_to_fit(self, new_len);
memcpy((self->data + self->len), other.data, (other.len * sizeof(String)));
(self->len = new_len);
    }
    if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
}

String Vec_String__get(Vec_String* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[idx];
    }
}

String Vec_String__index(Vec_String* self, size_t idx)
{
    {
    return Vec_String__get(self, idx);
    }
}

String* Vec_String__get_ref(Vec_String* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->data[idx]);
    }
}

String Vec_String__last(Vec_String* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[(self->len - 1)];
    }
}

size_t Vec_String__length(Vec_String* self)
{
    {
    return self->len;
    }
}

bool Vec_String__contains(Vec_String* self, String item)
{
    int __z_drop_flag_item = 1;
    {
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&item), sizeof(String)) == 0))     {
    { bool _z_ret = true;     if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }
    }
(i++);
    }
    { bool _z_ret = false;     if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }
    }
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

bool Vec_String__is_empty(Vec_String* self)
{
    {
    return (self->len == 0);
    }
}

void Vec_String__clear(Vec_String* self)
{
    {
(self->len = 0);
    }
}

void Vec_String__free(Vec_String* self)
{
    {
if (self->data)     {
free(self->data);
    }
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

String Vec_String__first(Vec_String* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[0];
    }
}

void Vec_String__set(Vec_String* self, size_t idx, String item)
{
    int __z_drop_flag_item = 1;
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->data[idx] = ({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));
    }
    if (__z_drop_flag_item) String__Drop_glue(&item);
}

void Vec_String__reverse(Vec_String* self)
{
    {
    size_t i = 0;
    size_t j = (self->len - 1);
while ((i < j))     {
    int __z_drop_flag_tmp = 1; String tmp = self->data[i];
(self->data[i] = self->data[j]);
(self->data[j] = ({ __typeof__(tmp) _mv = tmp; __z_drop_flag_tmp = 0; memset(&tmp, 0, sizeof(tmp)); _mv; }));
(i++);
(j--);
    if (__z_drop_flag_tmp) String__Drop_glue(&tmp);
    }
    }
}

bool Vec_String__eq(Vec_String* self, Vec_String other)
{
    int __z_drop_flag_other = 1;
    {
if ((self->len != other.len))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
return _z_ret; }
    }
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&other.data[i]), sizeof(String)) != 0))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
return _z_ret; }
    }
(i = (i + 1));
    }
    { bool _z_ret = true;     if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
return _z_ret; }
    }
    if (__z_drop_flag_other) Vec_String__Drop_glue(&other);
}

void Vec_String__forget(Vec_String* self)
{
    {
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

Vec_String Vec_String__clone(Vec_String* self)
{
    {
if ((self->len == 0))     {
    return (Vec_String){};
    }
    String* new_data = ((String*)(malloc((self->len * sizeof(String)))));
    size_t i = 0;
while ((i < self->len))     {
(new_data[i] = self->data[i]);
(i = (i + 1));
    }
    return (Vec_String){.data = new_data, .len = self->len, .cap = self->len};
    }
}

VecIterResult_String VecIterRef_String__next(VecIterRef_String* self)
{
    {
if ((self->idx < self->count))     {
    String* item = (&self->data[self->idx]);
(self->idx = (self->idx + 1));
    return (VecIterResult_String){.ptr = item};
    }
    return (VecIterResult_String){};
    }
}

VecIterRef_String VecIterRef_String__iterator(VecIterRef_String* self)
{
    {
    return (*self);
    }
}

bool VecIterResult_String__is_none(VecIterResult_String* self)
{
    {
    return (self->ptr == 0);
    }
}

String* VecIterResult_String__unwrap(VecIterResult_String* self)
{
    {
if ((self->ptr == 0))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->ptr;
    }
}

Option_String VecIter_String__next(VecIter_String* self)
{
    {
if ((self->idx < self->count))     {
    int __z_drop_flag_item = 1; String item = self->data[self->idx];
(self->idx = (self->idx + 1));
    { Option_String _z_ret = Option_String__Some(({ __typeof__(item) _mv = item; __z_drop_flag_item = 0; memset(&item, 0, sizeof(item)); _mv; }));     if (__z_drop_flag_item) String__Drop_glue(&item);
return _z_ret; }
    if (__z_drop_flag_item) String__Drop_glue(&item);
    }
    return Option_String__None();
    }
}

VecIter_String VecIter_String__iterator(VecIter_String* self)
{
    {
    return (*self);
    }
}

Option_String Option_String__Some(String v)
{
    int __z_drop_flag_v = 1;
    {
    { Option_String _z_ret = (Option_String){.is_some = true, .val = ({ __typeof__(v) _mv = v; __z_drop_flag_v = 0; memset(&v, 0, sizeof(v)); _mv; })};     if (__z_drop_flag_v) String__Drop_glue(&v);
return _z_ret; }
    }
    if (__z_drop_flag_v) String__Drop_glue(&v);
}

Option_String Option_String__None(void)
{
    {
    return (Option_String){.is_some = false};
    }
}

bool Option_String__is_some(Option_String* self)
{
    {
    return self->is_some;
    }
}

bool Option_String__is_none(Option_String* self)
{
    {
    return (!self->is_some);
    }
}

void Option_String__forget(Option_String* self)
{
    {
memset((&self->val), 0, sizeof(String));
    }
}

String Option_String__unwrap(Option_String* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));
memset((&self->val), 0, sizeof(String));
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_v) String__Drop_glue(&v);
_z_ret_mv; });
    if (__z_drop_flag_v) String__Drop_glue(&v);
    }
}

String* Option_String__unwrap_ref(Option_String* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

String Option_String__unwrap_or(Option_String* self, String def_val)
{
    int __z_drop_flag_def_val = 1;
    {
if (self->is_some)     {
    { String _z_ret = self->val;     if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
return _z_ret; }
    }
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
_z_ret_mv; });
    }
    if (__z_drop_flag_def_val) String__Drop_glue(&def_val);
}

String Option_String__expect(Option_String* self, char* msg)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Option_String Option_String__or_else(Option_String* self, Option_String other)
{
    {
if (self->is_some)     {
    return (*self);
    }
    return other;
    }
}

void Vec_String__Drop_drop(Vec_String* self)
{
    {
Vec_String__free(self);
    }
}

Option_size_t Option_size_t__Some(size_t v)
{
    {
    return (Option_size_t){.is_some = true, .val = v};
    }
}

Option_size_t Option_size_t__None(void)
{
    {
    return (Option_size_t){.is_some = false};
    }
}

bool Option_size_t__is_some(Option_size_t* self)
{
    {
    return self->is_some;
    }
}

bool Option_size_t__is_none(Option_size_t* self)
{
    {
    return (!self->is_some);
    }
}

void Option_size_t__forget(Option_size_t* self)
{
    {
memset((&self->val), 0, sizeof(size_t));
    }
}

size_t Option_size_t__unwrap(Option_size_t* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    size_t v = self->val;
memset((&self->val), 0, sizeof(size_t));
    return v;
    }
}

size_t* Option_size_t__unwrap_ref(Option_size_t* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

size_t Option_size_t__unwrap_or(Option_size_t* self, size_t def_val)
{
    {
if (self->is_some)     {
    return self->val;
    }
    return def_val;
    }
}

size_t Option_size_t__expect(Option_size_t* self, char* msg)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Option_size_t Option_size_t__or_else(Option_size_t* self, Option_size_t other)
{
    {
if (self->is_some)     {
    return (*self);
    }
    return other;
    }
}

Vec_char Vec_char__new(void)
{
    {
    return (Vec_char){};
    }
}

Vec_char Vec_char__with_capacity(size_t cap)
{
    {
if ((cap == 0))     {
    return (Vec_char){};
    }
    return (Vec_char){.data = ((char*)(malloc((cap * sizeof(char))))), .cap = cap};
    }
}

void Vec_char__grow(Vec_char* self)
{
    {
if ((self->cap == 0))     {
(self->cap = 8);
    }
 else     {
(self->cap = (self->cap * 2));
    }
(self->data = ((char*)(realloc(self->data, (self->cap * sizeof(char))))));
    }
}

void Vec_char__grow_to_fit(Vec_char* self, size_t new_len)
{
    {
if ((self->cap >= new_len))     {
    return ;
    }
if ((self->cap == 0))     {
(self->cap = 8);
    }
while ((self->cap < new_len))     {
(self->cap = (self->cap * 2));
    }
(self->data = ((char*)(realloc(self->data, (self->cap * sizeof(char))))));
    }
}

VecIter_char Vec_char__iterator(Vec_char* self)
{
    {
    return (VecIter_char){.data = self->data, .count = self->len};
    }
}

VecIterRef_char Vec_char__iter_ref(Vec_char* self)
{
    {
    return (VecIterRef_char){.data = self->data, .count = self->len};
    }
}

void Vec_char__push(Vec_char* self, char item)
{
    {
if ((self->len >= self->cap))     {
Vec_char__grow(self);
    }
(self->data[self->len] = item);
(self->len = (self->len + 1));
    }
}

void Vec_char__insert(Vec_char* self, size_t idx, char item)
{
    {
if ((idx > self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
if ((self->len >= self->cap))     {
Vec_char__grow(self);
    }
if ((idx < self->len))     {
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(char)));
    }
(self->data[idx] = item);
(self->len = (self->len + 1));
    }
}

char Vec_char__pop(Vec_char* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->len = (self->len - 1));
    return self->data[self->len];
    }
}

Option_char Vec_char__pop_opt(Vec_char* self)
{
    {
if ((self->len == 0))     {
    return Option_char__None();
    }
(self->len = (self->len - 1));
    return Option_char__Some(self->data[self->len]);
    }
}

char Vec_char__remove(Vec_char* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    char item = self->data[idx];
if ((idx < (self->len - 1)))     {
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(char)));
    }
(self->len = (self->len - 1));
    return item;
    }
}

void Vec_char__append(Vec_char* self, Vec_char other)
{
    int __z_drop_flag_other = 1;
    {
    size_t new_len = (self->len + other.len);
Vec_char__grow_to_fit(self, new_len);
memcpy((self->data + self->len), other.data, (other.len * sizeof(char)));
(self->len = new_len);
    }
    if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
}

char Vec_char__get(Vec_char* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[idx];
    }
}

char Vec_char__index(Vec_char* self, size_t idx)
{
    {
    return Vec_char__get(self, idx);
    }
}

char* Vec_char__get_ref(Vec_char* self, size_t idx)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->data[idx]);
    }
}

char Vec_char__last(Vec_char* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[(self->len - 1)];
    }
}

size_t Vec_char__length(Vec_char* self)
{
    {
    return self->len;
    }
}

bool Vec_char__contains(Vec_char* self, char item)
{
    {
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&item), sizeof(char)) == 0))     {
    return true;
    }
(i++);
    }
    return false;
    }
}

bool Vec_char__is_empty(Vec_char* self)
{
    {
    return (self->len == 0);
    }
}

void Vec_char__clear(Vec_char* self)
{
    {
(self->len = 0);
    }
}

void Vec_char__free(Vec_char* self)
{
    {
if (self->data)     {
free(self->data);
    }
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

char Vec_char__first(Vec_char* self)
{
    {
if ((self->len == 0))     {
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->data[0];
    }
}

void Vec_char__set(Vec_char* self, size_t idx, char item)
{
    {
if ((idx >= self->len))     {
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
(self->data[idx] = item);
    }
}

void Vec_char__reverse(Vec_char* self)
{
    {
    size_t i = 0;
    size_t j = (self->len - 1);
while ((i < j))     {
    char tmp = self->data[i];
(self->data[i] = self->data[j]);
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

bool Vec_char__eq(Vec_char* self, Vec_char other)
{
    int __z_drop_flag_other = 1;
    {
if ((self->len != other.len))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
return _z_ret; }
    }
    size_t i = 0;
while ((i < self->len))     {
if ((memcmp((&self->data[i]), (&other.data[i]), sizeof(char)) != 0))     {
    { bool _z_ret = false;     if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
return _z_ret; }
    }
(i = (i + 1));
    }
    { bool _z_ret = true;     if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
return _z_ret; }
    }
    if (__z_drop_flag_other) Vec_char__Drop_glue(&other);
}

void Vec_char__forget(Vec_char* self)
{
    {
(self->data = 0);
(self->len = 0);
(self->cap = 0);
    }
}

Vec_char Vec_char__clone(Vec_char* self)
{
    {
if ((self->len == 0))     {
    return (Vec_char){};
    }
    char* new_data = ((char*)(malloc((self->len * sizeof(char)))));
    size_t i = 0;
while ((i < self->len))     {
(new_data[i] = self->data[i]);
(i = (i + 1));
    }
    return (Vec_char){.data = new_data, .len = self->len, .cap = self->len};
    }
}

VecIterResult_char VecIterRef_char__next(VecIterRef_char* self)
{
    {
if ((self->idx < self->count))     {
    char* item = (&self->data[self->idx]);
(self->idx = (self->idx + 1));
    return (VecIterResult_char){.ptr = item};
    }
    return (VecIterResult_char){};
    }
}

VecIterRef_char VecIterRef_char__iterator(VecIterRef_char* self)
{
    {
    return (*self);
    }
}

bool VecIterResult_char__is_none(VecIterResult_char* self)
{
    {
    return (self->ptr == 0);
    }
}

char* VecIterResult_char__unwrap(VecIterResult_char* self)
{
    {
if ((self->ptr == 0))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->ptr;
    }
}

Option_char VecIter_char__next(VecIter_char* self)
{
    {
if ((self->idx < self->count))     {
    char item = self->data[self->idx];
(self->idx = (self->idx + 1));
    return Option_char__Some(item);
    }
    return Option_char__None();
    }
}

VecIter_char VecIter_char__iterator(VecIter_char* self)
{
    {
    return (*self);
    }
}

Option_char Option_char__Some(char v)
{
    {
    return (Option_char){.is_some = true, .val = v};
    }
}

Option_char Option_char__None(void)
{
    {
    return (Option_char){.is_some = false};
    }
}

bool Option_char__is_some(Option_char* self)
{
    {
    return self->is_some;
    }
}

bool Option_char__is_none(Option_char* self)
{
    {
    return (!self->is_some);
    }
}

void Option_char__forget(Option_char* self)
{
    {
memset((&self->val), 0, sizeof(char));
    }
}

char Option_char__unwrap(Option_char* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    char v = self->val;
memset((&self->val), 0, sizeof(char));
    return v;
    }
}

char* Option_char__unwrap_ref(Option_char* self)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return (&self->val);
    }
}

char Option_char__unwrap_or(Option_char* self, char def_val)
{
    {
if (self->is_some)     {
    return self->val;
    }
    return def_val;
    }
}

char Option_char__expect(Option_char* self, char* msg)
{
    {
if ((!self->is_some))     {
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });
exit(1);
    }
    return self->val;
    }
}

Option_char Option_char__or_else(Option_char* self, Option_char other)
{
    {
if (self->is_some)     {
    return (*self);
    }
    return other;
    }
}

void Vec_char__Drop_drop(Vec_char* self)
{
    {
Vec_char__free(self);
    }
}

int main(int32_t ac, string* av)
{
    {
if ((ac != 2))     {
    ({ fprintf(stdout, "%s", "Usage: cppclass <FileName>[.cpp | .hpp]"); fprintf(stdout, "\n"); 0; });
    return 1;
    }
for (int i = (int)(0); av[1][i]; i++)     {
    char c = av[1][i];
if ((c == '/'))     {
    ({ fprintf(stdout, "%s", "Subdir are not suported yet"); fprintf(stdout, "\n"); 0; });
    return 1;
    }
if (((((((c < 'a') || (c > 'z')) && ((c < 'A') || (c > 'Z'))) && (c != '_')) && (c != '.')) && (((i != 1) || (c < '0')) || (c > '9'))))     {
    ({ fprintf(stdout, "%s", "Invalid character in file name: '"); fprintf(stdout, "%c", c); fprintf(stdout, "%s", "'"); fprintf(stdout, "\n"); 0; });
    return 1;
    }
    }
if (File__exists(av[1]))     {
    ({ fprintf(stdout, "%s", av[1]); fprintf(stdout, "%s", " alreeady exists"); fprintf(stdout, "\n"); 0; });
    }
    Result_FileType file_type = check_ext(av[1]);
if ((!Result_FileType__is_ok((&file_type))))     {
    ({ fprintf(stdout, "%s", file_type.err); fprintf(stdout, "\n"); 0; });
    return 1;
    }
{ FileType _m_0 = Result_FileType__unwrap((&file_type)); if (_m_0.tag == 0) {     {
gen_file(av[1], cpp_format());
    }
 } else if (_m_0.tag == 1) {     {
gen_file(av[1], hpp_format());
    }
 } };
    }
}

void gen_file(string file_name, string format)
{
    {
    Result_File res = File__open(file_name, "w");
if (Result_File__is_err((&res)))     {
    ({ fprintf(stdout, "%s", "Cant open file"); fprintf(stdout, "\n"); 0; });
    return ;
    }
    File file = Result_File__unwrap((&res));
    int __z_drop_flag_lower = 1; Vec_char lower = get_lower(file_name);
    int __z_drop_flag_upper = 1; Vec_char upper = get_upper(file_name);
    int __z_drop_flag_out = 1; Vec_char out = Vec_char__new();
    int32_t i = 0;
while (format[i])     {
if ((format[i] == '%'))     {
if ((format[(i + 1)] == 'n'))     {
Vec_char__append((&out), Vec_char__clone((&lower)));
(i++);
    }
 else if ((format[(i + 1)] == 'N'))     {
Vec_char__append((&out), Vec_char__clone((&upper)));
(i++);
    }
 else     {
Vec_char__push((&out), format[i]);
    }
    }
 else     {
Vec_char__push((&out), format[i]);
    }
(i++);
    }
Vec_char__push((&out), 0);
    Result_bool write_res = File__write_string((&file), out.data);
if (Result_bool__is_err((&write_res)))     {
    ({ fprintf(stdout, "%s", "Error writing to file: "); fprintf(stdout, "%s", write_res.err); fprintf(stdout, "\n"); 0; });
    }
 else     {
    ({ fprintf(stdout, "%s", "Successfully created "); fprintf(stdout, "%s", file_name); fprintf(stdout, "\n"); 0; });
    }
File__close((&file));
    if (__z_drop_flag_out) Vec_char__Drop_glue(&out);
    if (__z_drop_flag_upper) Vec_char__Drop_glue(&upper);
    if (__z_drop_flag_lower) Vec_char__Drop_glue(&lower);
    }
}

Vec_char get_upper(string file_name)
{
    {
    int32_t i = 0;
while (file_name[i])     {
(i++);
    }
    int __z_drop_flag_ret = 1; Vec_char ret = Vec_char__with_capacity(i);
(i = 0);
while (file_name[i])     {
    int32_t c = file_name[i];
if (((c >= 'a') && (c <= 'z')))     {
Vec_char__push((&ret), (c - 32));
    }
 else if ((c == '.'))     {
Vec_char__push((&ret), '_');
    }
 else     {
Vec_char__push((&ret), c);
    }
(i++);
    }
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_ret) Vec_char__Drop_glue(&ret);
_z_ret_mv; });
    if (__z_drop_flag_ret) Vec_char__Drop_glue(&ret);
    }
}

Vec_char get_lower(string file_name)
{
    {
    int32_t i = 0;
while (file_name[i])     {
(i++);
    }
(i = (i - 4));
    int __z_drop_flag_ret = 1; Vec_char ret = Vec_char__with_capacity(i);
    int32_t j = 0;
while ((j < i))     {
Vec_char__push((&ret), file_name[j]);
(j++);
    }
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_ret) Vec_char__Drop_glue(&ret);
_z_ret_mv; });
    if (__z_drop_flag_ret) Vec_char__Drop_glue(&ret);
    }
}

Result_FileType check_ext(string file_name)
{
    {
    int32_t len = 0;
while (file_name[len])     {
(len++);
    }
if ((len < 1))     {
    return Result_FileType__Err("Empty file name");
    }
if ((len < 3))     {
    return Result_FileType__Err("Wrong file extention");
    }
if ((((file_name[(len - 1)] != 'p') || (file_name[(len - 2)] != 'p')) || (file_name[(len - 4)] != '.')))     {
    return Result_FileType__Err("Wrong file extention");
    }
if ((file_name[(len - 3)] == 'c'))     {
    return Result_FileType__Ok(FileType_CPP());
    }
if ((file_name[(len - 3)] == 'h'))     {
    return Result_FileType__Ok(FileType_HPP());
    }
    return Result_FileType__Err("Wrong file extention");
    }
}

string hpp_format(void)
{
    {
    return ({     static char _b[4096]; _b[0]=0;
    char _t[128];
    strcat(_b, "#ifndef %N\n# define %N\n\nclass %n ");
    strcat(_b, "{");
    strcat(_b, "\n\npublic:\n\n\t%n();\n\n\t%n(const %n& other);\n\n\t%n& operator=(const %n& other);\n\n\t~%n();\n\n");
    strcat(_b, "}");
    strcat(_b, ";\n\n#endif\n");
    _b;
 });
    }
}

string cpp_format(void)
{
    {
    return ({     static char _b[4096]; _b[0]=0;
    char _t[128];
    strcat(_b, "#include \"%n.hpp\"\n\n%n::%n() ");
    strcat(_b, "{");
    strcat(_b, "}");
    strcat(_b, "\n\n%n::%n(const %n& other) ");
    strcat(_b, "{");
    strcat(_b, "}");
    strcat(_b, "\n\n%n& %n::operator=(const %n& other) ");
    strcat(_b, "{");
    strcat(_b, "\n\tif (this != &other) ");
    strcat(_b, "{");
    strcat(_b, "\n\t");
    strcat(_b, "}");
    strcat(_b, "\n\n\treturn (*this);\n");
    strcat(_b, "}");
    strcat(_b, "\n\n%n::~%n() ");
    strcat(_b, "{");
    strcat(_b, "}");
    strcat(_b, "\n");
    _b;
 });
    }
}

void _zen_panic(const char* file, int32_t line, const char* func, const char* msg)
{
    {
fprintf(stderr, "%s:%d (%s): Panic: %s\n", file, line, func, msg);
exit(1);
    }
}

Result_File File__open(char* path, char* mode)
{
    {
    void* h = _z_fs_fopen(path, mode);
if ((h == NULL))     {
    return Result_File__Err("Failed to open file");
    }
    return Result_File__Ok((struct File){.handle = h});
    }
}

void File__close(File* self)
{
    {
if (self->handle)     {
_z_fs_fclose(self->handle);
(self->handle = NULL);
    }
    }
}

Result_String File__read_to_string(File* self)
{
    {
if ((self->handle == NULL))     {
    return Result_String__Err("File not open");
    }
_z_fs_fseek(self->handle, 0, 2);
    int64_t size = _z_fs_ftell(self->handle);
_z_fs_fseek(self->handle, 0, 0);
    char* buffer = malloc((((size_t)(size)) + 1));
if ((buffer == NULL))     {
    return Result_String__Err("Out of memory");
    }
    size_t read = _z_fs_fread(buffer, 1, size, self->handle);
(buffer[read] = 0);
    int __z_drop_flag_s = 1; String s = String__new(buffer);
free(buffer);
    Result_String res = Result_String__Ok(({ __typeof__(s) _mv = s; __z_drop_flag_s = 0; memset(&s, 0, sizeof(s)); _mv; }));
String__forget((&s));
    Result_String ret = res;
Result_String__forget((&res));
    { Result_String _z_ret = ret;     if (__z_drop_flag_s) String__Drop_glue(&s);
return _z_ret; }
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

Result_String File__read_all(char* path)
{
    {
    Result_File res = File__open(path, "rb");
if (Result_File__is_err((&res)))     {
    return Result_String__Err(res.err);
    }
    File f = Result_File__unwrap((&res));
    Result_String s_res = File__read_to_string((&f));
File__close((&f));
    Result_String ret = s_res;
Result_String__forget((&s_res));
    return ret;
    }
}

Result_bool File__write_string(File* self, char* content)
{
    {
if ((self->handle == NULL))     {
    return Result_bool__Err("File not open");
    }
    ZC_AUTO len = strlen(content);
    size_t written = _z_fs_fwrite(content, 1, len, self->handle);
if ((written != len))     {
    return Result_bool__Err("Write incomplete");
    }
    return Result_bool__Ok(true);
    }
}

bool File__exists(char* path)
{
    {
    int zero = 0;
    return (access(path, 0) == zero);
    }
}

Result_Metadata File__metadata(char* path)
{
    {
    uint64_t size = 0;
    int is_d = 0;
    int is_f = 0;
    int res = _z_fs_get_metadata(path, (&size), (&is_d), (&is_f));
    int non_zero = 0;
if ((res != non_zero))     {
    return Result_Metadata__Err("Failed to get metadata");
    }
    return Result_Metadata__Ok((struct Metadata){.size = ((uint64_t)(size)), .is_dir = (is_d != non_zero), .is_file = (is_f != non_zero)});
    }
}

Result_bool File__create_dir(char* path)
{
    {
    int res = _z_fs_mkdir(path);
    int zero = 0;
if ((res != zero))     {
    return Result_bool__Err("Failed to create directory");
    }
    return Result_bool__Ok(true);
    }
}

Result_bool File__remove_file(char* path)
{
    {
    int res = unlink(path);
    int zero = 0;
if ((res != zero))     {
    return Result_bool__Err("Failed to remove file");
    }
    return Result_bool__Ok(true);
    }
}

Result_bool File__remove_dir(char* path)
{
    {
    int res = rmdir(path);
    int zero = 0;
if ((res != zero))     {
    return Result_bool__Err("Failed to remove directory");
    }
    return Result_bool__Ok(true);
    }
}

Result_Vec_DirEntry File__read_dir(char* path)
{
    {
    void* dir = _z_fs_opendir(path);
if ((dir == NULL))     {
    return Result_Vec_DirEntry__Err("Failed to open directory");
    }
    int __z_drop_flag_entries = 1; Vec_DirEntry entries = Vec_DirEntry__new();
    char* name_buf = malloc(256);
if ((name_buf == NULL))     {
_z_fs_closedir(dir);
    { Result_Vec_DirEntry _z_ret = Result_Vec_DirEntry__Err("Out of memory");     if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
return _z_ret; }
    }
    int is_d = 0;
    int is_d_zero = 0;
while (_z_fs_read_entry(dir, name_buf, 256, (&is_d)))     {
    int zero_cmp = 0;
if (((strcmp(name_buf, ".") == zero_cmp) || (strcmp(name_buf, "..") == zero_cmp)))     {
continue;
    }
    int __z_drop_flag_s = 1; String s = String__new(name_buf);
    int __z_drop_flag_ent = 1; DirEntry ent = (struct DirEntry){.name = ({ __typeof__(s) _mv = s; __z_drop_flag_s = 0; memset(&s, 0, sizeof(s)); _mv; }), .is_dir = (is_d != is_d_zero)};
String__forget((&s));
Vec_DirEntry__push((&entries), ({ __typeof__(ent) _mv = ent; __z_drop_flag_ent = 0; memset(&ent, 0, sizeof(ent)); _mv; }));
String__forget((&ent.name));
    if (__z_drop_flag_ent) DirEntry__Drop_glue(&ent);
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
free(name_buf);
_z_fs_closedir(dir);
    Result_Vec_DirEntry res = Result_Vec_DirEntry__Ok(({ __typeof__(entries) _mv = entries; __z_drop_flag_entries = 0; memset(&entries, 0, sizeof(entries)); _mv; }));
Vec_DirEntry__forget((&entries));
    Result_Vec_DirEntry ret = res;
Result_Vec_DirEntry__forget((&res));
    { Result_Vec_DirEntry _z_ret = ret;     if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
return _z_ret; }
    if (__z_drop_flag_entries) Vec_DirEntry__Drop_glue(&entries);
    }
}

Result_String File__current_dir(void)
{
    {
    char* buf = malloc(1024);
if ((buf == NULL))     {
    return Result_String__Err("Out of memory");
    }
if ((_z_fs_getcwd(buf, 1024) == NULL))     {
free(buf);
    return Result_String__Err("Failed to get current directory");
    }
    int __z_drop_flag_s = 1; String s = String__new(buf);
free(buf);
    Result_String res = Result_String__Ok(({ __typeof__(s) _mv = s; __z_drop_flag_s = 0; memset(&s, 0, sizeof(s)); _mv; }));
String__forget((&s));
    Result_String ret = res;
Result_String__forget((&res));
    { Result_String _z_ret = ret;     if (__z_drop_flag_s) String__Drop_glue(&s);
return _z_ret; }
    if (__z_drop_flag_s) String__Drop_glue(&s);
    }
}

String String__new(char* s)
{
    {
    ZC_AUTO len = strlen(s);
    int __z_drop_flag_v = 1; Vec_char v = Vec_char__new();
for (int i = (int)(0); i < len; i = (i + 1))     {
Vec_char__push((&v), s[i]);
    }
Vec_char__push((&v), 0);
    char* d = v.data;
    size_t l = v.len;
    size_t c = v.cap;
Vec_char__forget((&v));
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}};     if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

String String__from(char* s)
{
    {
    return String__new(s);
    }
}

char* String__c_str(String* self)
{
    {
    return self->vec.data;
    }
}

void String__destroy(String* self)
{
    {
Vec_char__free((&self->vec));
    }
}

void String__forget(String* self)
{
    {
Vec_char__forget((&self->vec));
    }
}

void String__append(String* self, String* other)
{
    {
if ((self->vec.len > 0))     {
(self->vec.len = (self->vec.len - 1));
    }
    size_t other_len = (*other).vec.len;
for (int i = (int)(0); i < other_len; i = (i + 1))     {
Vec_char__push((&self->vec), Vec_char__get((&(*other).vec), i));
    }
    }
}

void String__append_c(String* self, char* s)
{
    {
if ((self->vec.len > 0))     {
(self->vec.len = (self->vec.len - 1));
    }
    ZC_AUTO len = strlen(s);
for (int i = (int)(0); i < len; i = (i + 1))     {
Vec_char__push((&self->vec), s[i]);
    }
Vec_char__push((&self->vec), 0);
    }
}

void String__append_c_ptr(String* ptr, char* s)
{
    {
if ((ptr->vec.len > 0))     {
(ptr->vec.len = (ptr->vec.len - 1));
    }
    ZC_AUTO len = strlen(s);
for (int i = (int)(0); i < len; i = (i + 1))     {
Vec_char__push((&ptr->vec), s[i]);
    }
Vec_char__push((&ptr->vec), 0);
    }
}

String String__add(String* self, String* other)
{
    {
    int __z_drop_flag_new_s = 1; String new_s = String__from(String__c_str(self));
String__append((&new_s), other);
    char* d = new_s.vec.data;
    size_t l = new_s.vec.len;
    size_t c = new_s.vec.cap;
String__forget((&new_s));
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}};     if (__z_drop_flag_new_s) String__Drop_glue(&new_s);
return _z_ret; }
    if (__z_drop_flag_new_s) String__Drop_glue(&new_s);
    }
}

bool String__eq(String* self, String* other)
{
    {
    int zero = 0;
    return (strcmp(String__c_str(self), String__c_str((&(*other)))) == zero);
    }
}

bool String__eq_str(String* self, char* s)
{
    {
    int zero = 0;
    return (strcmp(String__c_str(self), s) == zero);
    }
}

size_t String__length(String* self)
{
    {
if ((self->vec.len == 0))     {
    return 0;
    }
    return (self->vec.len - 1);
    }
}

String String__substring(String* self, size_t start, size_t len)
{
    {
if (((start + len) > String__length(self)))     {
panic("substring out of bounds");
    }
    int __z_drop_flag_v = 1; Vec_char v = Vec_char__new();
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {
Vec_char__push((&v), Vec_char__get((&self->vec), (start + i)));
    }
Vec_char__push((&v), 0);
    char* d = v.data;
    size_t l = v.len;
    size_t c = v.cap;
Vec_char__forget((&v));
    { String _z_ret = (struct String){.vec = (Vec_char){.data = d, .len = l, .cap = c}};     if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
return _z_ret; }
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
}

Option_size_t String__find(String* self, char target)
{
    {
    size_t len = String__length(self);
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {
if ((Vec_char__get((&self->vec), i) == target))     {
    return Option_size_t__Some(i);
    }
    }
    return Option_size_t__None();
    }
}

void String__print(String* self)
{
    {
printf("%s", String__c_str(self));
fflush(stdout);
    }
}

void String__println(String* self)
{
    {
printf("%s\n", String__c_str(self));
    }
}

bool String__is_empty(String* self)
{
    {
    return (String__length(self) == 0);
    }
}

bool String__contains(String* self, char target)
{
    {
    return Option_size_t__is_some((__typeof__((String__find(self, target)))[]){String__find(self, target)});
    }
}

bool String__starts_with(String* self, char* prefix)
{
    {
    ZC_AUTO plen = strlen(prefix);
if ((plen > String__length(self)))     {
    return false;
    }
    int zero = 0;
    return (strncmp(String__c_str(self), prefix, plen) == zero);
    }
}

bool String__ends_with(String* self, char* suffix)
{
    {
    ZC_AUTO slen = strlen(suffix);
    size_t len = String__length(self);
if ((slen > len))     {
    return false;
    }
    int32_t offset = ((int32_t)((len - slen)));
    int zero = 0;
    return (strcmp((String__c_str(self) + offset), suffix) == zero);
    }
}

void String__reserve(String* self, size_t cap)
{
    {
Vec_char__grow_to_fit((&self->vec), (cap + 1));
    }
}

void String__free(String* self)
{
    {
Vec_char__free((&self->vec));
    }
}

size_t String___utf8_seq_len(char first_byte)
{
    {
    int32_t b = ((int32_t)(first_byte));
if (((b & 128) == 0))     {
    return 1;
    }
if (((b & 224) == 192))     {
    return 2;
    }
if (((b & 240) == 224))     {
    return 3;
    }
if (((b & 248) == 240))     {
    return 4;
    }
    return 1;
    }
}

size_t String__utf8_len(String* self)
{
    {
    size_t count = 0;
    size_t i = 0;
    size_t len = String__length(self);
while ((i < len))     {
    char c = Vec_char__get((&self->vec), i);
(i = (i + String___utf8_seq_len(c)));
(count = (count + 1));
    }
    return count;
    }
}

String String__utf8_at(String* self, size_t idx)
{
    {
    size_t count = 0;
    size_t i = 0;
    size_t len = String__length(self);
while ((i < len))     {
    char c = Vec_char__get((&self->vec), i);
    size_t seq = String___utf8_seq_len(c);
if ((count == idx))     {
    return String__substring(self, i, seq);
    }
(i = (i + seq));
(count = (count + 1));
    }
    return String__new("");
    }
}

String String__utf8_substr(String* self, size_t start_idx, size_t num_chars)
{
    {
if ((num_chars == 0))     {
    return String__new("");
    }
    size_t byte_start = 0;
    size_t byte_len = 0;
    size_t count = 0;
    size_t i = 0;
    size_t len = String__length(self);
    bool found_start = false;
while ((i < len))     {
if (((!found_start) && (count == start_idx)))     {
(byte_start = i);
(found_start = true);
(count = 0);
    }
 else if ((!found_start))     {
    char c = Vec_char__get((&self->vec), i);
(i = (i + String___utf8_seq_len(c)));
(count = (count + 1));
continue;
    }
if ((count < num_chars))     {
    char c = Vec_char__get((&self->vec), i);
    size_t seq = String___utf8_seq_len(c);
(byte_len = (byte_len + seq));
(i = (i + seq));
(count = (count + 1));
    }
 else     {
break;
    }
    }
if ((!found_start))     {
    return String__new("");
    }
    return String__substring(self, byte_start, byte_len);
    }
}

Vec_String String__split(String* self, char delim)
{
    {
    int __z_drop_flag_parts = 1; Vec_String parts = Vec_String__new();
    size_t len = String__length(self);
if ((len == 0))     {
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
_z_ret_mv; });
    }
    size_t start = 0;
    size_t i = 0;
while ((i < len))     {
if ((Vec_char__get((&self->vec), i) == delim))     {
Vec_String__push((&parts), String__substring(self, start, (i - start)));
(start = (i + 1));
    }
(i = (i + 1));
    }
if ((start <= len))     {
Vec_String__push((&parts), String__substring(self, start, (len - start)));
    }
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
_z_ret_mv; });
    if (__z_drop_flag_parts) Vec_String__Drop_glue(&parts);
    }
}

String String__trim(String* self)
{
    {
    size_t start = 0;
    size_t len = String__length(self);
    size_t end = len;
while ((start < len))     {
    char c = Vec_char__get((&self->vec), start);
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {
break;
    }
(start = (start + 1));
    }
if ((start == len))     {
    return String__new("");
    }
while ((end > start))     {
    char c = Vec_char__get((&self->vec), (end - 1));
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {
break;
    }
(end = (end - 1));
    }
    return String__substring(self, start, (end - start));
    }
}

String String__replace(String* self, char* target, char* replacement)
{
    {
    ZC_AUTO t_len = strlen(target);
if ((t_len == 0))     {
    return String__substring(self, 0, String__length(self));
    }
    size_t s_len = String__length(self);
    int __z_drop_flag_result = 1; String result = String__new("");
    size_t i = 0;
while ((i < s_len))     {
if (((i + t_len) <= s_len))     {
    bool is_match = true;
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {
if ((Vec_char__get((&self->vec), (i + k)) != target[k]))     {
(is_match = false);
break;
    }
    }
if (is_match)     {
    int __z_drop_flag_r_str = 1; String r_str = String__new(replacement);
String__append((&result), (&r_str));
(i = (i + t_len));
    if (__z_drop_flag_r_str) String__Drop_glue(&r_str);
continue;
    if (__z_drop_flag_r_str) String__Drop_glue(&r_str);
    }
    }
    int __z_drop_flag_v = 1; Vec_char v = Vec_char__new();
Vec_char__push((&v), Vec_char__get((&self->vec), i));
Vec_char__push((&v), 0);
    int __z_drop_flag_ch_s = 1; String ch_s = String__new(v.data);
String__append((&result), (&ch_s));
Vec_char__free((&v));
(i = (i + 1));
    if (__z_drop_flag_ch_s) String__Drop_glue(&ch_s);
    if (__z_drop_flag_v) Vec_char__Drop_glue(&v);
    }
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv));     if (__z_drop_flag_result) String__Drop_glue(&result);
_z_ret_mv; });
    if (__z_drop_flag_result) String__Drop_glue(&result);
    }
}
