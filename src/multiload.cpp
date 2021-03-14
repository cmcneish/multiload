#include "multiload.h"
#include <mutex>
#include <atomic>
#include <filesystem>
#include <dlfcn.h>
#include <sys/stat.h>

namespace multiload {

static std::atomic<size_t> ctr = 0;

int dlopen_flags(multiload_flags flags)
{
    switch (flags)
    {
    case MULTILOAD_RTLD_LAZY:
        return RTLD_LAZY;
        break;
    case MULTILOAD_RTLD_NOW:
        return RTLD_NOW;
        break;
    default:
        // we're just passing these on to dlopen anyways, it can reject them if it doesn't like them
        return static_cast<int>(flags);
        break;
    }
}

void* make_handle(char const* path, multiload_flags flags, std::string prefix)
{
    std::filesystem::create_directory("/tmp/multiload");

    std::filesystem::path orig_path = path;

    std::filesystem::path filename = orig_path.filename();
    std::filesystem::path new_path = std::filesystem::temp_directory_path() / (prefix + std::string(filename));

    std::filesystem::copy_file(orig_path, new_path);

    void* ret = ::dlopen(new_path.c_str(), dlopen_flags(flags));

    std::filesystem::remove(new_path);
    return ret;
}

void* dlopen(char const* path, multiload_flags flags)
{
    size_t idx = ++ctr;
    std::string prefix = std::to_string(idx) + "_";
    return make_handle(path, flags, std::move(prefix));
}

int dlclose(void* handle)
{
    return ::dlclose(handle);
}

void* dlsym(void* handle, char const* sym)
{
    return ::dlsym(handle, sym);
}

} // namespace multiload

void* multiload_dlopen(char const* path, multiload_flags flags)
{
    return multiload::dlopen(path, flags);
}

int multiload_dlclose(void* handle)
{
    return multiload::dlclose(handle);
}

void* multiload_dlsym(void* handle, char const* sym)
{
    return multiload::dlsym(handle, sym);
}
