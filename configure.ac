AC_INIT([JsonBenchmark], [0.1], [YourName@Email.com])
AC_PREREQ([2.65])

# Set up the build system
if [[ ! -e build/Notes ]]; then
    git submodule init
    git submodule update
fi
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])
AX_THOR_FUNC_BUILD


AC_CONFIG_SRCDIR([src/benchmark/benchmark.cpp])
AC_PROG_CXX
AX_THOR_PROG_COV

AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["$CXXFLAGS -ansi"])])
AX_CHECK_COMPILE_FLAG([-Wno-literal-suffix], [AC_SUBST([LITERAL_WARNING_SUFFIX], [-Wno-literal-suffix])], [], [$CXX_STD_FLAG -Werror -Wall -Wextra])
AX_CHECK_COMPILE_FLAG([-Wno-literal-range],  [AC_SUBST([LITERAL_WARNING_RANGE],  [-Wno-literal-range])],  [], [$CXX_STD_FLAG -Werror -Wall -Wextra])
AC_LANG_POP([C++])

AX_THOR_FUNC_LANG_FLAG(17)

LT_INIT


#
# Add your defintions in here.
# Note there are some predefined macros in build/autotools/m4/
AC_DEFUN([AC_STRUCT_DIRENT_NAME_LEN], [_AC_STRUCT_DIRENT([d_namlen])])

AC_CHECK_PROGS([TestSCons], [scons], [:])
AS_IF(
    [test "$TestSCons" == ":"],
    [
        AC_MSG_ERROR([

The thirdparty library "fastjson" requires scons.
Please install via

    sudo apt-get install scons   # linux
    brew install scons           # mac

        ])
    ], []
)

# Add a test for sed -i
# Already works on Linux.
# But on Mac you need:

# brew install gnu-sed --with-default-names



AC_HEADER_DIRENT
AC_STRUCT_DIRENT_NAME_LEN


# Build all the Makefiles and configuration files.
# Used by ThorMaker
# Note: you can push the config file to sub directories in the AC_CONFIG_HEADERS macro (see example)
# Note: Local Make variables should be placed in Makefile.config.in
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([config.h src/benchmark/benchmarkConfig.h:config.h.in src/Memory/MemoryConfig.h:config.h.in])
AC_CONFIG_FILES([Makefile.extra Makefile.config:build/autotools/build/Makefile.config.in:Makefile.config.in])
AC_OUTPUT
