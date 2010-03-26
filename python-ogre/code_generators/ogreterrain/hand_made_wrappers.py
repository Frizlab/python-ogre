import os
import environment
WRAPPER_DEFINITION_TerrainGroup =\
"""
boost::python::list
TerrainGroup_boxIntersects(::Ogre::TerrainGroup & me, Ogre::AxisAlignedBox& box)
{
    Ogre::TerrainGroup::TerrainList resultList;
    boost::python::list outlist;

    me.boxIntersects(box, &resultList);
    for (Ogre::TerrainGroup::TerrainList::iterator ti = resultList.begin();
	   ti != resultList.end(); ++ti)
       outlist.append ( *ti );
       
    return outlist;
}

boost::python::list
TerrainGroup_sphereIntersects(::Ogre::TerrainGroup & me, Ogre::Sphere& sphere)
{
    Ogre::TerrainGroup::TerrainList resultList;
    boost::python::list outlist;

    me.sphereIntersects(sphere, &resultList);
    for (Ogre::TerrainGroup::TerrainList::iterator ti = resultList.begin();
	   ti != resultList.end(); ++ti)
       outlist.append ( *ti );

    return outlist;
}
//		void convertWorldPositionToTerrainSlot(const Vector3& pos, long *x, long *y) const;

"""
WRAPPER_REGISTRATION_TerrainGroup = [
    """def( "boxIntersects", &::TerrainGroup_boxIntersects,\\
                "Python-Ogre Helper Function: Returns the list of intersects.\\n\\
                Input: box\\n\\
                Output: list of any intersects");""",
    """def( "sphereIntersects", &::TerrainGroup_sphereIntersects,\\
                "Python-Ogre Helper Function: Returns the list of intersects.\\n\\
                Input: sphere\\n\\
                Output: list of any intersects");""",
    ]

#################################################################################################
#################################################################################################

def iter_as_generator_vector( cls ):
    print "ITER:", cls.name

    try:
        code = os.linesep.join([
                'typedef %(cls)s iter_type;'
                , 'generators::generator_maker_vector< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
        cls.add_registration_code(
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED "

def iter_as_generator_map( cls ):
    print "ITER:", cls.name

    code = os.linesep.join([
            'typedef %(cls)s iter_type;'
            , 'generators::generator_maker_map< iter_type >::register_< %(call_policies)s >( %(exposer_name)s );'])
    try:
        cls.add_registration_code(
                code % { 'cls' : cls.decl_string
                         , 'call_policies' : cls.mem_fun( 'getNext' ).call_policies.create_type()
                         , 'exposer_name' : cls.class_var_name }
                , works_on_instance=False )
        cls.include_files.append( 'generators.h' )
        print "OK"
    except:
        print "FAILED"

#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'VectorIteratorWrapper<' ) )
    for cls in vec_iterators:
        iter_as_generator_vector( cls )

    map_iterators = mb.classes( lambda cls: cls.name.startswith( 'MapIteratorWrapper<' ) )
    for cls in map_iterators:
        iter_as_generator_map( cls )
        
    try:
        vec_iterators = mb.classes( lambda cls: cls.name.startswith( 'ConstVectorIteratorWrapper<' ) )
        for cls in vec_iterators:
            iter_as_generator_vector( cls )
    except:
        pass
    try:
        map_iterators = mb.classes( lambda cls: cls.name.startswith( 'ConstMapIteratorWrapper<' ) )
        for cls in map_iterators:
            iter_as_generator_map( cls )
    except:
        pass
        
    rt = mb.class_( 'TerrainGroup' )
    rt.add_declaration_code( WRAPPER_DEFINITION_TerrainGroup )
    apply_reg (rt,  WRAPPER_REGISTRATION_TerrainGroup )

