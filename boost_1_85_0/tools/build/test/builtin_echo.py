#!/usr/bin/env python3

# Copyright 2012 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# This tests the ECHO rule.

import BoostBuild

def test_echo(name):
    t = BoostBuild.Tester(["-ffile.jam"], pass_toolset=0)

    t.write("file.jam", """\
%s ;
UPDATE ;
""" % name)
    t.run_build_system(stdout="\n")

    t.write("file.jam", """\
%s a message ;
UPDATE ;
""" % name)
    t.run_build_system(stdout="a message\n")

    t.cleanup()

test_echo("ECHO")
test_echo("Echo")
test_echo("echo")
