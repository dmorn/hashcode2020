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
% cmake --build . # or `make`.
% ctest # or `make test`. `ctest -V` for verbose output.
```

## Current Status: 24,984,339 points
```
% ./testall.sh
computing solution for a_example.txt..
--- detected 2(2) valuable libraries out of 2
--- total score: 21

real	0m0.007s
user	0m0.001s
sys	0m0.003s
computing solution for b_read_on.txt..
--- detected 90(90) valuable libraries out of 100
--- total score: 5,822,900

real	0m0.042s
user	0m0.035s
sys	0m0.004s
computing solution for c_incunabula.txt..
--- detected 804(1298) valuable libraries out of 10000
--- total score: 3,627,746

real	0m0.082s
user	0m0.072s
sys	0m0.007s
computing solution for d_tough_choices.txt..
--- detected 14710(15000) valuable libraries out of 30000
--- total score: 4,810,585

real	0m0.102s
user	0m0.089s
sys	0m0.008s
computing solution for e_so_many_books.txt..
--- detected 131(132) valuable libraries out of 1000
--- total score: 4,602,123

real	0m0.192s
user	0m0.175s
sys	0m0.010s
computing solution for f_libraries_of_the_world.txt..
--- detected 17(17) valuable libraries out of 1000
--- total score: 5,240,161

real	0m0.208s
user	0m0.187s
sys	0m0.012s
```
