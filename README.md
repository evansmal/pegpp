# pegpp

A PEG parser generator written in C++20. This project is a work-in-progress.

# Getting started

First clone the repository:

```sh
git clone git@github.com:evansmal/ccombinators.git
```

You can build the code with the following command:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build -j12
```

Enable tests by flipping `BUILD_TESTING` to `ON`. Building tests will automatically fetch [Catch2](https://github.com/catchorg/Catch2/).

Run the tests:

```sh
ctest --test-dir build/tests
```
Clean up the build directory with the following:

```sh
cmake --build build --target clean
```

# References

- [Understanding Parser Combinators](https://fsharpforfunandprofit.com/posts/understanding-parser-combinators-2/)
