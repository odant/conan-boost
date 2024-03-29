#!/usr/bin/env python3

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Tests that on gcc, we correctly report a problem when static runtime is
# requested for building a shared library.

import BoostBuild

t = BoostBuild.Tester()
t.write("jamroot.jam", "lib hello : hello.cpp ;")
t.write("hello.cpp", "int main() {}\n")

t.run_build_system(["runtime-link=static"])
t.expect_output_lines("warning: On gcc, DLLs can not be built with "
    "'<runtime-link>static'.")
t.expect_nothing_more()

t.run_build_system(["link=static", "runtime-link=static"])
t.expect_addition("bin/$toolset/debug*/link-static*/hello.obj")
t.expect_addition("bin/$toolset/debug*/link-static*/hello.lib")
t.expect_nothing_more()

t.cleanup()
