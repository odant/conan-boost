+++
title = "`value_type &assume_value() & noexcept`"
description = "Narrow contract lvalue reference observer of any value present. Constexpr propagating, never throws."
categories = ["observers"]
weight = 600
+++

Narrow contract lvalue reference observer of any value present. `NoValuePolicy::narrow_value_check()` is first invoked, then the reference to the value is returned. The convention is that hard undefined behaviour occurs if no value is actually present, however `NoValuePolicy::narrow_value_check()` can do something to avoid that.

Note that if `value_type` is `void`, only a `const` overload returning `void` is present.

*Requires*: Always available.

*Complexity*: Depends on `NoValuePolicy::narrow_value_check()`.

*Guarantees*: An exception is never thrown.
