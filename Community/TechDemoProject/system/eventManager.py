#-----------------------------------------------------------------------------#
#                                                                             #
#   This source code is part of the python-ogre techdemo project.             #
#                                                                             #
#   This program is released as public domain                                 #
#                                                                             #
#-----------------------------------------------------------------------------#
#   
#   TITLE: EventManager
#   DESCRIPTION: Application wide event gathering and dispatching
#   AUTHOR: Ben Harling
#   Based on: http://www.ogre3d.org/phpBB2addons/viewtopic.php?t=4049&start=0&postdays=0&postorder=asc&highlight=heapq

import heapq
import logging
# -- Simple Event Manager
# -- the update method is run every frame to see if any event is in
# -- the queue, it then filters down to lists of the events happening
# -- on or around this frame, and returns them to the datamanager

# -- Events are indexed by time, so the first value must be an int
# -- the time is the delay before you want the event to occur
# -- Example: 

# -- in your actor's update method:
#
#       if self.foundFood:
#           self.events.append([1, "eatFood", foodActor, [22,55,3]])
#
# -- in a frameListener or manager object
#
#       if sun.brightness < 0.0:
#           self.events.append([1, 'NightTime'])

# -- the datamanager then adds to the eventManager
#            EventManager.addWorldEvent(2, "NightTime")

class Event:
    def __init__(self, time, name, function, *args):
        self.time = time
        self.name = name
        self.function = function
        self.args = args
    def __cmp__(self, other):
        return cmp(self.time, other.time)
    def __repr__(self):
        return ('Event Name = ' + self.name + ' - Event Time: ' + str(self.time))

class EventManager:
    def __init__(self):
        self.worldTime = 0.0
        self.systemEvents = []
        self.actorEvents = []
        self.worldEvents = []
        
    def addWorldEvent(self, evt):
        evt.time += self.worldTime
        heapq.heappush(self.worldEvents, evt)
        
    def addSystemEvent(self, evt):
        evt.time += self.worldTime
        heapq.heappush(self.systemEvents, evt)
        
    def addActorEvent(self, evt):
        logging.debug('Actor Event Added' + str(evt))
        evt.time += self.worldTime
        heapq.heappush(self.actorEvents, evt)
        
    def update(self, time):
        # update time
        self.worldTime += time
        
        # create event 'out-boxes' to send to datamanager
        world_events = []
        actor_events = []
        system_events = []
        
        while self.worldEvents and self.worldEvents[0].time <= self.worldTime:
            world_events.append( heapq.heappop(self.worldEvents) )
                
        while self.actorEvents and self.actorEvents[0].time <= self.worldTime:
            actor_events.append( heapq.heappop(self.actorEvents) )
            
        while self.systemEvents and self.systemEvents[0].time <= self.worldTime:
            system_events.append( heapq.heappop(self.systemEvents) )
                
        return system_events, world_events, actor_events
                
##        if world_events:
##            for actor in self.ActorsList:
##                for we in world_events:
##                    try:
##                        func = actor.events[we[0]]
##                        params = we[1]
##                        func( params )
##                    except:
##                        pass
                