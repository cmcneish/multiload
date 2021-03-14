gtest_dep = dependency('gtest', main : true)

shared_library('test_lib', 'test_lib.c', name_prefix : '')

test_exe = executable('multiload_test',
    'test.cpp',
    dependencies : [
        multiload_dep,
        gtest_dep,
    ])

test('multiload_test', test_exe)
