#! /usr/bin/python
# Copyright 2007 - Python-Ogre Project 
# Based upon code from Lakin Wecker (2006)
# 
##
## This file should be located in the root dir
##

## some more goes here

_LOGGING_ON = True

def log ( instring ):
    if _LOGGING_ON:
        print instring

import os
import sys
import environment

#
# list the modules here you want to build
# The names must match those in environment.projects
#
#tobuild = ['ogre' , 'ois', 'ogrerefapp', 'ogrenewt', 'cegui']
#tobuild = ['ode']
 
builddir = "build_dir"
 
def create_SConscript ( cls ):
    fname = os.path.join( cls._source, 'SConscript')
    if os.path.isfile ( fname ):
        log ("WARNING: Over-Writing  %s as it already exists" % (fname)) 
        ## return ## uncomment this if you have manually created a Sconscript file..
    
    f = open (  fname , 'w' )
    f.write (  """
Import(\'_env\') \n
_%s = _env.SharedLibrary( "%s", SHLIBPREFIX=\'\', source=_env["FILES"])\n
Return ('_%s') """ % ( cls._name, cls._name, cls._name ) )
    
                                   
def get_ccflags():
    if os.name=='nt':
        CCFLAGS=''
        #CCFLAGS += '-DBOOST_PYTHON_MAX_ARITY=19'
        CCFLAGS += ' -Zm800 -nologo'
        CCFLAGS += ' -W3' # warning level
        CCFLAGS += ' -TP -O2 -Ob2 -GR -MD -Zc:forScope -Zc:wchar_t -Ogiy -Gs -Ot -GB -Op -wd4675 -EHs -c'
    elif os.name =='posix':
        CCFLAGS = ' `pkg-config --cflags OGRE` '
        CCFLAGS += ' -I' 
        CCFLAGS += ' -O0 -g -I./'
    return CCFLAGS

def get_source_files(_dir):
    try:
        source_files = filter( lambda s: s.endswith( '.cpp' ), os.listdir(_dir) )
    except OSError,e:
        print 'WARNING: Generate the sources this directory: "%s"' % _dir
        raise e
    source_files.sort()
    return source_files

def get_linkflags():
    if os.name=='nt':
        #LINKFLAGS = " -NOLOGO -INCREMENTAL:NO -DLL -OPT:NOREF -subsystem:console " # no change
        LINKFLAGS = " -NOLOGO -INCREMENTAL:NO -DLL -OPT:NOICF -subsystem:console " # 7 minutes 25% smaller 16.6 Meg
        #LINKFLAGS = " -NOLOGO -INCREMENTAL:NO -DLL -subsystem:console " ### LONG Link , 80 minutes - 15.7 meg
    elif os.name == 'posix':
        LINKFLAGS = ' `pkg-config --libs OGRE` '
        #LINKFLAGS += ' -lboost_python'
    return LINKFLAGS

# Let us select the projects to build
possible_projects = ['ogre' , 'ois', 'ogrerefapp', 'ogrenewt', 'CEGUI']
default_projects = possible_projects

# This lets you call scons like: 'scons PROJECTS=ogre,cegui'
opts = Options('custom.py')
opts.Add(ListOption('PROJECTS', 'Project to build wrappers for',
                    possible_projects, default_projects))
temp_env = Environment(options = opts)
tobuild = temp_env['PROJECTS']
del temp_env    
    
        
for name, cls in environment.projects.items():
    ##if name.active:
    if name in tobuild:
        log ("Building " + name)
        
        ## setup some defaults etc
        cls._source = cls.generated_dir
        cls._build_dir = os.path.join ( builddir, cls.dir_name)
        cls._name = name
        _env = Environment(ENV=os.environ)

        ## Use custom compilier if wanted (things like ccache)
        if hasattr(environment, 'cxx_compiler'):
            _env['CXX'] = environment.cxx_compiler
        if hasattr(environment, 'cc_compiler'):
            _env['CC'] = environment.cc_compiler
           
        ## setup linker paths/libs and flags (standard and additional)
        _env.Append ( LIBPATH= cls.lib_dirs + [environment.python_lib_dirs] )
        linkflags=get_linkflags()
        if hasattr ( cls, 'LINKFLAGS' ):
            linkflags += cls.LINKFLAGS
        _env.Append( LINKFLAGS=linkflags )
        _env.Append ( LIBS = cls.libs )
        
        ## setup compile paths and flags (standard and additional)
        _env.Append ( CPPPATH = cls.include_dirs + [environment.python_include_dirs, environment.Config.PATH_Boost]  )
        ccflags= get_ccflags()
        if hasattr( cls, 'CCFLAGS'):
            ccflags += cls.CCFLAGS
        _env.Append ( CCFLAGS=ccflags )
        
        ## create a list of source files to include
        _env.Append ( FILES= get_source_files(cls._source) )
        ##cls._env = _env.Copy()  ## NOT sure this is needed...
        
        ## build it to somewhere else
        _env.BuildDir(cls._build_dir, os.path.join( environment.generated_dir_name, cls.dir_name ), duplicate=0)
        ##cls._env.BuildDir(cls._build_dir, os.path.join( environment.generated_dir_name, cls.dir_name ), duplicate=0)
        
        ## create a dynamic Sconscript file in the source directory (only if it doesn't exist)
        create_SConscript ( cls )
        
        ## now call the SConscript file in the source directory
        Export ( '_env' ) 
        package = _env.SConscript(os.path.join( cls._build_dir, 'SConscript' ) )
        
        ## and lets have it install the output into the 'package_dir_name/ModuleName' dir and rename to the PydName
        
        ## ugly hack - scons returns a list of targets from SharedLibrary - we have to choose the one we want
        index = 0  # this is the index into a list of targets - '0' should be the platform default
        _env.InstallAs(os.path.join(environment.package_dir_name,
                                    cls.ModuleName, cls.PydName), 
                                     package[index] )
