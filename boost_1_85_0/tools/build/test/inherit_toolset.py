#!/usr/bin/env python3

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild
import string

t = BoostBuild.Tester(pass_toolset=0)

t.write("a.cpp", "\n")

t.write("yfc1.jam", """\
import feature ;
import generators ;

feature.extend toolset : yfc1 ;
rule init ( ) { }

generators.register-standard yfc1.compile : CPP : OBJ : <toolset>yfc1 ;
generators.register-standard yfc1.link : OBJ : EXE : <toolset>yfc1 ;

actions compile { yfc1-compile }
actions link { yfc1-link }
""")

t.write(
    'yfc1.py',
"""
from b2.build import feature, generators
from b2.manager import get_manager

MANAGER = get_manager()
ENGINE = MANAGER.engine()

feature.extend('toolset', ['yfc1'])

generators.register_standard('yfc1.compile', ['CPP'], ['OBJ'], ['<toolset>yfc1'])
generators.register_standard('yfc1.link', ['OBJ'], ['EXE'], ['<toolset>yfc1'])

ENGINE.register_action(
    'yfc1.compile',
    'yfc1-compile'
)

ENGINE.register_action(
    'yfc1.link',
    'yfc1-link'
)

def init(*args):
    pass

"""
)

t.write("yfc2.jam", """\
import feature ;
import toolset ;

feature.extend toolset : yfc2 ;
toolset.inherit yfc2 : yfc1 ;
rule init ( ) { }

actions link { yfc2-link }
""")

t.write(
    'yfc2.py',
"""
from b2.build import feature, toolset
from b2.manager import get_manager

MANAGER = get_manager()
ENGINE = MANAGER.engine()

feature.extend('toolset', ['yfc2'])
toolset.inherit('yfc2', 'yfc1')

ENGINE.register_action('yfc2.link', 'yfc2-link')

def init(*args):
    pass
"""
)

t.write("jamfile.jam", "exe a : a.cpp ;")
t.write("jamroot.jam", "using yfc1 ;")

t.run_build_system(["-n", "-d2", "yfc1"])
t.fail_test(t.stdout().find("yfc1-link") == -1)

# Make sure we do not have to explicitly 'use' yfc1.
t.write("jamroot.jam", "using yfc2 ;")

t.run_build_system(["-n", "-d2", "yfc2"])
t.fail_test(t.stdout().find("yfc2-link") == -1)

t.cleanup()
