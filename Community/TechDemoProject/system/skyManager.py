#// Quick 'n Dirty Sky Manager v1.0
#// by mikeInside 2007
#// http://mikeinside.modthesims2.com
#// This code is under the Whatevar! licence. Do what you want; but keep the original copyright header.
# Pythonised by Ben Harling



#       createStar(Star("sun", ColourValue( 1, 1, 1 ), ColourValue( 0.4, 0.3, 0.2 ), 
#           100, Vector3::UNIT_Y, Radian(0), 800, 700));

import ogre.renderer.OGRE as ogre
            
class Star:
    def __init__(self, name, diffuse, specular, orbitTime=20, rotAxis=ogre.Vector3.UNIT_Y,
                     rotAngle=ogre.Radian(0), distance=300, size=400):
        self.name = name
        self.diffuse = diffuse
        self.specular = specular
        self.orbitTime = orbitTime
        self.rotAxis = rotAxis
        self.rotAngle = rotAngle
        self.distance = distance
        self.size = size
        
        
class SkyManager(ogre.FrameListener):
    def __init__(self, useDefaults = True, directionalStars = True, sceneMgrName = ""):
        ogre.FrameListener.__init__(self)
        self.mTime = 0
        self.mStartFade = 0.85
        self.mEndFade = 0.30
        self.mStarList = []
        
        self.events = []
        
        if sceneMgrName != "":
            self.mSceneMgr = ogre.Root.getSingleton().getSceneManager(sceneMgrName)
        else:
            self.mSceneMgr = ogre.Root.getSingleton().getSceneManager("MainScene")

        self.mDirectionalStars = directionalStars

        self.mSceneMgr.setSkyDome(True, "SkydomeShaderBlend", 5, 8)
        self.mActiveMaterial = ogre.MaterialManager.getSingleton().getByName("SkydomeShaderBlend")
        self.mActivePass = self.mActiveMaterial.getTechnique(0).getPass(0)
        self.mActiveFragmentProgram = self.mActivePass.getFragmentProgram()
        self.mActiveFragmentParameters = self.mActivePass.getFragmentProgramParameters()

        self.mNightLight = self.mSceneMgr.createLight( "nightLight" )
        self.setNightLightParameters()

        if useDefaults:
            self.makeDefaultStars()


#List of default stars that will be used by the init method
    def makeDefaultStars(self):
        self.mStarList = []
        self.createStar(Star("sun_sol_", ogre.ColourValue( 1, 1, 1 ), ogre.ColourValue( 0.4, 0.3, 0.2 ), 
            100, ogre.Vector3.UNIT_Y, ogre.Radian(0), 1700, 1500))
        #self.createStar(Star("moon", ogre.ColourValue( 0.15, 0.2, 0.3 ), ogre.ColourValue( 0.1, 0.2, 0.6 ), 
            #27, ogre.Vector3(0.2,1,0), ogre.Radian(0.7), 750, 250))
#       //createStar(Star("redsun", ColourValue( 0.5, 0.3, 0.2 ), ColourValue( 0.4, 0.1, 0.0 ), 
#       //  65, Vector3(0,1,0.2), Radian(2), 700, 300));
    
