#!/usr/bin/env python3

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2006 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that we can specify sources using relative names.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

# Test that relative path to source, 'src', is preserved.
t.write("jamroot.jam", "exe a : src/a.cpp ;")
t.write("src/a.cpp", "int main() {}\n")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/src/a.obj")

# Test that the relative path to source is preserved
# when using 'glob'.
t.rm("bin")
t.write("jamroot.jam", "exe a : [ glob src/*.cpp ] ;")
t.run_build_system()
t.expect_addition("bin/$toolset/debug*/src/a.obj")


# Test that relative path with ".." is *not* added to
# target path.
t.rm(".")
t.write("jamroot.jam", "")
t.write("a.cpp", "int main() { return 0; }\n")
t.write("build/Jamfile", "exe a : ../a.cpp ; ")
t.run_build_system(subdir="build")
t.expect_addition("build/bin/$toolset/debug*/a.obj")

t.cleanup()
