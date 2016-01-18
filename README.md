SWATCH Control Software Package {#mainpage}
===============================

__Swatch__ = <b>S</b>oftWare for <b>A</b>utomating con<b>T</b>rol of <b>C</b>ommon <b>H</b>ardware

Documentation of the SWATCH software library

The user's guide for subsystem developers can be found here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWATCHUsersGuide


# Notes for developers of SWATCH

## Run the unit tests 

The tests are based on the boost unit test framework. Before running the unit tests, you can setup
your `LD_LIBRARY_PATH` using `test/env.sh` as follows:
~~~
source test/env.sh
~~~
This script also sets the `BOOST_TEST_LOG_LEVEL` environment variable, so that the boost unit test 
framework prints to the console at the start and end of each test suite/case. 

You can run tests either for all of swatch, per subsystem or individually
 - all:
~~~
./test/bin/boostTest.exe
~~~

 - per subsystem (library)
~~~
./test/bin/console_test_runner.exe --test system/test/lib/libcactus_swatch_system_test.so
~~~

 - individual:
~~~
 ./test/bin/boostTest.exe --run_test=SystemTestSuite/BuildSystem
~~~

You can change the log threshold for the boost unit test framework using the `--log_level` 
command-line option; e.g. to display all messages (including each time that a check passes):
~~~
./test/bin/boostTest --log_level=all
~~~


## Other
Other tests can be run via
~~~
./bin/SwatchTest.exe json_build
./bin/SwatchTest.exe sys_explore
~~~

## Vagrant
If you are not working on a Scientific Linux 6 machine, it might be worthwhile
to have a look at Vagrant so you can compile the code and run the tests on your machine.
Instructions for swatch are provided here:
https://github.com/cms-l1t-online/cactus-vagrant-box

## Docker
Even easier than Vagrant, you can use a pre-built docker container for code development. Simply install Docker on your machine and launch the SWATCH container via
```
docker run -v <path to cactus on your machine>/cactus/cactuscore/swatch:/opt/swatch -t -i kreczko/swatch
```
and you will be given a shell inside the docker container. Then
```
cd /opt/swatch
# to compile
make jobs=2
```
The ```docker run``` command will download the docker image ```kreczko/swatch```, mount your local swatch path to /opt/swatch inside the container and start it.
