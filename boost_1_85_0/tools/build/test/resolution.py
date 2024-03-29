#!/usr/bin/env python3

# Copyright (C) Vladimir Prus 2006.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Tests for the target id resolution process.

import BoostBuild

# Create a temporary working directory.
t = BoostBuild.Tester(use_test_config=False)

# Create the needed files
t.write("jamroot.jam", """\
exe hello : hello.cpp ;
# This should use the 'hello' target, even if there is a 'hello' file in the
# current dir.
install s : hello : <location>. ;
""")

t.write("hello.cpp", "int main() {}\n")

t.run_build_system()

t.expect_addition("bin/$toolset/debug*/hello.obj")

t.touch("hello.cpp")
t.run_build_system(["s"])
# If 'hello' in the 's' target resolved to file in the current dir, nothing
# will be rebuilt.
t.expect_touch("bin/$toolset/debug*/hello.obj")

t.cleanup()
