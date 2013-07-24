# Installation

Because cipra is a header-only library, you don't even need to install
it.  You can just copy the contents of the `include/` directory into
your own project, and you will then be able to use cipra in your own
program.

If you want to use cipra's documentation or to install cipra to your
system, you can use our provided CMake system.

# Simple Installation

If you only want to install the header files to your system, you can
use the following steps:

    $ cd cipra/
    $ cmake -DWITH_DOXYGEN=OFF -DWITH_PKGCONFIG=OFF .
    $ sudo make install

No dependencies are required for this, outside of CMake and the
standard `make` tool.  Headers will be installed by default to the
`/usr/local/include` directory.

# Detailed Instructions

The above steps will only install the header files for cipra.  For
those who want to customize the installation, we provide the following
support in our CMake system:

## Custom Installation Path

By default, cipra will install itself into the `/usr/local` tree.
This can be changed by running the `cmake` command with the
`-DCMAKE_INSTALL_PREFIX=path` flag, where `path` is the path into
which you want to install cipra.  All of cipra's components will be
installed here.

## Doxygen

Doxygen is a documentation generator for C++ that takes comments in
the source files and turns them into formatted docs.  cipra's source
code has Doxygen-style comments in it, so Doxygen is able to produce
nicely-formatted documentation from cipra.

If you want to generate HTML documentation for the API and man pages,
you will need Doxygen 1.8.2 (or greater) installed.  cipra uses C++11
features that older versions of Doxygen do not support.  Doxygen is
turned on by default; just run CMake and both HTML and man page API
documentation will be turned on.  When you run `make` or `make
install`, the documentation will be generated and installed to your
system.  The HTML documentation can then be found under the
`share/doc/api` directory in your install path.

## Pkg-Config

Pkg-Config is a utility that saves compile-time information about a
library, so that applications that link with the library can more
easily use it.  Some information saved by Pkg-Config is the include
path, the library path, and the dependencies of the library.

In order to simplify using cipra, we provide a Pkg-Config file.  It is
installed by default when you run `make install`.
