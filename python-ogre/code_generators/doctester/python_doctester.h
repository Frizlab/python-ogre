#include <string>
typedef float Real;

namespace DocTester
{
    /** The root class of the Ogre system.
        @remarks
            The Ogre::Root class represents a starting point for the client
            application. From here, the application can gain access to the
            fundamentals of the system, namely the rendering systems
            available, management of saved configurations, logging, and
            access to other classes in the system. Acts as a hub from which
            all other objects may be reached. An instance of Root must be
            created before any other Ogre operations are called. Once an
            instance has been created, the same instance is accessible
            throughout the life of that object by using Root::getSingleton
            (as a reference) or Root::getSingletonPtr (as a pointer).
    */
    class  Root 
    {
        // To allow update of active renderer if
        // RenderSystem::initialise is used directly
        friend class RenderSystem;
    public:
    
        // In case multiple render windows are created, only once are the resources loaded.
        bool mFirstTimePostWindowInit;

        // Singletons
        int mLogManager;

		/// Are we initialised yet?
		bool mIsInitialised;

        /** Method reads a plugins configuration file and instantiates all
            plugins.
            @param
                pluginsfile The file that contains plugins information.
                Defaults to "plugins.cfg".
        */
        void loadPlugins( const std::string pluginsfile = "plugins.cfg" );
		/** Initialise all loaded plugins - allows plugins to perform actions
			once the renderer is initialised.
		*/
		void initialisePlugins();
		/** Shuts down all loaded plugins - allows things to be tidied up whilst
			all plugins are still loaded.
		*/
		void shutdownPlugins();

		/** Unloads all loaded plugins.
        */
        void unloadPlugins();

        // Internal method for one-time tasks after first window creation
        void oneTimePostWindowInit(void);

        /** Set of registered frame listeners */
        int mFrameListeners;

        /** Set of frame listeners marked for removal*/
        char mRemovedFrameListeners;

        /** Indicates the type of event to be considered by calculateEventTime(). */
        enum FrameEventTimeType {
            FETT_ANY = 0, 
			FETT_STARTED = 1, 
			FETT_QUEUED = 2, 
			FETT_ENDED = 3, 
			FETT_COUNT = 4
        };

        /// Contains the times of recently fired events
        unsigned long mEventTimes[FETT_COUNT];

        /** Internal method for calculating the average time between recently fired events.
        @param now The current time in ms.
        @param type The type of event to be considered.
        */
        Real calculateEventTime(unsigned long now, char type);
    public:

        /** Constructor
        @param pluginFileName The file that contains plugins information.
            Defaults to "plugins.cfg", may be left blank to ignore.
		@param configFileName The file that contains the configuration to be loaded.
			Defaults to "ogre.cfg", may be left blank to load nothing.
		@param logFileName The logfile to create, defaults to Ogre.log, may be 
			left blank if you've already set up LogManager & Log yourself
		*/
        Root(const std::string& pluginFileName = "plugins.cfg", 
			const std::string& configFileName = "ogre.cfg", 
			const std::string& logFileName = "Ogre.log");
        ~Root();

        /** Saves the details of the current configuration
            @remarks
                Stores details of the current configuration so it may be
                restored later on.
        */
        void saveConfig(void);

        /** Checks for saved video/sound/etc settings
            @remarks
                This method checks to see if there is a valid saved configuration
                from a previous run. If there is, the state of the system will
                be restored to that configuration.

            @returns
                If a valid configuration was found, <b>true</b> is returned.
            @par
                If there is no saved configuration, or if the system failed
                with the last config settings, <b>false</b> is returned.
        */
        bool restoreConfig(void);

        /** Displays a dialog asking the user to choose system settings.
            @remarks
                This method displays the default dialog allowing the user to
                choose the rendering system, video mode etc. If there is are
                any settings saved already, they will be restored automatically
                before displaying the dialogue. When the user accepts a group of
                settings, this will automatically call Root::setRenderSystem,
                RenderSystem::setConfigOption and Root::saveConfig with the
                user's choices. This is the easiest way to get the system
                configured.
            @returns
                If the user clicked 'Ok', <b>true</b> is returned.
            @par
                If they clicked 'Cancel' (in which case the app should
                strongly consider terminating), <b>false</b> is returned.
         */
 
        /** Sets the rendering subsystem to be used.
            @remarks
                This method indicates to OGRE which rendering system is to be
                used (e.g. Direct3D, OpenGL etc). This is called
                automatically by the default config dialog, and when settings
                are restored from a previous configuraion. If used manually
                it could be used to set the renderer from a custom settings
                dialog. Once this has been done, the renderer can be
                initialised using Root::initialise.
            @par
                This method is also called by render systems if they are
                initialised directly.
            @param
                system Pointer to the render system to use.
            @see
                RenderSystem
        */
        void setRenderSystem(int system);

        /** Retrieve a pointer to the currently selected render system.
        */
        void getRenderSystem(void);

        /** Initialises the renderer.
            @remarks
                This method can only be called after a renderer has been
                selected with Root::setRenderSystem, and it will initialise
                the selected rendering system ready for use.
            @param
                autoCreateWindow If true, a rendering window will
                automatically be created (saving a call to
                Root::createRenderWindow). The window will be
                created based on the options currently set on the render
                system.
            @returns
                A pointer to the automatically created window, if
                requested, otherwise <b>NULL</b>.
        */
	    int initialise(bool autoCreateWindow, const std::string& windowTitle = "OGRE Render Window",
                                    const std::string& customCapabilitiesConfig = "");

		/** Returns whether the system is initialised or not. */
		bool isInitialised(void) const { return mIsInitialised; }

 
		/** Create a SceneManager instance of a given type.
		@remarks
			You can use this method to create a SceneManager instance of a 
			given specific type. You may know this type already, or you may
			have discovered it by looking at the results from getMetaDataIterator.
		@note
			This method throws an exception if the named type is not found.
		@param typeName String identifying a unique SceneManager type
		@param instanceName Optional name to given the new instance that is
			created. If you leave this blank, an auto name will be assigned.
		*/
		double* createSceneManager(const std::string& typeName, 
			const std::string& instanceName = "");

        /** Retrieves a reference to the current TextureManager.
            @remarks
                This performs the same function as
                TextureManager::getSingleton, but is provided for convenience
                particularly to scripting engines.
            @par
                Note that a TextureManager will NOT be available until the
                Ogre system has been initialised by selecting a RenderSystem,
                calling Root::initialise and a window having been created
                (this may have been done by initialise if required). This is
                because the exact runtime subclass which will be implementing
                the calls will differ depending on the rendering engine
                selected, and these typically require a window upon which to
                base texture format decisions.
        */
        int getTextureManager(void);

    };
} // Namespace Ogre
