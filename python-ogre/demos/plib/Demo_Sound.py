import ogre.addons.plib as plib
import time

sched = plib.slScheduler (  ) 
# mixer = plib.smMixer ( "/dev/mixer" ) 

# mixer.setMasterVolume ( 50 )
sched.setSafetyMargin ( 0.5 )

s1 = plib.slSample ( '1.ogg' )
s2 = plib.slSample ( 'theme.ogg', sched )
# s1 = plib.slSample ( 'tuxr.mod', sched )

# s1.adjustVolume ( 10.0 )
# print dir(sched)
sched.loopSample ( s2 )
# sched.loopMusic ( 'tuxr.mod' )
# sched.loopMusic ( 'theme.ogg' )
print dir(sched)

tim = 0 

while ( 1 ):
#     tim+=1
#     time.sleep ( 2  )  ## simulate 30 frames per second 
#     print tim
    sched.update ()


