import ogre_settings

from pygccxml import declarations

def shared_ptr (mb):
    """A method which attempts to appropriately export the shared pointer classes.
    """
    ogre_ns = mb.namespace ('Ogre')

    file_template = """
#ifndef CUSTOM_SHARED_PTR_HPP
#define CUSTOM_SHARED_PTR_HPP
%(headers)s
namespace boost { namespace python {
%(code)s
}}// namespace boost::python converter
#endif//CUSTOM_SHARED_PTR_HPP
"""

    per_class_code = """
        inline %(class_name)s* get_pointer( %(class_ptr_name)s const& p ){
            return p.get();
        }

        template <> 
        struct pointee< %(class_ptr_name)s >{
            typedef %(class_name)s type;
        };
    """
    ogre_shared_ptr_code = """
        template<class T>  T * get_pointer(Ogre::SharedPtr<T> const& p);
        template<class T> 
        inline T * get_pointer(Ogre::SharedPtr<T> const& p){
            return p.get();
        }
        
        template <class T> struct pointee< Ogre::SharedPtr<T> >{
            typedef T type;
        };
    """

    shared_ptr_code_list = []

    for shared_ptr_derived in ogre_ns.classes():
        if len(shared_ptr_derived.bases) != 1 or not shared_ptr_derived.bases[0].related_class.name.startswith ('SharedPtr'):
            continue
        shared_ptr_derived.exclude()
        shared_ptr_instantiation = shared_ptr_derived.bases[0].related_class

        # Now, set the held types appropriately,
        pointee  = declarations.remove_pointer (shared_ptr_instantiation.variable ('pRep').type)
        pointee.declaration.held_type = shared_ptr_derived.decl_string
        print " -- Adding held type for class:", pointee.decl_string, "of type:", shared_ptr_derived.decl_string

        # Generate pointee<> and get_pointer specialization.
        name_dict = {
            'namespace_name': 'Ogre',
            'class_name': pointee.decl_string,
            'class_ptr_name': shared_ptr_derived.decl_string,
        }
        shared_ptr_code_list.append (per_class_code % name_dict)

    shared_ptr_code_list.append (ogre_shared_ptr_code)
    headers = "\n".join (['#include "%s"' % header for header in ogre_settings.ogre_header_list])
    final_code_header = file_template % {
            'code': "\n".join (shared_ptr_code_list),
            'headers': headers,
    }

    fd = open (ogre_settings.build_dir + "/CustomSharedPtr.hpp", "w")
    fd.write (final_code_header)
    fd.close()


def generate_shared_ptr_typedefs (mb):
    print "Setting held_type for SharedPtr typedefs ..."
    print "-" * 80
    ogre_ns = mb.namespace ('Ogre')
    for ogre_typedef in ogre_ns.typedefs():
        if not hasattr(ogre_typedef.type,"declaration"):
            continue
        typedef_declaration = ogre_typedef.type.declaration
        if not typedef_declaration.name.startswith ('SharedPtr'):
            continue


        print " -- Setting held type of:", typedef_declaration.decl_string

        # Now we have a SharePtr instantiation
        shared_ptr_instantiation = typedef_declaration

        # For some reason some of these don't have the variable attribute
        if not hasattr (shared_ptr_instantiation, "variable"):
            print " Not Exporting ... trying to find out why:",
            if hasattr (shared_ptr_instantiation, "why_not_exportable"):
                print " -- NotExportable:", shared_ptr_instantiation
            print ""
            continue

        pointee  = declarations.remove_pointer (shared_ptr_instantiation.variable ('pRep').type)
        pointee.declaration.held_type = shared_ptr_instantiation.decl_string

        print " -- for:", pointee.decl_string, " -- to",
        print pointee.declaration.held_type


def create (mb):
    shared_ptr (mb)                    
    generate_shared_ptr_typedefs (mb)                    

# vim:et:ts=4:sts=4:sw=4
