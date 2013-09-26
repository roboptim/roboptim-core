roboptim-core
=============

[![Build Status](https://travis-ci.org/roboptim/roboptim-core.png?branch=master)](https://travis-ci.org/roboptim/roboptim-core)
[![Coverage Status](https://coveralls.io/repos/roboptim/roboptim-core/badge.png)](https://coveralls.io/r/roboptim/roboptim-core)

For general information about the project, please refer to its
homepage: http://www.roboptim.net/

How can I install roboptim-core?
--------------------------------

*STOP!*

First question: do you need to compile this package from source
manually?

The answer is yes if:

 1. There is no native package available for your system and you do
 not want to use an external system such as [RobotPkg] to handle your
 dependencies.
 1. You want to develop new features for this package and you do not
 want to use [RobotPkg] on top of your system to handle this.

If the answer is yes, then please proceed. Otherwise, please checkout
the "Available Package" section at the end of this page.


### Installing dependencies

RobOptim uses the following tools:

 * [Git][] a source content management system
 * [CMake][] a build system
 * [pkg-config][] dependency tracking tool
 * [Doxygen][] a documentation generation tool
 * [Boost][] C++ library
 * [Eigen][] C++ template library for linear algebra
 * [log4cxx][] logging framework
 * [Libtool][] and its ltdl library for portable plug-in management
 * a C++03 compliant modern C++ compiler such as GCC or clang

[Boost]: http://www.boost.org/
[CMake]: htttp://www.cmake.org/
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Eigen]: http://eigen.tuxfamily.org/
[Git]: http://git-scm.com/
[Libtool]: https://www.gnu.org/software/libtool/
[log4cxx]: https://logging.apache.org/log4cxx/
[pkg-config]: http://www.freedesktop.org/wiki/Software/pkg-config/

### Compiling the package

RobOptim uses [CMake](http://www.cmake.org/) to generate build files. For
instance, if you want to build roboptim-core in release with debug info, and
install it in `/my/prefix`, go to the root of the project folder and type:

    $ mkdir -p build && cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=/my/prefix -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ..
    $ make && make test
    $ sudo make install


[RobotPkg]: http://robotpkg.openrobots.org/

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


Available Packages
------------------

 * Debian and derivatives, i.e. Ubuntu (Release 2.0):
   http://packages.qa.debian.org/r/roboptim-core.html
 * Arch Linux (Release 2.0):
   https://aur.archlinux.org/packages/roboptim-core/
 * Arch Linux (Git master branch):
   https://aur.archlinux.org/packages/roboptim-core-git/
 * Fedora (Release 0.5):
   https://apps.fedoraproject.org/packages/roboptim-core
 * RobotPkg (Release 1.0):
   http://robotpkg.openrobots.org/robotpkg/optimization/roboptim-core/

Development Snapshost (*unstable*):

 * Ubuntu Launchpad PPA:
   https://launchpad.net/~roboptim/+archive/ppa
