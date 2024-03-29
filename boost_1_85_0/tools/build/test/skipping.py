#!/usr/bin/env python3

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that V2 does not fail gracelessy when any target is skipped.

import BoostBuild

# Create a temporary working directory.
t = BoostBuild.Tester(use_test_config=False)

t.write("a.cpp", "int main() {}\n")
t.write("b.cpp", "int main() {}\n")
t.write("c.cpp", "int main() {}\n")
t.write("jamroot.jam", """\
import feature ;
feature.feature foo : 1 2 : link-incompatible ;
exe a : a.cpp : <foo>1 ;
exe b : b.cpp : <foo>2 ;
exe c : c.cpp ;
""")

t.run_build_system(["foo=1"])

t.cleanup()
