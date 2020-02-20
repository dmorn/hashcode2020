# hashcode2020
Team 84cb48 google hashcode solution code.

## Build and Test
Clone repository and generate appropriate makefiles.
```
% git clone --recurse-submodules https://github.com/jecoz/hashcode2020.git
% cd hashcode2020
% cmake -H. -Bbuild # generate makefiles inside build/
```
Enter build/, make and test.
```
% cd build
% cmake build . # or `make`.
% ctest # or `make test`. `ctest -V` for verbose output.
```
