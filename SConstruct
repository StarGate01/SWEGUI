# This file tells scons how to compile SWEGUI

import os
import sys
import re

# Set possible variables
vars = Variables()
# Read parameters from a file if given
vars.AddVariables(
  PathVariable('buildVariablesFile', 
    'location of the python file, which contains the build variables', 
    None, PathVariable.PathIsFile)
)
env = Environment(variables=vars)
if 'buildVariablesFile' in env:
    vars = Variables(env['buildVariablesFile'])

# Specific variables
vars.AddVariables(
    PathVariable('buildDir', 'Where to build the code', 'build', PathVariable.PathIsDirCreate),
    EnumVariable('compileMode', 'Mode of the compilation', 'release',
        allowed_values=('debug', 'release')
    )
)

# Set environment
env = Environment(ENV = {'PATH': os.environ['PATH']}, variables=vars, tools = ['default'])

# GTK resource builder
gtkresbld = Builder(action = 'glib-compile-resources --target=$TARGET --generate-source $SOURCE', 
    suffix = '.c', src_suffix = '.xml')
env.Append(BUILDERS = {'GtkResBld': gtkresbld })

# GTK resource dependency finder
include_gtkre = re.compile(r'<file.*>(.+)<\/file>', re.M)
def gtkres_scan(node, env, path):
    contents = node.get_contents()
    includes = include_gtkre.findall(contents)
    return ['#' + s for s in includes]

gtkresscan = Scanner(function = gtkres_scan, skeys = ['.xml'])
env.Append(SCANNERS = gtkresscan)

# All required libraries
libs = ['gtkmm-3.0', 'sfml-system', 'sfml-graphics', 'netcdf']

# Auto configuration
conf = Configure(env)
for x in libs:
    if not conf.CheckLib(x): print 'Missing library ' + x
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

# Set names
env.program_name = 'swegui'
env.library_name = 'sweguiwidgetsglade'

# Build binaries
env.src_files = []
Export('env')
SConscript('src/SConscript', variant_dir=env['buildDir']+'/build_'+env.program_name, duplicate=0)