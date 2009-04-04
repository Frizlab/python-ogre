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
 
if os.name=='nt': 
    builddir = "c:/temp/build_dir" + "_" + environment.PythonVersionString
else:
    builddir = "build_dir" + "_" + environment.PythonVersionString
 
def create_SConscript ( cls ):
    fname = os.path.join( cls._source, 'SConscript')
    if os.path.isfile ( fname ):
        log ("WARNING: Over-Writing  %s as it already exists" % (fname)) 
        ## return ## uncomment this if you have manually created a Sconscript file..
    
    f = open (  fname , 'w' )
    if os.sys.platform <> 'darwin':
        targettype = 'SharedLibrary'
    else:
        targettype = 'LoadableModule'
    f.write ("""
Import(\'_env\') \n
_%(name)s = _env.%(targettype)s( "%(name)s", SHLIBPREFIX=\'\', source=_env["FILES"])\n
Return ('_%(name)s')\n
 """ % { 'name':cls._name, 'targettype':targettype } )
                                   
def get_ccflags(cls):
    if os.name=='nt':
        CCFLAGS=''
        CCFLAGS += ' /DBOOST_PYTHON_MAX_ARITY=19 /DBOOST_PYTHON_NO_PY_SIGNATURES ' 
        CCFLAGS += '  /nologo -Zm200 '
        CCFLAGS += ' /W3 /wd4675' # warning level  -Zc:wchar_t 
        CCFLAGS += ' /TP /MD /Zc:forScope /EHs /c  /Ob2 /Oi /O2 /Ot /Oy /GS- /GR ' #/Ox
    elif os.name =='posix':
        if os.sys.platform <> 'darwin':
            CCFLAGS = ' `pkg-config --cflags OGRE` '        ## needs to change I think :)
            CCFLAGS += ' -I' 
            CCFLAGS += ' -O3 -I./ '
            CCFLAGS += ' -DBOOST_PYTHON_MAX_ARITY=19 -DBOOST_PYTHON_NO_PY_SIGNATURES ' 
            if cls.ModuleName == 'OGRE':
                ##### -fvisibility=hidden -finline-limit=20 '
                #CCFLAGS += ' -fvisibility=hidden -fvisibility-inlines-hidden -DOGRE_GCC_VISIBILITY '
                CCFLAGS += ' ' 
            
        else:
            CCFLAGS = ' -I./ -O3 -dynamic -fPIC -DBOOST_PYTHON_NO_PY_SIGNATURES -DBOOST_PYTHON_MAX_ARITY=19   '
            CCFLAGS += ' -ftemplate-depth-128 -finline-functions -Wno-inline -Wall -no-cpp-precomp -gdwarf-2 -DNDEBUG '
            CCFLAGS += ' -include strings.h -include Carbon/Carbon.h '
            # -ftemplate-depth-128 -D_POSIX_C_SOURCE -no-cpp-precomp -gdwarf-2 -finline-functions -Wno-inline -Wall -DBOOST_ALL_NO_LIB=1 -DBOOST_PYTHON_SOURCE 
            # -I/Developer/SDKs/MacOSX10.5.sdk/usr/include/
            # -I/Developer/SDKs/MacOSX10.5.sdk/usr/include/             CCFLAGS  = ' -I -pipe -O3 -I./'
            
    ## change to ensure the source file is also in the include path due to indexing suite changes            
    CCFLAGS += ' -I' +cls._source + ' '            
    
    return CCFLAGS

def get_source_files(cls, _dir, usepch = False):

    def filterfunc ( filein ):
        validext = ['.cpp', '.cxx', '.c']
        if usepch:
            if filein == cls.pchbuild:
                return False
        for e in validext:
            if filein.endswith ( e ):
                return True
        return False
        
    try:
        source_files = filter( filterfunc , os.listdir(_dir) )
    except OSError,e:
        print 'WARNING: Generate the sources in this directory: "%s"' % _dir
        raise e
    source_files.sort()
    return source_files ## "Image.pypp.cpp" ##source_files

def get_linkflags():
    if environment.isWindows():
        #LINKFLAGS = " /NOLOGO /INCREMENTAL:NO /DLL /OPT:NOREF /OPT:NOICF /OPT:NOWIN98 /subsystem:console " # no change
        LINKFLAGS = " /MAP /MAPINFO:EXPORTS /NOLOGO /OPT:REF /INCREMENTAL:NO /DLL /OPT:ICF /subsystem:console " # 7 minutes 25% smaller 16.6 Meg
        #LINKFLAGS = " /NOLOGO /INCREMENTAL:NO /DLL /subsystem:console " ### LONG Link , 80 minutes - 15.7 meg
    if environment.isLinux():
        if os.sys.platform <> 'darwin':
            LINKFLAGS = ' `pkg-config --libs OGRE` -lstdc++ '
    if environment.isMac():
#            LINKFLAGS = ' -Wl,-x -framework Python -framework Ogre -framework Carbon -F' + environment.Config.FRAMEWORK_DIR + ' '
            LINKFLAGS = ' -dynamiclib -dynamic -framework Ogre -framework Carbon -framework Python '
    return LINKFLAGS

