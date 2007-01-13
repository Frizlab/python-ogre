##
##      ===   SConscript file format   ===
## Import('OUTNAME FILES local_env')
## _Ogre_ = local_env.SharedLibrary( OUTNAME, SHLIBPREFIX='', source=FILES)
##
##


# SharedLibrary('foo', ['f1.c', 'f2.c', 'f3.c'])

# LIBS=['foo'], LIBPATH='.'

# Program('prog.c', LIBS = 'm',
#                         LIBPATH = ['/usr/lib', '/usr/local/lib'])
#                         
# Can pass the object it's self so more portable...                        
# hello_list = Object('hello.c', CCFLAGS='-DHELLO')
#       goodbye_list = Object('goodbye.c', CCFLAGS='-DGOODBYE')
#       Program(hello_list + goodbye_list)
#       
# Program('hello.c')
#         SourceSignatures('MD5')  ## note default scons uses md5 hashes to detect changes - so touching a file doesn't rebuild
#         SourceSignatures('timestamp')  ## unless you do this...
#         
# ## here we are specifing specific include directories - otherwise scons won't kno
#         Program('hello.c', CPPPATH = ['include', '/home/project/inc'])
# ## incase scons doens't correctly identify dependancies 
# hello = Program('hello.c')
#        Depends(hello, 'other_file')
#        
# ##  Use environemtns (construction envrionments) to cope with multiple versions/platforms
# opt = Environment(CCFLAGS = '-O2')
#       dbg = Environment(CCFLAGS = '-g')

#       o = opt.Object('foo-opt', 'foo.c')
#       opt.Program(o)

#       d = dbg.Object('foo-dbg', 'foo.c')
#       dbg.Program(d)
# ## scons uses it's own default environment and you need to specifically set it
# import os
#       env = Environment(ENV = os.environ)
# ## need to tell it what to build by default
#  env = Environment()
#        hello = env.Program('hello.c')
#        env.Program('goodbye.c')
#        Default(hello)
# ## reading options from a command file
# opts = Options('custom.py')   ## 'RELEASE = 1'
# opts.Add('RELEASE', 'Set to 1 to build for release', 0)
# env = Environment(options = opts,
#                CPPDEFINES={'RELEASE_BUILD' : '${RELEASE}'})
# env.Program(['foo.c', 'bar.c'])
# Help(opts.GenerateHelpText(env))

# ## PATH STUFF
# opts = Options('custom.py')
#            opts.Add(PathOption('DBDIR',
#                                'Path to database directory',
#                                '/var/my_dbdir',
#                                PathOption.PathIsDir))
#            env = Environment(options = opts,
#                              CPPDEFINES={'DBDIR' : '"$DBDIR"'})
#            env.Program('foo.c')
#            
#  PathIsDirCreate will create it if it doesn't exist...
#  
#  opts = Options()
#         opts.AddOptions(
#             ('RELEASE', 'Set to 1 to build for release', 0),
#             ('CONFIG', 'Configuration file', '/etc/my_config'),
#             BoolOption('warnings', 'compilation with -Wall and similiar', 1),
#             EnumOption('debug', 'debug output and symbols', 'no',
#                        allowed_values=('yes', 'no', 'full'),
#                        map={}, ignorecase=0),  # case sensitive
#             ListOption('shared',
#                        'libraries to build as shared libraries',
#                        'all',
#                        names = list_of_libs),
#             PackageOption('x11',
#                           'use X11 installed here (yes = search some places)',
#                           'yes'),
#             PathOption('qtdir', 'where the root of Qt is installed', qtdir),
#         )

# ## and it will install/copy the file as required
# env = Environment()
#        hello = env.Program('hello.c')
#        goodbye = env.Program('goodbye.c')
#        env.Install('/usr/bin', hello)
#        env.Install('/usr/bin', goodbye)
#        env.Alias('install', '/usr/bin')

# ## opt leve scon file that calls build files in each directory
#  SConscript(['prog1/SConscript',
#                   'prog2/SConscript'])

#   in each subdir have :
#   env = Environment()
#       env.Program('prog1', ['main.c', 'foo1.c', 'foo2.c'])
#       
# ## here we specific the build dir, and tell scons NOT to copy the source to the build dir      
# SConscript('src/SConscript', build_dir='build', duplicate=0)
# ##
# ## this allows use to seperate the directories...      
# BuildDir('build', 'src', duplicate=0)
#       env = Environment()
#       env.Program('build/hello.c')
#       
# ## scon
# BuildDir('build', 'src')
#       SConscript('build/SConscript')
#       
# ######## platforms ##########      
# platform = ARGUMENTS.get('OS', Platform())

