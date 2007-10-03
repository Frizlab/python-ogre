import ogre.addons.plib as plib
import time

sched = plib.slScheduler ( 8000 ) 
mixer = plib.smMixer ( "/dev/mixer" ) 

mixer.setMasterVolume ( 50 )
sched.setSafetyMargin ( 0.128 )

s1 = plib.slSample ( 'wheeee.ub', sched )
s2 = plib.slSample ( 'zzap.wav', sched )

s1.adjustVolume ( 10.0 )
print dir(sched)
sched.loopSample ( s1 )
sched.loopSample ( s2 )

tim = 0 

while ( 1 ):
    tim+=1
    time.sleep ( 1 / 30  )  ## simulate 30 frames per second 
    sched.update ()


