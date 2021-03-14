#include <gtest/gtest.h>

#include <multiload.h>

TEST(multiload, dlopen) {
    // dlopen claims to have opened a library
    void* dll = multiload_dlopen("test_lib.so", MULTILOAD_RTLD_NOW);

    EXPECT_NE(dll, nullptr);

    multiload_dlclose(dll);
}

TEST(multiload, dlsym) {
    // dlsym claims to have loaded functions
    void* dll = multiload_dlopen("test_lib.so", MULTILOAD_RTLD_NOW);
    ASSERT_NE(dll, nullptr);

    auto get_x = multiload_dlsym(dll, "get_x");
    auto set_x = multiload_dlsym(dll, "set_x");

    EXPECT_NE(get_x, nullptr);
    EXPECT_NE(set_x, nullptr);

    multiload_dlclose(dll);
}

int get_int();
void set_int(int);

TEST(multiload, functions) {
    // dlsym'd functions appear to work
    void* dll = multiload_dlopen("test_lib.so", MULTILOAD_RTLD_NOW);
    ASSERT_NE(dll, nullptr);

    auto get_x = reinterpret_cast<decltype(&get_int)>(multiload_dlsym(dll, "get_x"));
    auto set_x = reinterpret_cast<decltype(&set_int)>(multiload_dlsym(dll, "set_x"));

    ASSERT_NE(get_x, nullptr);
    ASSERT_NE(set_x, nullptr);

    EXPECT_EQ(get_x(), 0);
    set_x(2);
    EXPECT_EQ(get_x(), 2);

    multiload_dlclose(dll);
}

TEST(multiload, independence) {
    // multiple dlopened libraries appear to be independent
    void* dll_1 = multiload_dlopen("test_lib.so", MULTILOAD_RTLD_NOW);
    ASSERT_NE(dll_1, nullptr);
    auto get_x_1 = reinterpret_cast<decltype(&get_int)>(multiload_dlsym(dll_1, "get_x"));
    auto set_x_1 = reinterpret_cast<decltype(&set_int)>(multiload_dlsym(dll_1, "set_x"));
    ASSERT_NE(get_x_1, nullptr);
    ASSERT_NE(set_x_1, nullptr);

    void* dll_2 = multiload_dlopen("test_lib.so", MULTILOAD_RTLD_NOW);
    ASSERT_NE(dll_2, nullptr);
    auto get_x_2 = reinterpret_cast<decltype(&get_int)>(multiload_dlsym(dll_2, "get_x"));
    auto set_x_2 = reinterpret_cast<decltype(&set_int)>(multiload_dlsym(dll_2, "set_x"));
    ASSERT_NE(get_x_2, nullptr);
    ASSERT_NE(set_x_2, nullptr);

    EXPECT_EQ(get_x_1(), 0);
    set_x_1(2);
    EXPECT_EQ(get_x_1(), 2);

    EXPECT_EQ(get_x_2(), 0);
    set_x_2(3);
    EXPECT_EQ(get_x_2(), 3);

    EXPECT_EQ(get_x_1(), 2);

    multiload_dlclose(dll_1);
    multiload_dlclose(dll_2);
}
