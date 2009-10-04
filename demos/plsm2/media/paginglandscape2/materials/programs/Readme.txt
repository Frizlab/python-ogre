Author: Clay Larabie, June 22, 2006

You're probably wondering what all these shaders do?

I'm currently only documenting the shaders that I've added recently to the paging landscape system, which are currently the only ones prefixed with "PL".

The various shaders are used for different types of lights (directional and point lights implemented so far, spotlights not yet implemented) as well as vertex programs that support compressed terrain, ambient passes, and unlit terrain. 

As far as naming conventions go, I think it's important that shaders that are used by the paging landscape scene manager are prefixed with something so that they can be easily identified when debugging, as well as keeping separate from application specific shaders and materials. In this case I adopted PL for Paging Landscape because these shaders specifically have to do with landscape. The other conventions used are - the postfix "2" denotes a shader that was written with Shader Model 2.0 support required, or has not been tested with earlier shader models. Since I expect most if not all of the shaders will need to be rewritten if you want them to work with earlier shader models, this naming convention was in anticipation of that fact. 

The naming convention is as follows:

Vertex programs:
PL + [Decompress if performs decompression] + [Light Type or Ambient or Unlit]  + VP + [Shader Model].cg

Fragment programs:
PL + [Splat if performs splatting] + [LightType] + FP + [Shader Model] + [Lighting quality if provided in multiple qualities such as Med or High] .cg

