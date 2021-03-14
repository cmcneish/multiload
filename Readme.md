Multiload: load a library more than once
========================================

How does this work?
-------------------
Copy and paste. Rather than loading your library, we load
`/tmp/multiload/0_your_library.so`. Rather than incrementing a refcount when
loading your library a second time, we load `/tmp/multiload/0_your_library.so`.
This tricks `dlopen` into giving you a new copy of your library, rather than the
old one.

Caveats
-------
 * No Windows support. There's nothing preventing this in theory and I've
   created this library for Windows before, but it's harder to guarantee
   cleanup there. On Linux, I can create and immediately delete a file, meaning
   it won't leak unless the program crashes in that small window of time. On
   Windows, I can't, so I have to keep track of what file a library handle came
   from, so I can delete it on disk when closing it. Plus,
   `__declspec(dllexport)`-type stuff is annoying.
 * There's no `dlopenat` call, so the filesystem code is racy. Try to avoid
   loading different libraries with the same name at the same time, and know
   that if someone can write to `/tmp/multiload` they can cause you to run
   arbitrary code. I don't think these are fixable without reimplementing
   `dlopen` to take a fd or `FILE*`.
 * I haven't tested musl, or anything except glibc. Likewise for any of the
   BSDs.
 * C++17, need a recent compiler. May have to add -lc++fs.

License
-------
LGPL v2.1
