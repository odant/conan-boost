#!/usr/bin/env python3

# Copyright (C) Vladimir Prus 2006.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test the 'customization' example.

import BoostBuild

t = BoostBuild.Tester()

t.set_tree("../example/customization")

t.run_build_system()

t.expect_addition(["bin/$toolset/debug*/codegen.exe",
                   "bin/$toolset/debug*/usage.cpp"])

t.cleanup()
