AC_INIT([JsonBenchmark], [0.1], [Loki.Astari+ThorsAnvil@gmail.com])
AC_PREREQ([2.65])

# Set up the build system
if [[ ! -e build/Notes ]]; then
    git submodule init
    git submodule update
fi
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])

export enable_vera='no'
AX_THOR_FUNC_BUILD_FIX_GIT_SYMLINKS_WINDOWS
AX_THOR_FUNC_INIT_BUILD([JsonBenchmark], [], [23])

AC_SUBST([VERATOOL], [off])
# AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["$CXXFLAGS -ansi"])])

LT_INIT

AX_THOR_FEATURE_HEADER_ONLY_VARIANT([THORS_JSONBENCHMARK])
AX_THOR_FUNC_TERM_BUILD([JSONBENCHMARK], [src/benchmark/benchmarkConfig.h:config.h.in src/Memory/MemoryConfig.h:config.h.in])

AC_OUTPUT
