# SLB

> C++ Standard Library Backport

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![license][badge.license]][license]

[badge.travis]: https://travis-ci.org/mpark/slb.svg?branch=master
[badge.appveyor]: https://ci.appveyor.com/api/projects/status/github/mpark/slb?branch=master&svg=true
[badge.license]: https://img.shields.io/badge/license-boost-blue.svg

[travis]: https://travis-ci.org/mpark/slb
[appveyor]: https://ci.appveyor.com/project/mpark/slb
[license]: https://github.com/mpark/slb/blob/master/LICENSE.md

## Test

This directory contains the tests for __SLB__.

## Build / Run

Execute the following commands from the top-level directory:

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```
