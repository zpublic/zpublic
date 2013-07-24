# cipra Unit Testing Framework
@mainpage
![Version 1.1](screenshot.png)

cipra is a simple, TAP-compatible Unit Testing Framework for C++.
cipra is Free and Open Source Software, released under a 3-clause
BSD-style license.  It's written in 100% standard C++11 and is only a
couple header files, making it easy to include in your C++11 project.

TAP, the Test Anything Protocol, is a standard output format for
software unit test frameworks that was originally designed for Perl,
but can serve other languages.  It has a rich number of tools
("harnesses") that parse TAP formatted output and do useful things
with it.  TAP, however, is equally human-readable.

The name cipra (pronounced /ˈʃi.pɾaː/ "SHEE-prah") comes from the
lojban phrase "lo cipra", which means "the test".  It is properly
written with an initial minuscule 'c', even when at the start of a
sentence.

Included in this distribution are several other files for your
reference:

  * [INSTALL](@ref md_INSTALL): Instructions for installation and configuration.
  * [COPYING](@ref md_COPYING): License clarifications.
    * `COPYING.bsd`:  3-clause BSD-style license.
    * `COPYING.ccby`: CC-BY 3.0 US license.
  * [AUTHORS](@ref md_AUTHORS): Author(s) of the software.
  * [NEWS](@ref md_NEWS): Changes between releases.

## Usage

You can find a tutorial on the [Tutorial page](@ref md_tutorial) or in the
`tutorial.md` file in the `doc/` directory.

Where possible, cipra's test interface is analogous to Perl's
`Test::More` module.

To use cipra, just include the `cipra.hpp` header file in your test
file.  You do not need to link to any library.  All methods are inline
in the header files; there is no need to compile cipra.

@include usage.cpp <!-- see usage.cpp for an example -->

## Resources

  * [Home Page](https://sourceforge.net/projects/cipra/)
  * [Git Repo](https://sourceforge.net/p/cipra/code/ci/master/tree/)
  * [TAP Specification Website](http://testanything.org/)
