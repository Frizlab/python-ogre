# NO_UNITTEST
import Pyro.core

#get the object proxy from the server
app_main = Pyro.core.getProxyForURI("PYROLOC://localhost:7766/app_main")

#you can call any funtions that are NOT returning ogre objects, because they are not serializable.
#maybe this can be enabled in the boost:python configuration, it's named pickling

#you can't do this: 
#app_main.getOgreRoot()

app_main.showConfig()
app_main.initWindow()
app_main.startRendering()