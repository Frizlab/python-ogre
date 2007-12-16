#ifndef GROUNDFOG_H
#define GROUNDFOG_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"

namespace caelum {

/** Exponential ground fog system implementation.
 *
 *  This class controls CaelumGroundFog passes in a potentially large number
 *  of materials, changing shader program parameters. This class keeps a list
 *  of passes to control; which can be build based on pass name.
 *
 *  This simulates a field of fog where "absorption" at a certain point is
 *	exp(-verticalDecay * (h - fogLevel)). This absorption is multiplicative,
 *	the total fog alpha is e^(-density * absorption_on_view_path).
 *
 *	You can set verticalDecay to 0 and get standard GL_EXP fog. Don't actually
 *	do that though because you'll get a division by 0.
 */
class DllExport GroundFog: public CameraBoundElement {
	public:
		static const Ogre::String DEFAULT_PASS_NAME;

		/** Constructor.
		 */
		GroundFog (Ogre::SceneManager *scene,
			const Ogre::String &domeMaterialName = "CaelumGroundFogDome",
			const Ogre::String &domeEntityName = "CaelumGroundFogDome");

		/** Virtual destructor.
		 */
		virtual ~GroundFog ();

		/** Typedef for easier manipulation of a set of Passes.
		 */
		typedef std::set<Ogre::Pass *> PassSet;

		/** Get the set of currently controlled passes.
		 *	This is provided if you really want to change the set by hand.
		 *  You should call forceUpdate after modifying this set.
		 */
		PassSet& getPasses();

		/** Get the set of currently controlled passes.
		 *  This is a const overload which doesn't let you modify the
		 *	underlying collection.
		 */
		const PassSet& getPasses () const;

		/** Find caelum fog passes to control by name.
		 *	By default this looks for passes called "CaleumGroundFog".
		 *	@note This calls forceUpdate()
		 */
		void findFogPassesByName (const Ogre::String& passName = DEFAULT_PASS_NAME);

		/// Sets the fog density multiplier
		void setDensity (Ogre::Real density);

		/// Get the fog density multiplier
		Ogre::Real getDensity ();

		/// Sets fog colour
		void setColour (const Ogre::ColourValue &colour);

		/// Gets fog colour
		Ogre::ColourValue getColour ();

		/// Sets the vertical fog decay constant.
		void setVerticalDecay (Ogre::Real verticalDecay);

		/// Get the vertical fog decay constant.
		Ogre::Real getVerticalDecay ();

		/** Sets the ground level.
		 *	At ground level 'fogginess' is equal to 1.
		 */
		void setGroundLevel (Ogre::Real GroundLevela);

		/** Get the ground level.
		 */
		Ogre::Real getGroundLevel ();

		/** Forces an update of all the passes. You have to use this if you modify
		 *	the set of passes by hand, otherwise avoid it.
		 */
		void forceUpdate ();

	private:
		/// Cached Density
		Ogre::Real mDensity;

		/// Cached VerticalDecay
		Ogre::Real mVerticalDecay;

		/// Cached GroundLevel
		Ogre::Real mGroundLevel;

        /// Fog colour
		Ogre::ColourValue mFogColour;

		/// The passes to control.
		PassSet mPasses;

		/// Sky dome material
		Ogre::MaterialPtr mDomeMaterial;

		/// Sky dome mesh
		Ogre::SceneNode *mDomeNode;

		/// Sky dome entity
		Ogre::Entity *mDomeEntity;

		/// The scene to control fog in.
		Ogre::SceneManager* mScene;

		// Called whenever something changes to update the sky dome.
		void updateSkyFogging();

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
};

}

#endif //GROUNDFOG_H