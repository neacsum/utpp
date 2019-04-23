UTPP - A New Generation of UnitTest++
=====================================

This test library is based on UnitTest++.

Latest version can be downloaded from [GitHub](https://neacsum.github.com/utpp) or
[BitBucket](https://bitbucket.org/neacsum/utpp).

There is also [documentation](https://neacsum.github.io/utpp) generated with Doxygen.


Author:
Mircea Neacsu (mircea@neacsu.net)
See architecture.md file for details

## License ##

The MIT License (MIT)
 
Copyright (c) 1999-2017 Mircea Neacsu

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


### UnitTest++ License ###
Version: v1.4
Last update: 2008-10-30

UnitTest++ is free software. You may copy, distribute, and modify it under
the terms of the License contained in the file COPYING distributed
with this package. This license is the same as the MIT/X Consortium
license.

Authors:
Noel Llopis (llopis@convexhull.com)  
Charles Nicholson (charles.nicholson@gmail.com)

Contributors:  
Jim Tilander  
Kim Grasman  
Jonathan Jansson  
Dirck Blaskey  
Rory Driscoll  
Dan Lind  
Matt Kimmel -- Submitted with permission from Blue Fang Games  
Anthony Moralez  
Jeff Dixon  
Randy Coulman  
Lieven van der Heide  

## Building ##
For Windows users, there is a Visual Studio 2017 solution file. Just open it and
build any configuration you need (debug, release, 32-bit, 64-bit).

For Unix users there is a makefile that creates a static library `libutpp.a`.
For 32-bit version add `ARCH=x86` to make command line. For debug version add
`_DEBUG=1`.

## Usage ##

See [using.md](docs/using.md) for details