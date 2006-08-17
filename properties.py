#!/usr/bin/env python

from pygccxml import declarations

from pyplusplus import code_creators
from pyplusplus import module_builder



#-------------------------------------------------------------------------------
class property_creator_t (code_creators.code_creator_t):
    '''A property creator which has support for generating readonly properties,
if no set_function is provided. As well as adding a policy to
reference_existing_object if the return type of the function is a reference or
pointer.'''

    def __init__( self, get_function, set_function ):
        #get/set functions are references to relevant declarations
        code_creators.code_creator_t.__init__ (self)
        self.__get_function = get_function and get_function.declaration
        self.__set_function = set_function and set_function.declaration

    def _create_template (self, function):
        '''Use this function to create the reference to the function with the
appropriate policy.'''
        if not function: return ''

        impl = '(%(type)s)( &%(function)s )' % dict (
                function = declarations.full_name (function),
                type = function.function_type ().decl_string)

        if declarations.is_pointer (function.return_type) \
            or declarations.is_reference (function.return_type):
            # OGRE's default is to return internal references
            impl = '''bp::make_function (%s, %s)''' \
                % (impl, function.call_policies.create (None))

        return ', %s' % impl


    def _create_impl(self):
        template = '''add_property ("%(name)s" %(properties)s)'''
        properties_template = self._create_template (self.__get_function)
        properties_template += self._create_template (self.__set_function)
        property_name = self.__get_function.name[3:]

        if property_name == '': return ''

        property_name = property_name[0].lower() + property_name[1:]
        return template % dict (name=property_name, properties=properties_template)

improper_getters = 0
improper_setters = 0



#-------------------------------------------------------------------------------
def create (mb):
    '''OGRE typically uses getX and setX for properties.
However, we have to be careful because there are methods names setX and getX
which are not true properties (in a pythonic sense), as sometimes a getX will
take one or more arguments, or the setX will take more than one argument.
TODO: there are probably exceptions to the naming scheme which should be found
and documented.'''

    # First, some helper functions
    def is_relevant (code_creator):
        '''Ensures that we're looking at methods that start with get/set and
are public member functions.'''
        if not isinstance (code_creator, code_creators.declaration_based_t):
            #We are looking for code creator, that has to export some declaration
            return False
        if not isinstance (code_creator.declaration, module_builder.member_function_t):
            #declaration should be member function
            return False
        decl = code_creator.declaration

        if decl.access_type != "public":
            # allow only public members.
            return False

        # Member name should also have something after the get/set
        if len(decl.name) == 3: return False

        #member function name should start from "get" or "set"
        return decl.name.startswith ('get') or decl.name.startswith ('set')

    def is_property (getter, setter):
        '''Checks that we have at least a getter function and that the form
of the functions is appropriate: getter has no arguments and returns the same
type of object as the type of the only argument to the setter function.'''
        global improper_setters, improper_getters

        # Ignore these, as set-only properties don't make of sense.
        if not getter: return False
        get_decl = getter.declaration

        if len (get_decl.arguments) > 0 or get_decl.return_type == 'void':
            # Make sure it has a non-void return type.
            improper_getters += 1
            return False

        # We're ok with read only properties
        if not setter: return True
        set_decl = setter.declaration

        if len(set_decl.arguments) != 1:
            # Setter doesn't have exaclty one argument
            improper_setters += 1
            return False

        if get_decl.return_type != set_decl.arguments[0].type:
            # Make sure their types match up.
            improper_setters += 1
            return False

        return True

    classes = filter (lambda creator: isinstance (creator, code_creators.class_t),
        code_creators.make_flatten(mb.code_creator))

    code_creator_dict = {}
    for class_creator in classes:
        code_creator_dict[class_creator.declaration.name] = class_creator

    for class_creator in classes:
        accessor_list = filter (is_relevant, class_creator.creators)
        # get only the relevant code_creators

        accessors_db = {} # name : {'get':get creator, 'set':set creator]

        #Filling accessors_db
        for creator in accessor_list:
            type = creator.declaration.name[:3]
            property_name = creator.declaration.name[3:]
            if not accessors_db.has_key (property_name):
                accessors_db [property_name] = {'set':None,'get':None}
            if accessors_db[property_name][type]:
                # If we already have this type of creator we will prefer
                # ones without a const, or the first one
                if hasattr (accessors_db[property_name][type].declaration,'has_const') and \
                    accessors_db[property_name][type].declaration.has_const:
                    print "SWITCHING:", creator.declaration.name, "instead of", accessors_db [property_name][type].declaration.name
                    accessors_db [property_name][type] = creator
            else:
                accessors_db [property_name][type] = creator



        def find_accessor_in_base (code_creator, name, type):
            '''Searches in the base classes for the accessor with the
right type and name.'''
            bases = code_creator.declaration.bases[:] # make a copy of direct bases

            while len(bases) > 0:
                base_class = bases.pop(0)
                if not code_creator_dict.has_key (base_class.related_class.name): continue
                accessor_list = filter (is_relevant, code_creator_dict[base_class.related_class.name].creators)

                for creator in accessor_list:
                    if not hasattr (creator,"declaration"): continue
                    foundType = creator.declaration.name[:3]
                    property_name = creator.declaration.name[3:]
                    if type == foundType and property_name == name:
                        print "Found :", type, name, "in", base_class.related_class.name
                        return creator

                # add indirect bases of this class.
                bases.extend (base_class.related_class.bases)
            return None

        for name,accessors in accessors_db.items():
            # If we're missing an accessor, try to find it in the parent class
            accessors['get'] = accessors['get'] or find_accessor_in_base (class_creator,name,'get')
            accessors['set'] = accessors['set'] or find_accessor_in_base (class_creator,name,'set')

            if is_property (accessors['get'], accessors['set']):
                prop_creator = property_creator_t (accessors['get'], accessors['set'])
                class_creator.adopt_creator (prop_creator)



    print "-"*80
    print "Improper Getters", improper_getters
    print "Improper Setters", improper_setters

# vim:et:ts=4:sts=4:sw=4
