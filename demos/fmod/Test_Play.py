import FMOD
import sys

if __name__ == '__main__':
    # you can't create a system directly ..ie FMOD.System() is invalid
    system = FMOD.PYSystem_Create()
    
    #system.init(1,0,FMOD.CastVoidPtr(0)) #  This is another example of a function that crashes
    FMOD.PYFMOD_System_Init (system, 1, 0, FMOD.CastVoidPtr(0) ) # use a utility function instead
    
    # yet this works fine!!, However it's a function transformation case so has been "wrapped"
    result, sound1 = system.createSound( "../media/sounds/drumloop.wav", 32 , FMOD.CastVoidPtr(0))  
    # and this is fine, again a function transformation piece..
    result, channel = system.playSound(FMOD.FMOD_CHANNEL_FREE, sound1, False)
    x=0
    while x != 10000:
        ## system.update() ## this one failes
        system.update1() ## this one works
        print x
        x+=1

    result = sound1.release()
    result = system.close()
    result = system.release()



