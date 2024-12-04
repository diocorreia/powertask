# PowerTask - A Simple Energy Aware Task Scheduler

>[!warning]
> This library is still under development.
## How to test

* Install dependencies: CMake, GCC and CppUTest

* Create a build directory:
```sh
mkdir build
```

* Inside that directory, run CMake in order to produce the Makefiles:
```sh
cd build
cmake ..
```

* Build the tests. Calling `make` will build all the existing tests.
```sh
make
```

* Run the test:
```sh
./tests/[module_under_test]/test_[module_component]
```

e.g.
```sh
./tests/scheduler/test_scheduler
```