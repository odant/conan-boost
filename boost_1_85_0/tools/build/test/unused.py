#!/usr/bin/env python3

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that unused sources are at least reported.

import BoostBuild

t = BoostBuild.Tester(["-d+2"], use_test_config=False)

t.write("a.cpp", "int main() {}\n")
t.write("b.cpp", "\n")
t.write("b.xyz", "")
t.write("jamroot.jam", """\
import "class" : new ;
import modules ;
import project ;
import targets ;
import type ;
import virtual-target ;

type.register X : xyz ;

class test-target-class : basic-target
{
    rule construct ( name : source-targets * : property-set )
    {
        local result = [ property-set.empty ] ;
        if ! [ modules.peek : GENERATE_NOTHING ]
        {
            result += [ virtual-target.from-file b.xyz : . : $(self.project) ] ;
            if ! [ modules.peek : GENERATE_ONLY_UNUSABLE ]
            {
                result += [ virtual-target.from-file b.cpp : . : $(self.project)
                    ] ;
            }
        }
        return $(result) ;
    }

    rule compute-usage-requirements ( rproperties : targets * )
    {
        return [ property-set.create <define>FOO ] ;
    }
}

rule make-b-main-target
{
    local project = [ project.current ] ;
    targets.main-target-alternative [ new test-target-class b : $(project) ] ;
}

exe a : a.cpp b c ;
make-b-main-target ;
alias c ;  # Expands to nothing, intentionally.
""")

t.run_build_system()

# The second invocation should do nothing, and produce no warning. The previous
# invocation might have printed executed actions and other things, so it is not
# easy to check if a warning was issued or not.
t.run_build_system(stdout="")

t.run_build_system(["-sGENERATE_ONLY_UNUSABLE=1"], stdout="")

# Check that even if main target generates nothing, its usage requirements are
# still propagated to dependants.
t.write("a.cpp", """\
#ifndef FOO
    #error We refuse to compile without FOO being defined!
    We_refuse_to_compile_without_FOO_being_defined
#endif
int main() {}
""")
t.run_build_system(["-sGENERATE_NOTHING=1"])

t.cleanup()
