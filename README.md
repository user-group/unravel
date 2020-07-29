1. Introduction 

Unravel is a prototype program slicer for ANSI C developed at the National Institute of Standards and Technology (NIST). Unravel is usually easy to install but, there are some rough edges that can bite. This file identifies the supporting software that you need, gives some hints for finding it on your system or the Internet, and gives instructions for running make.

2. Unravel Requirements

Unravel was originally written for the UNIX (or POSIX) environment. Unravel assumes the following programs are available:

* csh (C Shell)
* date
* echo
* lex
* sed
* yacc
* a C compiler; at the moment, gcc is assumed.

The following C libraries should be available, with installation commands for Ubuntu and similar Linux distributions:

* Xaw

```sudo apt-get install libxaw7-dev```

* Xmu

```sudo apt-get install libxmu-dev```

* Xt

```sudo apt-get install libxt-dev```

* Xext

```sudo apt-get install libxext-dev```

* X11

```sudo apt-get install libx11-dev```


The echo command should allow the -n option (to indicate no newline). The original unravel development was under SunOs 4.1 but, we have compiled under Solaris, ULTRIX, SGI/IRIX and DEC OSF/1, and Linux Ubuntu. The unravel interface was developed under X Window System X11R5 using the Xt toolkit and the MIT Athena widgets. Unravel should compile and run under X11R6; it also compiles and runs under X11R4. Unravel needs a C preprocessor (that is ANSI C compliant if unravel is to be used on ANSI C source code) from a C compiler to invoke when unravel is looking at source code. Unravel compiles under either a K&R C compiler or an ANSI C compiler. The gcc compiler is ANSI compliant, produced by the Free Software Foundation (the GNU project).


3. Running make on Unravel

Before making unravel, the following should be done:
The original source code for version 2.2 is available here: 

```https://www.nist.gov/services-resources/software/unravel```

That version was adapted to get it to run on Linux Ubuntu. To compile the latter:

* download the source code
* run make

uncompress and un-tar the source code somewhere on your system Locate the X Window System tree with includes and libs
Decide on a C compiler for installing unravel
Decide on a home for installing unravel


3.1. C Compiler

The C compiler is specified in two places, the Makefile and the file config.h.
In the makefile, look for the compiler definition line: 

```CC = gcc```

By default, CC is set to gcc.


3.2. Install Directory

Once the make finishes, the unravel files cannot be moved since the location (directory) where the unravel source code is located is compiled into the unravel programs so that they can find certain files (e.g., icons and help files). However, the main control program can be installed anywhere in the file directory tree. The install directory is selected by setting the INSTALL_DIR macro in the makefile. To set the INSTALL_DIR macro, edit the makefile and find the line: 

```INSTALL_DIR = .``` 

Replace the dot with the selected install directory name. For example, to install in /usr/local, change the line to:

```INSTALL_DIR = /usr/local```

This puts the main control program (called unravel) in the directory /usr/local/bin and adds an unravel man page to /usr/local/man/man1. Note that the bin and man/man1 are added automatically to the INSTALL_DIR directory name.

3.3. Running make

```make```

This should compile each unravel source program. Depending on the compiler used and how it is installed, there may be lots of warning messages (especially if using an ANSI C compiler). If everything has compiled correctly, unravel can be tried out on the simple programs in the directory examples. The file flavors.c has several obvious slices and makes a good trial run.

To run the install script, type: 

```make install``` 

(not yet tested on Ubuntu). This copies the unravel program file and man page to the install subdirectories.

4. Help

For more information and very very limited help contact:
Dr. James R. Lyle at NIST email: jlyle <_at_> nist <_dot_> gov 
