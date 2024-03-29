#!/usr/bin/env python3

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that C files are compiled by a C compiler.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("jamroot.jam", """
project ;
exe hello : hello.cpp a.c ;
""")

t.write("hello.cpp", """
extern "C" int foo();
int main() { return foo(); }
""")

t.write("a.c", """
// This will not compile unless in C mode.
int foo()
{
    int new = 0;
    new = (new+1)*7;
    return new;
}
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/hello.exe")

t.cleanup()
