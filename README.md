roboptim-core
=============

[![Build Status](https://travis-ci.org/roboptim/roboptim-core.png?branch=master)](https://travis-ci.org/roboptim/roboptim-core)

For general information about the project, please refer to its
homepage: http://www.roboptim.net/

Where is the library documentation?
-----------------------------------

This README only covers configure/building issues, for more information
regarding this library usage, please refer to the Doxygen documentation.

If you have configured the package as explained in the first section, go
into you ``_build'' directory and type:

    $ make doc

To view the HTML documentation: go in the `doc/doxygen-html` directory
and open the `index.html` file with your favorite internet browser.


How to use Valgrind with the test suite?
----------------------------------------

All the tests launched by the test suite can be prefixed
with the environment variable ``CHECK_PREFIX''.

    $ cmake -DCHECK_PREFIX='valgrind --log-file=valgrind.log' ..
    $ make && make check
