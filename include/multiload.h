#ifndef MULTILOAD_H_
#define MULTILOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

enum multiload_flags {
MULTILOAD_RTLD_LAZY = 1,
MULTILOAD_RTLD_NOW  = 2
};

void* multiload_dlopen(char const*, multiload_flags flags);
int multiload_dlclose(void*);
void* multiload_dlsym(void*, char const*);

#ifdef __cplusplus
}
#endif

#endif /* MULTILOAD_H_ */
