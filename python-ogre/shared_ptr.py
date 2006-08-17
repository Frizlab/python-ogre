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

}}// namespace boost::python converter
"""

REGISTER_SP_TO_PYTHON = \
"""
boost::python::register_ptr_to_python< %(sp_inst_class_name)s >();
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

        if pointee in self.visited_classes:
            return
        self.visited_classes.add( pointee )

        pointee.add_declaration_code(
            OGRE_SP_HELD_TYPE_TMPL % {
                'class_name': pointee.decl_string
                , 'class_ptr_name': sp_derived.decl_string } )

        if pointee.is_abstract:
            #For some reason Boost.Python does not support HeldType on abstract
            #classes
            pointee.add_registration_code(
                REGISTER_SP_TO_PYTHON % dict( sp_inst_class_name=sp_derived.decl_string )
                , works_on_instance=False)
        else:
            pointee.held_type = sp_derived.decl_string

    def configure_instantiation( self, sp_instantiation ):
        pointee = self.get_pointee( sp_instantiation )
        if pointee in self.visited_classes:
            return
        self.visited_classes.add( pointee )

        #In this case, we can create single template that will tread this use case.
        #But, thus we will increase compilation time. I think it is much better to
        #generate non template version.
        pointee.add_declaration_code(
            OGRE_SP_HELD_TYPE_TMPL % {
                'class_name': pointee.decl_string
                , 'class_ptr_name': sp_instantiation.decl_string } )

        if pointee.is_abstract:
            #For some reason Boost.Python does not support HeldType on abstract
            #classes
            pointee.add_registration_code(
                REGISTER_SP_TO_PYTHON % dict( sp_inst_class_name=sp_instantiation.decl_string )
                , works_on_instance=False)
        else:
            pointee.held_type = sp_instantiation.decl_string

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
            elif cls.name.startswith( 'SharedPtr' ):
               self.configure_instantiation( cls )
            else:
                pass


def configure( mb ):
    ogre_shared_ptr_t( mb ).configure()
# vim:et:ts=4:sts=4:sw=4
