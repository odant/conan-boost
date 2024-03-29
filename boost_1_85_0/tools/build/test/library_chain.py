#!/usr/bin/env python3

# Copyright 2003, 2004, 2005, 2006 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that a chain of libraries works ok, no matter if we use static or shared
# linking.

import BoostBuild
import os
import string
import sys

t = BoostBuild.Tester(use_test_config=False)

# Stage the binary, so that it will be relinked without hardcode-dll-paths.
# That will check that we pass correct -rpath-link, even if not passing -rpath.
t.write("jamfile.jam", """\
stage dist : main ;
exe main : main.cpp b ;
""")

t.write("main.cpp", """\
void foo();
int main() { foo(); }
""")

t.write("jamroot.jam", "")

t.write("a/a.cpp", """\
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
gee() {}
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
geek() {}
""")

t.write("a/jamfile.jam", "lib a : a.cpp ;")

t.write("b/b.cpp", """\
void geek();
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
foo() { geek(); }
""")

t.write("b/jamfile.jam", "lib b : b.cpp ../a//a ;")

t.run_build_system(["-d2"], stderr=None)
t.expect_addition("bin/$toolset/debug*/main.exe")
t.rm(["bin", "a/bin", "b/bin"])

t.run_build_system(["link=static"])
t.expect_addition("bin/$toolset/debug/link-static*/main.exe")
t.rm(["bin", "a/bin", "b/bin"])


# Check that <library> works for static linking.
t.write("b/jamfile.jam", "lib b : b.cpp : <library>../a//a ;")

t.run_build_system(["link=static"])
t.expect_addition("bin/$toolset/debug/link-static*/main.exe")

t.rm(["bin", "a/bin", "b/bin"])

t.write("b/jamfile.jam", "lib b : b.cpp ../a//a/<link>shared : <link>static ;")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/main.exe")

t.rm(["bin", "a/bin", "b/bin"])


# Test that putting a library in sources of a searched library works.
t.write("jamfile.jam", """\
exe main : main.cpp png ;
lib png : z : <name>png ;
lib z : : <name>zzz ;
""")

t.run_build_system(["-a", "-d+2"], status=None, stderr=None)
# Try to find the "zzz" string either in response file (for Windows compilers),
# or in the standard output.
rsp = t.adjust_names("bin/$toolset/debug*/main.exe.rsp")[0]
if os.path.exists(rsp) and ( open(rsp).read().find("zzz") != -1 ):
    pass
elif t.stdout().find("zzz") != -1:
    pass
else:
    t.fail_test(1)

# Test main -> libb -> liba chain in the case where liba is a file and not a
# B2 target.
t.rm(".")

t.write("jamroot.jam", "")
t.write("a/jamfile.jam", """\
lib a : a.cpp ;
install dist : a ;
""")

t.write("a/a.cpp", """\
#if defined(_WIN32)
__declspec(dllexport)
#endif
void a() {}
""")

t.run_build_system(subdir="a")
t.expect_addition("a/dist/a.dll")

if t.is_implib_expected():
    # This is a Windows import library.
    file = t.adjust_name("a.implib")
else:
    file = t.adjust_name("a.dll")

t.write("b/jamfile.jam", "lib b : b.cpp ../a/dist/%s ;" % file)

t.write("b/b.cpp", """\
#if defined(_WIN32)
__declspec(dllimport)
#endif
void a();
#if defined(_WIN32)
__declspec(dllexport)
#endif
void b() { a(); }
""")

t.write("jamroot.jam", "exe main : main.cpp b//b ;")

t.write("main.cpp", """\
#if defined(_WIN32)
__declspec(dllimport)
#endif
void b();
int main() { b(); }
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/main.exe")

t.cleanup()
