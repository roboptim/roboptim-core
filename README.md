roboptim-core
=============

[![License LGPL 3][badge-license]](http://www.gnu.org/licenses/lgpl-3.0.txt)
[![Build Status](https://travis-ci.org/roboptim/roboptim-core.png?branch=master)](https://travis-ci.org/roboptim/roboptim-core)
[![Coverage Status](https://coveralls.io/repos/roboptim/roboptim-core/badge.png)](https://coveralls.io/r/roboptim/roboptim-core)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/863/badge.svg)](https://scan.coverity.com/projects/863)
[![Join the chat at https://gitter.im/roboptim/development](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/roboptim/development?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


[![Zenodo](https://zenodo.org/badge/doi/10.5281/zenodo.10332.png)](http://zenodo.org/record/10332)

This package is the Core layer of the RobOptim framework. It is
released under the [LGPL-3](COPYING.LESSER) license.

**Warning:** this repository contains [Git
submodules][git-submodules]. Please clone this repository using the
`git clone --recursive` command. If you already have cloned the
repository, you can run `git submodule init && git submodule update`
to retrieve the submodules.


For general information about the project, please refer to its
homepage: http://www.roboptim.net/


Documentation
-------------

To get started with this library, please read the [online Doxygen
documentation][doxygen-documentation].

It can also be generated locally by running the `make doc`
command. After the package is installed, the documentation will be
located in the `$prefix/share/doc/roboptim-core` directory where
`$prefix` is your installation prefix (`/usr/local` by default).


Getting Help
------------

Support is provided through:
 * the RobOptim mailing-list: roboptim@googlegroups.com
 * the following Gitter room: https://gitter.im/roboptim/development


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
 * [CMake][] (>= 2.8) a build system
 * [pkg-config][] dependency tracking tool
 * [Doxygen][] a documentation generation tool
 * [Boost][] C++ library
 * [Eigen][] (>= 3.2) C++ template library for linear algebra
 * [log4cxx][] logging framework
 * [Libtool][] and its ltdl library for portable plug-in management
 * a C++03 compliant modern C++ compiler such as GCC or clang


### Compiling and installing the package

The manual compilation requires two steps:

 1. configuration of the build and generation of the build files
 2. compilation of the sources and installation of the package

roboptim-core uses [CMake][] to generate build
files. It is recommended to create a separate build directory:

```sh
mkdir _build         # (1) Create a build directory
cd _build            # (2) Go to the newly created build directory
cmake [options] ..   # (3) Generate the build files
```

Options which can be passed to CMake are detailed in the next section.

```sh
make                 # (4) Compile the package
make test            # (5) Execute the package tests
make install         # (6) Install the package into the prefix (see step 3)
```


### Options

Additional options can be set on the command line through the
following command: `-D<option>=<value>`.

For instance: `cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..` will set
the `CMAKE_BUILD_TYPE` option to the value `RelWithDebInfo`.


Available options are:

- `CMAKE_BUILD_TYPE` set the build profile that should be used (debug,
  release, etc.). We recommend `RelWithDebInfo` as it will provide
  performances while keeping debugging symbols enabled.
- `CMAKE_INSTALL_PREFIX` set the installation prefix (the directory
  where the software will be copied to after it has been compiled).

### Concerning plug-ins

Plug-ins are installed to `$libdir/roboptim-core` by default. However, since
libtool needs to find the plug-ins, the proper environment variable needs to be
exported (e.g. in your `~/.bashrc` or `~/.zshrc`):

```sh
# $libdir should be the lib directory in your installation prefix, e.g. /usr/lib
export LTDL_LIBRARY_PATH="$LTDL_LIBRARY_PATH:$libdir/roboptim-core"
```

Failing to set the environment variable will lead to the following error when
trying to load a plug-in:

```txt
libltdl failed to load plug-in ``roboptim-core-plugin-xxxxx'': file not found
```

You can also use the `PLUGIN_SUBDIR` option to change the subdirectory. For
instance, if you want your plug-ins to be installed to the default library
directory (no subdirectory), set it to the empty string, e.g.:

```sh
cmake -DPLUGIN_SUBDIR="" ..
```

In that case, `LTDL_LIBRARY_PATH` does not need to be changed if you installed
RobOptim to a system prefix (e.g. `/usr` or `/usr/local`).

Tips and Tricks
---------------

### How to use Valgrind with the test suite?

All the tests launched by the test suite can be prefixed
with the environment variable `CHECK_PREFIX`.

```sh
cmake -DCHECK_PREFIX='valgrind --log-file=valgrind.log' ..
make && make test
```


Contributing
------------

If you want to contribute, please refer to the
[CONTRIBUTING.md](CONTRIBUTING.md) file.


Credits
-------

This package authors are credited in the [AUTHORS](AUTHORS) file.


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
 * Mac OS X Homebrew Formula (Release 2.0 / Git HEAD):
   https://www.github.com/roboptim/homebrew-roboptim
 * RobotPkg (Release 1.0):
   http://robotpkg.openrobots.org/robotpkg/optimization/roboptim-core/

Development Snapshost ( *unstable* ):

 * Ubuntu Launchpad PPA:
   https://launchpad.net/~roboptim/+archive/ppa


[badge-license]: https://img.shields.io/badge/license-LGPL_3-green.svg

[doxygen-documentation]: http://www.roboptim.net/roboptim-core/doxygen/HEAD/

[git-submodules]: http://git-scm.com/book/en/Git-Tools-Submodules

[Boost]: http://www.boost.org/
[CMake]: http://www.cmake.org/
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Eigen]: http://eigen.tuxfamily.org/
[Git]: http://git-scm.com/
[Libtool]: https://www.gnu.org/software/libtool/
[log4cxx]: https://logging.apache.org/log4cxx/
[pkg-config]: http://www.freedesktop.org/wiki/Software/pkg-config/
[RobotPkg]: http://robotpkg.openrobots.org/