#Creates a particle system and light based on the parameters in the given star
    def createStar(self, star):
        self.createParticleTemplate(star)
        system = self.mSceneMgr.createParticleSystem(star.name+"System", star.name+"ParticleTemplate")
        
        rotationNode = self.mSceneMgr.getRootSceneNode().createChildSceneNode(star.name+"RotationNode")
        rotationNode.rotate(star.rotAxis, star.rotAngle)
        
        positionNode = rotationNode.createChildSceneNode(star.name+"PositionNode")
        positionNode.attachObject(system)

        light = self.mSceneMgr.createLight( star.name+"Light" )
        if self.mDirectionalStars:
            light.setType( ogre.Light.LT_DIRECTIONAL )
        else:
            light.setType( ogre.Light.LT_POINT )
        light.setPosition( ogre.Vector3(0, 0, 0) )
        light.setDiffuseColour( star.diffuse )
        light.setSpecularColour( star.specular )
        positionNode.attachObject(light)

        self.mStarList.append(star)

    #Creates a particle effect that is based on the specular colour of the star
    def createParticleTemplate(self, star):
        system = ogre.ParticleSystemManager.getSingleton().createTemplate(
        star.name+"ParticleTemplate", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        system.setMaterialName("starFlare")
        system.setParameter("particle_width", str(star.size))
        system.setParameter("particle_height", str(star.size))
        system.setParameter("cull_each", "false")
        system.setParameter("quota", "200")
        system.setParameter("billboard_type", "point")

        emitter = system.addEmitter("Ellipsoid")
        emitter.setParameter("angle", "10")
        emitter.setParameter("emission_rate", "15")
        emitter.setParameter("time_to_live", "3")
        emitter.setParameter("direction", "0 1 0")
        emitter.setParameter("velocity", "0.001")
        emitter.setParameter("colour", 
            str(star.specular.r)+ " " + 
            str(star.specular.g)+ " " + 
            str(star.specular.b))
        emitter.setParameter("width", "1")
        emitter.setParameter("height", "1")
        emitter.setParameter("depth", "1")
        affector = system.addAffector("ColourFader")
        fadeSpeed = 0.8
        totalSpecular = star.specular.r + star.specular.g + star.specular.b
        affector.setParameter("red",  str((-1+(star.specular.r/totalSpecular))*fadeSpeed))
        affector.setParameter("green",str((-1+(star.specular.g/totalSpecular))*fadeSpeed))
        affector.setParameter("blue", str((-1+(star.specular.b/totalSpecular))*fadeSpeed))

    def frameStarted(self, timeSinceLastFrame):
        self.mTime += timeSinceLastFrame
        brightestLight = 0
        for i in range(len(self.mStarList)):
            star = self.mStarList[i]
            #           // find what percentage of the star's orbit it is currently at
            percent = self.mTime/star.orbitTime - int(self.mTime/star.orbitTime)
            x = star.distance * ogre.Math.Cos(percent*ogre.Math.TWO_PI)
            y = star.distance * ogre.Math.Sin(percent*ogre.Math.TWO_PI)

            #           // set star (light and particle) position
            node = self.mSceneMgr.getSceneNode(star.name+"PositionNode")
            node.setPosition( ogre.Vector3(x, y, 0) )
            


            #           // set light direction
            light = self.mSceneMgr.getLight(star.name+"Light")      
            if self.mDirectionalStars:
                light.setDirection(-x,-y,0)


            #           // find the current height of the star
            #           // 0.0 = midnight (star is below world)
            #           // 0.5 = sunset (star is on horizon)
            #           // 1.0 = midday (star is directly above)
            starHeight = (y+star.distance)/(star.distance*2)
            #           // brightness changes as the star moves through the fade zone, but is otherwise 0 or 1
            starBrightness = (starHeight - self.mEndFade)/(self.mStartFade - self.mEndFade)
            
            
            
            # set corona brightness ...
##            system = ogre.ParticleSystemManager.getSingleton().getTemplate(star.name+"ParticleTemplate")
##            coronaSize = star.size * starBrightness
##            system.setParameter("particle_width", str(coronaSize))
##            
##            system.setParameter("particle_height", str(coronaSize))

            if starBrightness <= 0:
            #               // star is below fade zone - i.e. below the world
                starBrightness = 0
                light.setVisible(False)
            else:
            #               // star is either in the fade zone or above it
                light.setVisible(True)
                if starBrightness >= 1:
                    starBrightness = 1
                light.setDiffuseColour(
                    starBrightness*star.diffuse.r,
                    starBrightness*star.diffuse.g,
                    starBrightness*star.diffuse.b)
                light.setSpecularColour(
                    starBrightness*star.specular.r,
                    starBrightness*star.specular.g,
                    starBrightness*star.specular.b)


            #           // check if this star is the brightest one found so far
            if starBrightness > brightestLight:
                brightestLight = starBrightness
            #           // if the sky brightness is linked to a particular star, set it when the star is found
##            if mainStar == i:
##                self.mActiveFragmentParameters.setNamedConstant("fBlend",starBrightness)

#       // if sky brightness can be based on any star, use the brightest star currently in the sky
        if mainStar < 0:
            self.mActiveFragmentParameters.setNamedConstant("fBlend",brightestLight)


#       // if there are no stars in the sky, turn on the night light
        
        ambient = self.mNightLightPower - (brightestLight * self.mNightLightPower)
        self.mNightLight.setDiffuseColour(
            ambient*self.mNightLightColour.r,
            ambient*self.mNightLightColour.g,
            ambient*self.mNightLightColour.b)
        self.mNightLight.setSpecularColour(
            ambient*self.mNightLightColour.r,
            ambient*self.mNightLightColour.g,
            ambient*self.mNightLightColour.b)
            
        
##        if brightestLight == 0:
##            self.setNightLightVisible(True)
##        else:
##            self.setNightLightVisible(False)
            



#Set when the star starts fading to when it is completely dark
#0.0 = midnight (star is below world)
#0.5 = sunset (star is on horizon)
#1.0 = midday (star is directly above)
    def setFade(self, _mStartFade, _mEndFade):
        self.mStartFade = _mStartFade
        self.mEndFade = _mEndFade

    def setNightLightVisible(self, visible):
        print 'SettingNightLight'
        self.mNightLight.setVisible(visible)
        
        # This adds an event to the eventManager
        # relaying the message to all actors that
        # it is sunset (or sunrise).
        self.events.append([1, "dayNightToggle", visible])
         
        

    def setNightLightParameters(self, directional = True, positionOrDirection = ogre.Vector3(0.1,-1,-0.1), mNightLightColour = ogre.ColourValue(0.3, 0.45, 1.0), mNightLightPower = 0.3, shadows = True):
        if directional:
            self.mNightLight.setType( ogre.Light.LT_DIRECTIONAL )
            self.mNightLight.setDirection(positionOrDirection)
        else:
            self.mNightLight.setType( ogre.LightLT_POINT )
            self.mNightLight.setPosition( positionOrDirection )
        self.mNightLight.setDiffuseColour( 0.3,0.2,0.1 )
        self.mNightLight.setSpecularColour( 0,0,0 )
        self.mNightLight.setCastShadows(shadows)
        self.mNightLightColour = mNightLightColour
        self.mNightLightPower  = mNightLightPower

    def setTime(self, time):
        self.mTime = time

    def __del__(self): 
        self.mActiveFragmentParameters = None
        self.mActiveFragmentProgram = None
        self.mActiveMaterial= None

