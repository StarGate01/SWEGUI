# This file tells scons how to compile SWEGUI

import os
import sys

# Set possible variables
vars = Variables()
# Read parameters from a file if given
vars.AddVariables(
  PathVariable('buildVariablesFile', 'location of the python file, which contains the build variables', None, PathVariable.PathIsFile)
)
env = Environment(variables=vars)
if 'buildVariablesFile' in env:
    vars = Variables(env['buildVariablesFile'])
# SWE specific variables
vars.AddVariables(
    PathVariable('buildDir', 'Where to build the code', 'build', PathVariable.PathIsDirCreate),
    EnumVariable('compileMode', 'Mode of the compilation', 'release',
        allowed_values=('debug', 'release')
    )
)
# External variables
vars.AddVariables(
    PathVariable('netCDFDir', 'location of netCDF', None)
)

# Set environment
env = Environment(ENV = {'PATH': os.environ['PATH']}, variables=vars, tools = ['default', 'cxxtest'])

# All required libraries
libs = ['gtkmm-3.0']

# Auto configuration
conf = Configure(env)
for x in libs:
	if not conf.CheckLib(x):
		print 'Missing library ' + x
env = conf.Finish()

# Add libs and their configs
env.Append(LIBS=libs)
for x in libs:
    env.ParseConfig('pkg-config --cflags --libs '+x)

# Add debug and release flags
if env['compileMode'] == 'debug':
    env.Append(CPPDEFINES=['DEBUG'])
    env.Append(CCFLAGS=['-O0','-g3','-Wall'])
elif env['compileMode'] == 'release':
    env.Append(CPPDEFINES=['NDEBUG'])
    env.Append(CCFLAGS=['-O3','-mtune=native'])

# Set names and paths
program_name = 'swegui'
build_dir = env['buildDir']+'/build_'+program_name

# Get the source files
env.src_files = []
Export('env')
SConscript('src/SConscript', variant_dir=build_dir, duplicate=0)
Import('env')

# Build binary
env.Program('build/'+program_name, env.src_files)

# Copy UI scripts
uis = ['basic']
for x in uis:
    Install('build/ui', 'src/ui/'+x+'.glade')
