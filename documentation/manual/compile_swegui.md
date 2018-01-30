# Compilation {#compilepageswegui}

### Required packages:
 - `g++`
 - `scons`
 -  `GTK3`
 -  `GTKmm (libgtkmm-3.0-dev)`
 -  `openGL (mesa-common-dev, freeglut3-dev)`
 -  `SFML (libsfml-dev)`
 -  `NetCdf (libnetcdf-dev)`

 - Additional compilers depending on your SWE configuration

Compile with `scons buildVariablesFile=build/options/SWE_gnu.py` or `scons buildVariablesFile=build/options/SWE_gnu.py compileMode=debug` for debug symbol support to `build/SWE_gnu_debug_none_fwave`.

Run this command in the root directory of the framework repository. There are several build variable files provided in `build/options`.
thispagestyle