#ifndef CAELUMSYSTEM_H
#define CAELUMSYSTEM_H

#include "CaelumPrerequisites.h"

#include "UniversalClock.h"
#include "CaelumListener.h"
#include "SkyColourModel.h"
#include "SkyDome.h"
#include "Starfield.h"
#include "Sun.h"

namespace caelum {

/** Root of the Caelum system.
	This class is the root of the Caelum system.
	@author Jesús Alonso Abad
 */
class DllExport CaelumSystem : public Ogre::FrameListener, public Ogre::RenderTargetListener {
// Attributes -----------------------------------------------------------------
	public:
		/** Common resource group name.
		 */
		static Ogre::String RESOURCE_GROUP_NAME;

	private:
		/** Root of the Ogre engine.
		 */
		Ogre::Root *mOgreRoot;

		/** Scene manager.
		 */
		Ogre::SceneManager *mSceneMgr;

		/** Cleanup requested flag.
		 */
		bool mCleanup;

		/** Flag to let Caelum manage the creation and destruction of the resource group.
		 */
		bool mManageResourceGroup;

		/** List of listeners registered.
		 */
		std::set<CaelumListener *> mListeners;

		/// Reference to the universal clock.
		UniversalClock *mUniversalClock;
		
		/** The sky dome.
		 */
		SkyDome *mSkyDome;

		/** Reference to the sky colour model in use.
		 */
		SkyColourModel *mSkyColourModel;

		/** Flag to indicate if Caelum should manage the fog or not.
		 */
		bool mManageFog;

		/** Reference to the sun.
		 */
		Sun *mSun;

		/** Reference to the starfield.
		 */
		Starfield *mStarfield;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			Registers itself in the Ogre engine and initialises the system.
			@param root The Ogre rool.
			@param sceneMgr The Ogre scene manager.
			@param manageResGroup Tells the system if the resource group has been created externally (true) or if it's to be managed by the system.
			@param resGroupName The resource group name, if it's desired to use an existing one or just a different name.
			@param createSkyDome Whether if the sky dome should be created or not.
			@param createSun Whether if the sun should be created or not.
			@param createStarfield Whether if the starfield should be created or not.
		 */
		CaelumSystem (Ogre::Root *root, 
										Ogre::SceneManager *sceneMgr, 
										bool manageResGroup = true, 
										const Ogre::String &resGroupName = RESOURCE_GROUP_NAME,
										bool createSkyDome = true, bool createSun = true, bool createStarfield = true);

		/** Destructor.
		 */
		~CaelumSystem ();

		/** Shuts down the system and detaches itself from the Ogre engine.
			@param cleanup True if we want the shutdown to also delete the system. It's dangerous if 
				it's intended to be rendered another frame later on, and thus turned off by default.
			@remarks The model used in this system <b>won't be deleted here</b>.
			This is the shutdown function to be called, and not the destructor itself.
		 */
		void shutdown (const bool cleanup = false);
		
		/** Registers a listener in the system.
			This listener will be called each frame, before and after Caelum does its work.
			@param listener The listener to register.
		 */
		void addListener (CaelumListener *listener);

		/** Unregisters a listener in the system.
			@note The listener must be destroyed out of Caelum.
			@param listener The listener to be removed.
		 */
		void removeListener (CaelumListener *listener);

		/** Event trigger called just before rendering a viewport in a render target Caelum is attached to.
			Useful to make objects follow every camera that renders a viewport in a certain render target.
			@param e The viewport event, containing the viewport (and camera) to be rendered right now.
		 */
		void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

		/** Gets the universal clock.
		 * @return A reference to the universal clock attached to this system.
		 */
		UniversalClock *getUniversalClock () const;
		
		/** Updates the system.
			@param e The frame event (contains the elapsed time since the last update).
		 */
		bool frameStarted (const Ogre::FrameEvent &e);

		/** Creates the sky dome, or returns the existing one if any yet.
		 */
		SkyDome *createSkyDome ();

		/** Returns the current sky dome.
			@return The current sky dome.
		 */
		SkyDome *getSkyDome () const;

		/** Destroys the sky dome.
			@remarks Remember to detach the sky dome from every viewport it is attached to before destroying it!
		 */
		void destroySkyDome ();

		/** Creates the sun.
			@return The sun.
		 */
		Sun *createSun ();

		/** Gets the current sun.
			@return The sun in use.
		 */
		Sun *getSun () const;

		/** Destroys the sun.
		 */
		void destroySun ();

		/** Create the starfield.
			@note Returns the existing one if there's one already in use.
			@note The old texture will be replaced by the passed one.
			@param mapName Name of the starfield texture bitmap.
			@return The new or current starfield.
		 */
		Starfield *createStarfield (const Ogre::String &mapName = "Starfield.jpg");

		/** Gets the starfield.
			@return The starfield.
		 */
		Starfield *getStarfield () const;

		/** Destroys the current starfield.
			@remark Remember to detach it from every viewport before deleting!
		 */
		void destroyStarfield ();

		/** Sets the sky colour model to be used.
			@param model The sky colour model.
		 */
		void setSkyColourModel (SkyColourModel *model);

		/** Enables/disables the Caelum fog management.
			@param manage True if you want Caelum to manage the fog for you.
		 */
		void setManageFog (bool manage);

		/** Tells if Caelum is managing the fog or not.
			@return True if Caelum manages the fog.
		 */
		bool isFogManaged () const;

	private:
		/** Fires the start event to all the registered listeners.
			@param e The Ogre FrameEvent object passed this frame.
			@return True if all the listeners returned true.
		 */
		bool fireStartedEvent (const Ogre::FrameEvent &e);

		/** Fires the finish event to all the registered listeners.
			@param e The Ogre FrameEvent object passed this frame.
			@return True if all the listeners returned true.
		 */
		bool fireFinishedEvent (const Ogre::FrameEvent &e);
};

} // namespace caelum

#endif //CAELUMSYSTEM_H
