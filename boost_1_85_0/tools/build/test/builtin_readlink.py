#!/usr/bin/env python3

# Copyright 2012 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild
import os
import sys
from unittest.mock import patch

t = BoostBuild.Tester(pass_toolset=0)

t.write("link-target", "")
try:
    with patch.dict(os.environ, {var: "winsymlinks:nativestrict" for var in ["MSYS", "CYGWIN"]}):
        os.symlink("link-target", "link")
except (AttributeError, OSError) as e:
    # Either OS does not support symlinks or not enough privilege
    print("XFAIL: %s" % e)
    t.cleanup()
    sys.exit()

t.write("file.jam", """
ECHO [ READLINK link ] ;
EXIT [ READLINK link-target ] : 0 ;
""")

t.run_build_system(["-ffile.jam"], stdout="""link-target

""")

t.cleanup()
