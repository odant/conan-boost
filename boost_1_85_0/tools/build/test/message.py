#!/usr/bin/env python3

# Copyright (C) Vladimir Prus 2003.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE.txt or copy at
#         https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test for the regression testing framework.

import BoostBuild

# Create a temporary working directory.
t = BoostBuild.Tester(["-d0"], use_test_config=False)

t.write("Jamroot.jam", """
project
  :
  : usage-requirements <define>TEST=1
  : default-build <link>static
;
message hello : "Hello World!" ;
alias hello : : <link>shared ;
obj test : test.cpp hello : <link>static ;
""")

t.write("test.cpp", """
#ifndef TEST
#error TEST not defined
#endif
""")

t.run_build_system(["test"], stdout="""Hello World!
""")

t.expect_addition("bin/$toolset/link-static*/test.obj")
t.expect_nothing_more()

t.cleanup()
