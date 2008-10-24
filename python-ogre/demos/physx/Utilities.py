import ogre.physics.physx as physx

def getNxSDKCreateError(errorCode):
    """ convert an errorCode to a useful string message
    """
    if errorCode == physx.NXCE_NO_ERROR: return "NXCE_NO_ERROR" 
    if errorCode == physx.NXCE_PHYSX_NOT_FOUND: return "NXCE_PHYSX_NOT_FOUND" 
    if errorCode == physx.NXCE_WRONG_VERSION: return "NXCE_WRONG_VERSION" 
    if errorCode == physx.NXCE_DESCRIPTOR_INVALID: return "NXCE_DESCRIPTOR_INVALID" 
    if errorCode == physx.NXCE_CONNECTION_ERROR: return "NXCE_CONNECTION_ERROR" 
    if errorCode == physx.NXCE_RESET_ERROR: return "NXCE_RESET_ERROR" 
    if errorCode == physx.NXCE_IN_USE_ERROR: return "NXCE_IN_USE_ERROR" 
    return "Unknown error" 

def RemoveFileFromPath(path):
    print "RemoveFileFromPath not yet implemented!!!!!"
#   char *ls = 0 
#   while(*path)
#     if(*path == '\\' || *path == '/') ls = path :
#     path++ 
#   if(ls) *ls = 0 :

def void SetCWDToEXE():
    print "SetCWDToEXE not yet implemented!!!!!!"
#   char exepath[1024] = {0} 
#   GetModuleFileNameA(0, exepath, sizeof(exepath)) 
#   assert(exepath[0]) 
#   RemoveFileFromPath(exepath) 
#   _chdir(exepath) 

def isProcessorBigEndian():
    print "isProcessorBigEndian not yet implemented!!!!!!"
    return False

## PT: those functions are useful for internal profiling during dev. They should not be used
## in the final code, so don't bother porting them to other platforms.

def StartProfile( val):
    print "StartProfile not yet implemented!!!!!!!!!"

def EndProfile(val):
    print "EndProfile not yet implemented!!!!!!!!!"

