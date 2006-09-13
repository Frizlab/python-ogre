import os
import environment
from pygccxml import declarations

OGRE_SP_HELD_TYPE_TMPL = \
"""
namespace boost{ namespace python{

%(class_name)s* get_pointer( %(class_ptr_name)s const& p ){
    return p.get();
}

template <>
struct pointee< %(class_ptr_name)s >{
    typedef %(class_name)s type;
};

}}// namespace boost::python
"""

REGISTER_SP_TO_PYTHON = \
"""
boost::python::register_ptr_to_python< %(sp_inst_class_name)s >();
"""

REGISTER_SPTR_CONVERSION =\
"""
boost::python::implicitly_convertible< %(derived)s, %(base)s >();
"""

class ogre_shared_ptr_t:
    def __init__( self, mb ):
        self.ogre_ns = mb.namespace ('Ogre')
        self.visited_classes = set()

    def get_pointee( self, sp_instantiation ):
        #sp_instantiation - reference to SharedPtr<XXX>
        #returns reference to XXX type/declaration
        no_ptr = declarations.remove_pointer( sp_instantiation.variable ('pRep').type )
        no_alias = declarations.remove_alias( no_ptr )
        return declarations.remove_declarated( no_alias )

    def configure_base_and_derived( self, sp_derived ):
        sp_instantiation = sp_derived.bases[0].related_class
        sp_derived.exclude()
        sp_instantiation.exclude()
        pointee = self.get_pointee( sp_instantiation )

        if pointee.held_type:
            return

        if pointee.is_abstract:
            pointee.held_type = '::Ogre::SharedPtr< %s >' % pointee.wrapper_alias

            sp_held_type_code = [ "struct %s;" % pointee.wrapper_alias ]
            sp_held_type_code.append(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': '::' + pointee.wrapper_alias
                                           , 'class_ptr_name': pointee.held_type } )

            sp_held_type_code.append(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': pointee.decl_string
                                           , 'class_ptr_name': sp_instantiation.decl_string } )

            pointee.add_declaration_code( os.linesep.join(sp_held_type_code) )

            pointee.add_registration_code(
                REGISTER_SP_TO_PYTHON % { 'sp_inst_class_name' : sp_instantiation.decl_string }
                , works_on_instance=False)

            pointee.add_registration_code(
                REGISTER_SPTR_CONVERSION % { 'derived' : pointee.held_type
                                             , 'base' : sp_instantiation.decl_string }
                , works_on_instance=False)

        else:
            pointee.held_type = sp_derived.decl_string

            pointee.add_declaration_code(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': pointee.decl_string
                                           , 'class_ptr_name': sp_derived.decl_string } )

        pointee.add_registration_code(
            REGISTER_SPTR_CONVERSION % { 'derived' : sp_derived.decl_string
                                         , 'base' : sp_instantiation.decl_string }
            , works_on_instance=False)

    def configure_instantiation( self, sp_instantiation ):
        sp_instantiation.exclude()
        pointee = self.get_pointee( sp_instantiation )
        if pointee.held_type:
            return

        if pointee.is_abstract:
            pointee.held_type = '::Ogre::SharedPtr< %s >' % pointee.wrapper_alias

            sp_held_type_code = [ "struct %s;" % pointee.wrapper_alias ]
            sp_held_type_code.append(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': '::' + pointee.wrapper_alias
                                           , 'class_ptr_name': pointee.held_type } )

            sp_held_type_code.append(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': pointee.decl_string
                                           , 'class_ptr_name': sp_instantiation.decl_string } )

            pointee.add_declaration_code( os.linesep.join(sp_held_type_code) )

            pointee.add_registration_code(
                REGISTER_SP_TO_PYTHON % { 'sp_inst_class_name' : sp_instantiation.decl_string }
                , works_on_instance=False)

            pointee.add_registration_code(
                REGISTER_SPTR_CONVERSION % { 'derived' : pointee.held_type
                                             , 'base' : sp_instantiation.decl_string }
                , works_on_instance=False)

        else:
            pointee.held_type = sp_instantiation.decl_string

            pointee.add_declaration_code(
                OGRE_SP_HELD_TYPE_TMPL % { 'class_name': pointee.decl_string
                                           , 'class_ptr_name': sp_instantiation.decl_string } )

        for hierarchy_info in pointee.bases:
            if hierarchy_info.access_type != 'public':
                continue
            pointee.add_registration_code(
                REGISTER_SPTR_CONVERSION % { 'derived' : sp_instantiation.decl_string
                                             , 'base' : '::Ogre::SharedPtr< %s >' % hierarchy_info.related_class.decl_string }
                , works_on_instance=False)

    def configure(self):
        """
        A method which attempts to appropriately export the shared pointer derived classes.
        Ogre uses next pattern:
        template class<T> SharedPtr{...};
        class XXXPtr : public SharedPtr<XXX> {... }
        We have to exclude ResourcePtr and to register Smart Pointer conversion
        See http://boost.org/libs/python/doc/v2/register_ptr_to_python.html
        """
        for cls in self.ogre_ns.classes():
            if 1 == len(cls.bases) and cls.bases[0].related_class.name.startswith ('SharedPtr'):
                self.configure_base_and_derived( cls )
        for cls in self.ogre_ns.classes():
            if cls.name.startswith( 'SharedPtr' ):
               self.configure_instantiation( cls )

def configure( mb ):
    ogre_shared_ptr_t( mb ).configure()
# vim:et:ts=4:sts=4:sw=4
