#!/usr/bin/env python3

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that the <library> property has no effect on "obj" targets. Previously,
# it affected all targets, so
#
#    project : requirements <library>foo ;
#    exe a : a.cpp helper ;
#    obj helper : helper.cpp : <optimization>off ;
#
# caused 'foo' to be built with and without optimization.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("jamroot.jam", """
project : requirements <library>lib//x ;
exe a : a.cpp foo ;
obj foo : foo.cpp : <variant>release ;
""")

t.write("a.cpp", """
void aux();
int main() { aux(); }
""")

t.write("foo.cpp", """
void gee();
void aux() { gee(); }
""")

t.write("lib/x.cpp", """
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
gee() {}
""")

t.write("lib/jamfile.jam", """
lib x : x.cpp ;
""")

t.write("lib/jamroot.jam", """
""")


t.run_build_system()
t.expect_addition("bin/$toolset/debug*/a.exe")
t.expect_nothing("lib/bin/$toolset/release/x.obj")

t.cleanup()
