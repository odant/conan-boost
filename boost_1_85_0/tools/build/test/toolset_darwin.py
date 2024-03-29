#!/usr/bin/env python3
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# validates the darwin toolset using a mock of gcc

from TestToolset import test_toolset

test_toolset("darwin", "4.2.1", [
    ["target-os=darwin"],
    ["target-os=darwin", "release", "strip=on"],
    ["target-os=darwin", "threading=multi"],
    ["target-os=darwin", "link=static"],
    ["target-os=darwin", "link=static", "runtime-link=static"],
# Address-model handling is quite broken
#    ["target-os=darwin", "architecture=x86", "address-model=32"]
])
