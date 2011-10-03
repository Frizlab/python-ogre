import os
import environment


WRAPPER_DEFINITION_TerrainInfo = \
"""
boost::python::list
TerrainInfo_rayIntersects(ET::TerrainInfo& ti, const Ogre::Ray& ray ) {
    boost::python::list outlist;
    std::pair<bool, Ogre::Vector3> res = ti.rayIntersects(ray);
    outlist.append(res.first);
    outlist.append(boost::python::make_tuple(res.second.x, res.second.y, res.second.z));
    return outlist;
}

"""
WRAPPER_REGISTRATION_TerrainInfo = [
        """def( "rayIntersects", &::TerrainInfo_rayIntersects,
         "Python-Ogre Hand Wrapped Function: Returns a list of intersection data.\\n\\
                Input: Ray\\n\\
                Output: List [Intersects, Tuple (intersection.x, intersection.y , intersection.z ) ]\'s");"""
                
    ]    

WRAPPER_DEFINITION_SplattingManager = \
"""
boost::python::list
SplattingManager_createColourMap(ET::SplattingManager& ti, Ogre::Image& image, boost::python::list listin ) {

    ET::ColourList mColourList;
    size_t size = len(listin);
    mColourList.reserve(size);
    mColourList.resize(size);

    for (int index=0;index<len(listin);index++ ) {
        mColourList[index] = boost::python::extract<Ogre::ColourValue>(listin[index]);
    }
    ti.createColourMap(image, mColourList);
    mColourList.clear();
}

void
SplattingManager_createBaseTexture(ET::SplattingManager& ti, Ogre::Image& image, size_t width, size_t height, boost::python::list textures,float repeatX, float repeatZ)
{
    ET::ImageList list_textures(len(textures));
    for (int index=0;index<len(textures);index++ ) {
        list_textures[index] = boost::python::extract<Ogre::Image>(textures[index]);
    }
    ti.createBaseTexture(image, width, height, list_textures,repeatX,repeatZ);
}


"""
WRAPPER_REGISTRATION_SplattingManager = [
        """def( "createColourMap", &::SplattingManager_createColourMap,
         "Python-Ogre Hand Wrapped Function: Creates a colour map.\\n\\
                Input: Ogre::Image,  List[Ogre.ColourValue] \\n\\
                Output: None\'s");"""
        """def( "createBaseTexture", &::SplattingManager_createBaseTexture,
         "Python-Ogre Hand Wrapped Function: Creates a base texture.\\n\\
                Input: Ogre::Image,  width,height, List[Ogre.Images],repeatX,repeatZ \\n\\
                Output: None\'s");"""
                
    ]    

##WRAPPER_DEFINITION_Brush = \
##"""
##ET::Brush Brush( boost::python::list listin, size_t width, size_t height )
##{
##    float* f = new float[width*height];
##    size_t index=0;
##    for (size_t w =0 ; index <width; w ++) {
##        for (size_t h =0 ; index <height; h ++) {
##            *f++ = boost::python::extract<float>(listin[index]);
##            index++;
##        }
##    }
##    //return me.setTextureCoords( start, count );
##    return ET::Brush(f, width, height);
##}

##""" 



##WRAPPER_REGISTRATION_Brush = [
##    """def( "Brush", &Brush,
##                "Python-Ogre Brush Helper Function: Reads floats from a brush array a python list.\\n\\
##                Input: x,y\\n\\
##                Ouput: Python List\\n\\
##                The list is populated with floats from the brush array starting at x,y" );"""
##     ]


#################################################################################################
#################################################################################################

def apply_reg ( class_, code ):
    for c in code:
        class_.add_registration_code ( c )
        
def apply( mb ):
    rt = mb.class_( 'TerrainInfo' )
    rt.add_declaration_code( WRAPPER_DEFINITION_TerrainInfo )
    apply_reg (rt,  WRAPPER_REGISTRATION_TerrainInfo )
    rt = mb.class_( 'SplattingManager' )
    rt.add_declaration_code( WRAPPER_DEFINITION_SplattingManager )
    apply_reg (rt,  WRAPPER_REGISTRATION_SplattingManager )
##    rt = mb.class_( 'Brush' )
##    rt.add_declaration_code( WRAPPER_DEFINITION_Brush )
##    apply_reg (rt,  WRAPPER_REGISTRATION_Brush )


