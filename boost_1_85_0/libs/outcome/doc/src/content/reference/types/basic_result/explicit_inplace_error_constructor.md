+++
title = "`explicit basic_result(in_place_type_t<error_type_if_enabled>, Args ...)`"
description = "Explicit inplace error constructor. Available if `predicate::enable_inplace_error_constructor<Args ...>` is true. Constexpr, triviality and noexcept propagating."
categories = ["constructors", "explicit-constructors", "inplace-constructors"]
weight = 420
+++

Explicit inplace error constructor. Calls {{% api "void on_result_in_place_construction(T *, in_place_type_t<U>, Args &&...) noexcept" %}} with `this`, `in_place_type<error_type>` and `Args ...`.

*Requires*: `predicate::enable_inplace_error_constructor<Args ...>` is true.

*Complexity*: Same as for the `error_type` constructor which accepts `Args ...`. Constexpr, triviality and noexcept of underlying operations is propagated.

*Guarantees*: If an exception is thrown during the operation, the state of the Args is left indeterminate.
