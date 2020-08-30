# Database with Trie Search Tree

[![Build Status](https://travis-ci.com/joaromera/trie-database.svg?branch=master)](https://travis-ci.com/joaromera/trie-database)

## Introduction

This project was done in the context of the subject _Algorithms and Data Structures 2_. Its goal was to exercise writing _abstract data types_ with their _formal specification_, and providing an implementation to them in C++ using our own data structures.

To keep this project interesting, to me at least, I'm currently working towards these goals:

1. Move to `C++17`.
2. Follow C++ best practices and core guidelines.
3. Improve the testing done with `Google Test`.
4. Set a CI pipeline with `Travis` for building and testing. Get coverage with `Codecov`.
5. Use tools from the ecosystem: `cppcheck`, `clang-format`, `clang-tidy`, `valgrind`.

---

### Documents

The original problem set is [here](docs/tp2_enunciado.pdf) (spanish only).

Formal specification can be found [here](docs/tp2_especificacion.pdf). This is also spanish only, but it is worth to take a look at the degree of formality the project aimed to.

I used `Doxygen` to document the code base. The generated html is `docs/html/index.html`. Unfortunately, it's outdated.

## What to look for... 👀

Probably the most interesting place to look at is the implementation of [Datum.h](src/Datum.h). This is the type that stores the values of each _cell_ in the database. I've drawn some inspiration from _Sean Parent's Better Code_ talk [Better Code: Runtime Polymorphism](https://www.youtube.com/watch?v=QGcVXgEVMJg) (also visit Sean Parent's [site](https://sean-parent.stlab.cc/) for more information on his papers and talks). There's still a lot to improve but the exercise was definitely worth the effort.

The implementation of the [Trie](https://en.wikipedia.org/wiki/Trie) search tree: [string_map.h](src/string_map.h) and [string_map.hpp](src/string_map.hpp).

## How to run

Execute the following lines in the repo's root:

```bash
mkdir build && cd build
cmake ..
make
./correrTests
```

---
