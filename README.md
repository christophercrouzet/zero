Zero
====

Zero is a bunch of single-file libraries for C/C++.

It is written in C89 at the exception of a couple of features borrowed from C99,
namely fixed-width integer types and variadic macros.


## Features

* mostly C89-compliant
* headers don't include anything by default <sup>[1]</sup>
* implementations are included upon defining the `ZR_IMPLEMENTATION` macro
* each library is a standalone single file to ease integration into projects
* simple

1. The only exception being `<stdarg.h>` for headers defining functions with a
`va_list` object as parameter.


## Libraries


| library | description | latest version | changelog |
|---------|-------------|----------------|-----------|
**[allocator.h](include/zero/allocator.h)** | Aligned and non-aligned wrappers of malloc/realloc/free | 0.1.0 | [changelog](changelogs/allocator.md)
**[logger.h](include/zero/logger.h)** | Simple logger with different log levels and colouring | 0.1.0 | [changelog](changelogs/logger.md)
**[timer.h](include/zero/timer.h)** | High-resolution real time clock and CPU (user/system) clocks | 0.1.0 | [changelog](changelogs/timer.md)


## FAQ

### Why defining custom fixed-width integer types and even `size_t`?

Because most projects target common platforms (Windows, Linux, macOS), thus
using either the ILP32, LP64, or LLP64 data models, which all guarantee the
`char` type to be 8 bits, `short` to be 16 bits, `int` to be 32 bits, and
`long long` to be 64 bits. If such types can be accurately defined in a few
lines for most of the projects, then why including a standard header that
resolves to _thousands_ of lines of code with its dependencies? For the exotic
platforms, the macro `ZR_USE_STD_FIXED_TYPES` can be defined, or each type can
be overridden individually.

The same applies to redefining `size_t`—on almost all platforms the size of
`size_t` equals the targetted architecture, that is either 32 or 64 bits. Here
again, if that's not enough then it's still possible to define
the macro `ZR_USE_STD_BASIC_TYPES`.

Note that these custom types are only used for the public interface defined in
the headers, to avoid cluttering project headers including them. But the
implementation sections make free use of standard headers as needed
(including standard fixed-width integer types and `size_t`).
