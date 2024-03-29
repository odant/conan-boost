#!/usr/bin/env python3

# Copyright 2003 Douglas Gregor
# Copyright 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", "import gcc ;")

t.write("jamfile.jam", """
import print ;
print.output foo ;
print.text \\\"Something\\\" ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Something\"""")

t.write("jamfile.jam", """
import print ;
print.output foo ;
print.text \\\n\\\"Somethingelse\\\" ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Something\"
\"Somethingelse\"""")

t.write("jamfile.jam", """
import print ;
print.output foo ;
print.text \\\"Different\\\" : true ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Different\"""")

t.cleanup()
