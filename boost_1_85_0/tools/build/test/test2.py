#!/usr/bin/env python3

# Copyright 2002, 2003 Dave Abrahams
# Copyright 2002, 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester()

t.set_tree("test2")

file_list = 'bin/$toolset/debug*/' * \
    BoostBuild.List("foo.exe foo.obj")

t.run_build_system(["-sBOOST_BUILD_PATH=" + t.original_workdir + "/.."])
t.expect_addition(file_list)


t.write("foo.cpp", "int main() {}\n")
t.run_build_system(["-d2", "-sBOOST_BUILD_PATH=" + t.original_workdir + "/.."])
t.expect_touch(file_list)

t.cleanup()
