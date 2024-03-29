#!/usr/bin/env python3

# Copyright 2018 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Tests the parsing of tokens

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write("file.jam", """\
rule test1 ( args * )
{
    EXIT $(args) : 0 ;
}

test1
a # a comment
# another comment
b
c #| a multiline comment |# d
#| another
multiline
comment
|#
e "#f" ;
""")

t.run_build_system(["-ffile.jam"], stdout="""\
a b c d e #f
""")

t.cleanup()
