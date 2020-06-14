UTPP - A New Generation of UnitTest++
=====================================

This is a test framework based on UnitTest++.

Latest version can be downloaded from [GitHub](https://neacsum.github.com/utpp).

There is also [documentation](https://neacsum.github.io/utpp) generated with Doxygen.


Author:
Mircea Neacsu (mircea@neacsu.net)

## License ##
The MIT License (MIT)
 
Copyright (c) 2017-2020 Mircea Neacsu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

For more information see the [LICENSE](LICENSE) file.

## Building ##
For Windows users, there is a Visual Studio 2019 solution file. Just open it and
build any configuration you need (debug, release, 32-bit, 64-bit). You can also
build everything using the `BUILD.bat` script. 

For Unix users there is a `makefile` that creates a static library `libutpp.a`.

## Usage ##
See [using.md](docs/using.md) for details.

More details about the internal architecture of this framework can be found in
the [architecture](docs/achitecture.md) file.
