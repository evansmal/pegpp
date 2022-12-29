# ccombinators

A parser combinators library written in C++20. This project is a work-in-progress.

# Getting started

To build the project:

```sh
git clone git@github.com:evansmal/ccombinators.git
cd ccombinators && mkdir build && cmake -Bbuild -DBUILD_TESTING=OFF
cmake --build build
```

Enable tests by flipping `BUILD_TESTING` to `ON`. Building tests requires [Catch2](https://github.com/catchorg/Catch2/) to be installed locally and in the `CMAKE_PREFIX_PATH`.

# References

- [Understanding Parser Combinators](https://fsharpforfunandprofit.com/posts/understanding-parser-combinators-2/)