#     include = "#export/$PLATFORM/include"
#     lib = "#export/$PLATFORM/lib"
#     bin = "#export/$PLATFORM/bin"

#     env = Environment(PLATFORM = platform,
#                       BINDIR = bin,
#                       INCDIR = include,
#                       LIBDIR = lib,
#                       CPPPATH = [include],
#                       LIBPATH = [lib],
#                       LIBS = 'world')

#     Export('env')

#     env.SConscript('src/SConscript', build_dir='build/$PLATFORM')

#     
#     
#     
#         
#       


# cl /Zm800 -nologo
# "C:\Development\boost\libs\pythonogre\ogre_CVS\SetOfStrings.pypp.cpp" -Fo"bin\ogre_cvs\msvc-7.1\release\threading-multi\SetOfStrings.pypp.obj"   -TP /O2 /Ob2 /W3 /GR /MD /Zc:forScope /Zc:wchar_t /Ogiy /Gs /Ot /GB /Op /wd4675 /EHs -c 
# -DBOOST_PYTHON_DYNAMIC_LIB 
# -DBOOST_PYTHON_MAX_ARITY=19 
# -DNDEBUG 
# "-I..\.." 
# "-Ic:\development\ocvs\ogrenew\ogremain\include" 
# "-Ic:\python25\include" 
# "-Iogre_CVS"

#! /usr/bin/python
# Copyright 2006 Lakin Wecker
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)


##################################################################################################33

##
## This file should be located in the root dir
_LOGGING_ON = True

def log ( instring ):
    if _LOGGING_ON:
        print instring

import os
import sys

import environment
tobuild = ['ogre']
builddir = "build_dir"
bindir = "bin"
_building=[]
 
def create_SConscript ( cls ):
    try:
        os.makedirs ( cls._build_dir )
    except:
        pass
    f = open ( os.path.join( cls._build_dir, 'SConscript') , 'w' )
    f.write (  'Import(\'_env\') \n\
_%s_ = _env.SharedLibrary( "_%s_", SHLIBPREFIX=\'\', source=_env["FILES"]) ' % ( cls._name, cls._name )
         )
                                   
def get_ccflags():
    if os.name=='nt':
        CCFLAGS=''
        #CCFLAGS += '-DBOOST_PYTHON_MAX_ARITY=19'
        CCFLAGS += ' /Zm800 -nologo'
        CCFLAGS += ' /W3' # warning level
        CCFLAGS += ' /TP /O2 /Ob2  /GR /MD /Zc:forScope /Zc:wchar_t /Ogiy /Gs /Ot /GB /Op /wd4675 /EHs -c'
    else:
        CCFLAGS = ' `pkg-config --cflags OGRE` '
        CCFLAGS += ' -I' + ogre_settings.python_headers_dir + ' -D"BOOST_PYTHON_MAX_ARITY=19"'
        CCFLAGS += ' -O0 -g -I./'
    return CCFLAGS

def get_source_files(_dir):
    source_files = filter( lambda s: s.endswith( '.cpp' ), os.listdir(_dir) )
    source_files.sort()
    return map( lambda fname: os.path.join( _dir, fname ), source_files )

def get_linkflags():
    if os.name=='nt':
        LINKFLAGS = " /NOLOGO /INCREMENTAL:NO /DLL /OPT:NOREF,NOICF /subsystem:console "
    else:
        LINKFLAGS = ' `pkg-config --libs OGRE` '
        LINKFLAGS += ' -lboost_python'
    return LINKFLAGS

    
for name, cls in environment.projects.items():
    if name in tobuild:
        log ("Building " + name)
        _building.append( cls )
        cls._source = cls.generated_dir
        cls._build_dir = os.path.join ( environment.root_dir, builddir, name + '_' + str(cls.version))
        cls._bin_dir = os.path.join ( environment.root_dir, bindir)
        cls._name = name
        _env = Environment(ENV=os.environ)
        _env.Append ( LIBPATH= cls.lib_dirs + [environment.python_lib_dirs, environment.PATH_LIB_Boost] )
        _env.Append( LINKFLAGS=get_linkflags() + cls.LINKFLAGS )
        _env.Append ( LIBS = cls.libs )
        _env.Append ( CPPPATH = cls.include_dirs + [environment.python_include_dirs, environment.PATH_Boost]  )
        _env.Append ( CCFLAGS=get_ccflags() + cls.CCFLAGS )
        _env.Append ( FILES= get_source_files(cls._source) )
        create_SConscript ( cls )
        BuildDir(cls._build_dir, name + '_' + str(cls.version), duplicate=0)
        Export ( '_env' ) 
        SConscript(os.path.join( cls._build_dir, 'Sconscript' ) ) 

