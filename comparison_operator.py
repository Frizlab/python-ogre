import ogre_settings
from pygccxml import declarations

def create (mb):
    """A method which attempts to appropriately export the shared pointer classes.
    """
    ogre_ns = mb.namespace ('Ogre')

    file_template = """
#ifndef COMPARISON_OPERATOR_HPP
#define COMPARISON_OPERATOR_HPP
%(headers)s
namespace Ogre {
%(code)s
}//end namespace Ogre
#endif//COMPARISON_OPERATOR_HPP
"""

    per_class_code = """
        inline bool operator==(%(type)s const &lhs, %(type)s const &rhs) {
            throw std::runtime_error("%(type)s has no comparison operator, using contains has no meaning.");
        }
    """
    
    classes_needing_operators = [
        'TargetOperation',
        'Edge',
        'EdgeGroup',
        'HardwareAnimationData',
        'ParameterDef',
        'PlaneBoundedVolume',
        'PoseRef',
        'VertexAnimationTrack',
        'SubMeshLodGeometryLink',
        '_ConfigOption',
        'RaySceneQueryResultEntry',
    ]
    
    output = []
    for class_name in classes_needing_operators:
        class_ = ogre_ns.class_(class_name)
        output.append (per_class_code % dict(type=class_.decl_string))

    fd = open (ogre_settings.build_dir + "/ComparisonOperators.hpp", "w")
    headers = "\n".join (['#include "%s"' % header for header in ogre_settings.ogre_header_list])
    fd.write (file_template % \
        dict(headers=headers
              ,code="\n".join(output)))
    fd.close ()


# vim:et:ts=4:sts=4:sw=4
