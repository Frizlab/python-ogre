import os,sys 
if sys.platform == 'win32': 
    os.environ['PATH'] += ';' + __file__[0] 

import ogre.renderer.OGRE as ogre 
import SampleFramework 
import math 

import ctypes


class TutorialApplication(SampleFramework.Application): 
    def _createScene(self): 
        sm = self.sceneManager 
        cam = self.camera
        rw = self.renderWindow
                
        # create head entity (left)
        self.headNode = sm.getRootSceneNode().createChildSceneNode('head1')
        self.headNode.position=-70,0,0
        self.entity = sm.createEntity('head', 'ogrehead.mesh')
        self.headNode.attachObject(self.entity)
        
        
        for r in self.root.getAvailableRenderers():
            print "Renderer: ",r.getName() 
            cap = r.getCapabilities()
            if cap: ## Note only the active renderer has capabilities
                print "Device Name: ", cap.DeviceName
                print "Driver Version/release: ", cap.DriverVersion.build, cap.DriverVersion.release
       
        fragmentProgramName = "OceanCg" #"OceanHLSL_GLSL"                
        mat = ogre.MaterialManager.getSingleton().getByName(fragmentProgramName) 
        
        # In case the material is not found 
        if (mat == None): 
            print "Material not found: ",fragmentProgramName 
            sys.exit()
        
        validkeys = [] 
        if (mat.getTechnique(0).getPass(0).hasFragmentProgram()): 
            print "\n\nHAS FRAGMENT"
            params = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
            if (params.hasNamedParameters()): 
                it = params.getConstantDefinitionIterator() 
#                 while it.hasMoreElements():
#                     key = it.peekNextKey()
#                     value = it.peekNextValue()
#                     print key, value
#                     index = value.physicalIndex 
#                     size = value.elementSize 
#                     p = ctypes.c_float * size 
#                     print p
#                     params._readRawConstants (index, size, ctypes.addressof(p) ) 
#                     print p
#                     print index, size
#                     it.moveNext()                   
                while it.hasMoreElements():
                    key = it.peekNextKey()
                    if not '[' in key: # seem to get double key's ??
                        value = it.peekNextValue() 
                        index = value.physicalIndex 
                        size = value.elementSize 
                       
                        if value.isFloat():
                            storageclass = ctypes.c_float * size
                            p =storageclass(1.1)
                            params._readRawConstantsFloat (index, size, ctypes.addressof(p) ) 
                        else:
                            storageclass = ctypes.c_int * size
                            p =storageclass(1)                            
                            params._readRawConstantsInt (index, size, ctypes.addressof(p) ) 
                        print key, index, size, p[0], p[1], p[2]
                    it.moveNext()
                print value
                print dir(value) 
                print p
                print dir(p)                       
                    
                                        
#                 while (True): 
#                     # Get the key 
#                     key = it.peekNextKey() 
#                     print "KEY: ",key 
#                     # And the corresponding value 
#                     value = it.peekNextValue() 
#                     index = value.physicalIndex 
#                     size = value.elementSize 
#                     p = ctypes.c_float * size 
#                     result = 0 
#                     print p
# #                     params._readRawConstants (index, size, ctypes.addressof(p) ) 
# #                     print "VALUE: ",p 
# #                     if (key.isalpha() and params._findNamedConstantDefinition(key)): 
# #                         validkeys.append(key) 
#                     it.moveNext() 
#                     if (not it.hasMoreElements()): 
#                         break 
        else:
            print "\n\nNo fragment programs found"                        
        sys.exit()

if __name__ == '__main__': 
    ta = TutorialApplication() 
    ta.go() 
