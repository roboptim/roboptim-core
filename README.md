roboptim-core
=============

[![Build Status](https://travis-ci.org/roboptim/roboptim-core.png?branch=master)](https://travis-ci.org/roboptim/roboptim-core)
[![Coverage Status](https://coveralls.io/repos/roboptim/roboptim-core/badge.png)](https://coveralls.io/r/roboptim/roboptim-core)

For general information about the project, please refer to its
homepage: http://www.roboptim.net/


What is it?
-----------

Unless Python or Ruby packages, streamlined C++ application deployment
is extremely challenging. Portability issues, compilation flags,
interoperability between packages is notable difficult to ensure.

This submodule contains a set of shell scripts which can be used to
help Travis integrate with various services and tools useful for any
C++ project.

In particular:

 * Setup dependencies both from apt and from source. In the source
   case, dependencies are built and installed in a local prefix.
 * Compile and test the software
 * Using the [jrl-cmakemodules][]:
   * Run a CMake compatible `make distcheck` (build a release tarball
     and the rebuild the whole project from it.
   * Upload the latest Doxygen documentation to GitHub pages.
 * Integrate with [cppcheck][], [nsiqcppstyle][], [coverity][] to run
   static analysis on the code and detect issues.

 * Annotate (see [git-notes][]) successful builds with dependencies
   commit id to be able to reproduce again the exact same successful
   build.

 [jrl-cmakemodules]: https://github.com/jrl-umi3218/jrl-cmakemodules

How can I install roboptim-core?
--------------------------------

RobOptim uses [CMake](http://www.cmake.org/) to generate build files. For
instance, if you want to build roboptim-core in release with debug info, and
install it in `/my/prefix`, go to the root of the project folder and type:

    $ mkdir -p build && cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=/my/prefix -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ..
    $ make && make test
    $ sudo make install

Where is the library documentation?
-----------------------------------

This **README** only covers configure/building issues. For more information
regarding this library usage, please refer to the Doxygen documentation.

If you have configured the package as explained in the first section, go
into your `build` directory and type:

    $ make doc

To view the HTML documentation: go in the `doc/doxygen-html` directory
and open `index.html` with your favorite internet browser.


How to use Valgrind with the test suite?
----------------------------------------

All the tests launched by the test suite can be prefixed
with the environment variable `CHECK_PREFIX`.

    $ cmake -DCHECK_PREFIX='valgrind --log-file=valgrind.log' ..
    $ make && make check


Available packages
------------------

 * Debian and derivatives, i.e. Ubuntu (Release 2.0):
   http://packages.qa.debian.org/r/roboptim-core.html
 * Arch Linux (Release 2.0):
   https://aur.archlinux.org/packages/roboptim-core/
 * Arch Linux (Git master branch):
   https://aur.archlinux.org/packages/roboptim-core-git/
 * Fedora (Release 0.5):
   https://apps.fedoraproject.org/packages/roboptim-core


Development Snapshost (*unstable*):

 * Ubuntu Launchpad PPA:
   https://launchpad.net/~roboptim/+archive/ppa