def build_pch( cls, pchfile ):
    """ lets create a source file that we will use to create the PCH file from """
    fout = open ( pchfile, 'w' ) ## delete it if it's already there
    for i in cls.pchincludes:
        fout.write ( '#include "' + i + '"\n' )
    fout.write ( "\n" )
    fout.close()
  
# Let us select the projects to build

possible_projects = []
for name,cls in environment.projects.items():
    possible_projects.append ( name )
#  ['ogre' , 'ois', 'ogrerefapp', 'ogrenewt', 'cegui', 'ode',\
#     'ogreode', 'ogreal', 'quickgui', 'opcode', 'nxogre', 'bullet', 'physx', 'betagui','theora',\
#      'ogrevideoffmpeg', 'ogredshow', 'plib', 'ogrebulletc', 'ogrebulletd',
#      'ogreforests', 'et', 'navi', 'caelum', 'noise', 'watermesh' ]  # , 'raknet'
default_projects = []
for name,cls in environment.projects.items():
    if cls.active:
        default_projects.append ( name )
#default_projects =  possible_projects ## environment.projects
# ['ogre' , 'ois', 'ogrerefapp', 'ogrenewt', 'cegui', 'ode',\
#     'ogreode', 'ogreal',  'quickgui', 'opcode', 'nxogre', 'bullet', 'physx', 'betagui','theora',\
#      'ogrevideoffmpeg', 'ogredshow', 'plib',  'ogrebulletc', 'ogrebulletd',
#      'ogreforests', 'et', 'caelum' ]  # 'navi',

# This lets you call scons like: 'scons PROJECTS=ogre,cegui'
opts = Options('custom.py')
opts.Add(ListOption('PROJECTS', 'Project to build wrappers for',
                    default_projects, possible_projects ))
temp_env = Environment(options = opts)
tobuild = temp_env['PROJECTS']
del temp_env    

   
        
for name, cls in environment.projects.items():
    ##if name.active:
    if name in tobuild:
        log ("SCONS: Building " + name)
        
        ## setup some defaults etc
        cls._source = cls.generated_dir
        cls._build_dir = os.path.join ( builddir, cls.dir_name)
        cls._name = name
        
        ## Note the change -- bug in scons means you have to set env seperately
        _env = Environment(ENV = os.environ)
        
        if environment.rpath:
            _env.Append(RPATH=environment.rpath)
        
        # Stores signatures in a separate .sconsign file 
        # in each directory as the single one was getting huge
        _env.SConsignFile(os.path.join( cls.generated_dir, "sconsign") )
              
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
        ccflags= get_ccflags(cls)
        if hasattr( cls, 'CCFLAGS'):
            ccflags += cls.CCFLAGS
        _env.Append ( CCFLAGS=ccflags )
        ## Use any extra stuff passed in th environment
        if 'CCFLAGS' in os.environ:
            _env.Append( CCFLAGS=os.environ['CCFLAGS'] )
        
        if hasattr( cls, 'pchbuild' ) and hasattr( cls, 'pchstop' ):
            usepch = True
            _env['PCHSTOP']= cls.pchstop
            build_pch ( cls, os.path.join(cls._source, cls.pchbuild) )  ## we build the source pch file here
            _env['PCH']=_env.PCH( os.path.join(cls._source, cls.pchbuild) )[0]  
        else:
            usepch = False  

        ## create a list of source files to include
        sourcefiles = get_source_files(cls, cls._source, usepch)
        _env.Append ( FILES= sourcefiles )
        ##cls._env = _env.Copy()  ## NOT sure this is needed...
        
        ## build it to somewhere else
        _env.BuildDir(cls._build_dir, os.path.join( environment.generated_dir_name, cls.dir_name ), duplicate=0)
        ##cls._env.BuildDir(cls._build_dir, os.path.join( environment.generated_dir_name, cls.dir_name ), duplicate=0)
        
        ## create a dynamic Sconscript file in the source directory (only if it doesn't exist)
        create_SConscript ( cls )
# #         _env.LINKCOM  = [_env['LINKCOM'],\
# #                         'mt.exe -nologo -manifest %(name)s.manifest -outputresource:%(name)s;2' % {'name':cls._name} ]
        ## now call the SConscript file in the source directory
        Export ( '_env' ) 
        package = _env.SConscript(os.path.join( cls._build_dir, 'SConscript' ) )
        
        ## ugly hack - scons returns a list of targets from SharedLibrary - we have to choose the one we want
        index = 0  # this is the index into a list of targets - '0' should be the platform default
        if environment.isWindows():
            ## and lets have it install the output into the 'package_dir_name/ModuleName' dir and rename to the PydName
            _env.AddPostAction(package,\
                 Action ('mt.exe -nologo -manifest %(name)s.manifest -outputresource:%(name)s;2' % { 'name':package[index] } ) )
        if environment.isLinux() and "-g" not in _env["CCFLAGS"]:
            _env.AddPostAction(package,\
                 '-strip -g -S -d --strip-debug -s %(name)s' % { 'name':package[index] } )
        if environment.isMac():
            pass
            #_env.AddPostAction(package,\
            #     '-strip -s %(name)s' % { 'name':package[index] } )
                         
        _env.InstallAs(os.path.join(environment.package_dir_name, cls.parent,
                                    cls.ModuleName, cls.PydName), 
                                     package[index] )

