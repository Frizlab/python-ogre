#ifndef CustomPagingLandScapeListener_H
#define CustomPagingLandScapeListener_H

namespace Ogre {

class CustomPagingLandScapeListener 
{
    public:
        //---------------------------------------------------------------------
        CustomPagingLandScapeListener (SceneManager *sceneMgr) :
		mSceneMgr(sceneMgr)
        {

			mterrainreadylistener = new PagingLandscapeDelegate(&(*this), 
								&CustomPagingLandScapeListener::terrainReady);
			mSceneMgr->setOption( "addTerrainListener", mterrainreadylistener ); 

			mpagepreloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pagePreloaded);
			mSceneMgr->setOption( "addPreloadPageListener", mpagepreloadlistener );
			mpageloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pageLoaded);
			mSceneMgr->setOption( "addLoadPageListener", mpageloadlistener ); 
			mpageunloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pageUnloaded);
			mSceneMgr->setOption( "addUnloadPageListener", mpageunloadlistener );
			mpagepostunloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pagePostunloaded);
			mSceneMgr->setOption( "addPostunloadPageListener", mpagepostunloadlistener ); 
			mpageshowlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pageShow);
			mSceneMgr->setOption( "addShowPageListener", mpageshowlistener );   
			mpagehidelistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::pageHide);
			mSceneMgr->setOption( "addHidePageListener", mpagehidelistener );   
			mpagedeformedlistener= new PagingLandscapeDelegate(&(*this), 
				&CustomPagingLandScapeListener::pageDeformed);
			mSceneMgr->setOption( "addModifyPageListener", mpagedeformedlistener );           


			mtileloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::tileLoaded);
			mSceneMgr->setOption( "addLoadTileListener", mtileloadlistener );
			mtileunloadlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::tileUnloaded);
			mSceneMgr->setOption( "addUnloadTileListener", mtileunloadlistener ); 
			mtileshowlistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::tileShow);
			mSceneMgr->setOption( "addShowTileListener", mtileshowlistener );
			mtilehidelistener = new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::tileHide);
			mSceneMgr->setOption( "addHideTileListener", mtilehidelistener ); 
			mtiledeformedlistener= new PagingLandscapeDelegate(&(*this), 
							&CustomPagingLandScapeListener::tileDeformed);
			mSceneMgr->setOption( "addModifyTileListener", mtiledeformedlistener );    
        }
        //---------------------------------------------------------------------
        ~CustomPagingLandScapeListener ()
        {
        

			mSceneMgr->setOption( "removeTerrainListener", mterrainreadylistener ); 
			
			mSceneMgr->setOption( "removePreloadPageListener", mpagepreloadlistener );
			mSceneMgr->setOption( "removeLoadPageListener", mpageloadlistener ); 
			mSceneMgr->setOption( "removeUnloadPageListener", mpageunloadlistener );
			mSceneMgr->setOption( "removePostunloadPageListener", mpagepostunloadlistener ); 
			mSceneMgr->setOption( "removeShowPageListener", mpageshowlistener );   
			mSceneMgr->setOption( "removeHidePageListener", mpagehidelistener );   
			mSceneMgr->setOption( "removeModifyPageListener", mpagedeformedlistener );   
			  
			mSceneMgr->setOption( "removeLoadTileListener", mtileloadlistener );
			mSceneMgr->setOption( "removeUnloadTileListener", mtileunloadlistener ); 
			mSceneMgr->setOption( "removeShowTileListener", mtileshowlistener );
			mSceneMgr->setOption( "removeHideTileListener", mtilehidelistener ); 
			mSceneMgr->setOption( "removeModifyTileListener", mtiledeformedlistener );    
        

            delete mterrainreadylistener;

            delete mpagepreloadlistener;
            delete mpageloadlistener;
            delete mpageunloadlistener;
            delete mpagepostunloadlistener;
            delete mpageshowlistener;
            delete mpagehidelistener;
            delete mpagedeformedlistener;


            delete mtileloadlistener;
            delete mtileunloadlistener;
            delete mtileshowlistener;
            delete mtilehidelistener;
            delete mtiledeformedlistener;
		}
		//---------------------------------------------------------------------
		virtual void terrainReady(PagingLandscapeEvent *p)
		{
			EventString += " | Terrain Ready";			
		}
        //---------------------------------------------------------------------
        virtual void pagePreloaded(PagingLandscapeEvent *p) 
        {
            EventString += " | pagePreloaded: " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez));

        }
        //---------------------------------------------------------------------
        virtual void pageLoaded(PagingLandscapeEvent *p )  
        {
            EventString += " | pageLoaded: " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez));
        }
        //---------------------------------------------------------------------
        virtual void pageUnloaded(PagingLandscapeEvent *p ) 
        {
            EventString += " | pageUnloaded: " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez));
        }
        //---------------------------------------------------------------------
        virtual void pagePostunloaded(PagingLandscapeEvent *p) 
        {
           EventString += " | pagePostunloaded: " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez));
        }
		//---------------------------------------------------------------------
		virtual void pageDeformed(PagingLandscapeEvent *p) 
		{
			EventString += " | PageDeformed: " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez));
		}
		//---------------------------------------------------------------------
        virtual void pageShow(PagingLandscapeEvent *p) 
        {
            EventString += " | pageShow: " 
                + StringConverter::toString(static_cast <Ogre::uint>  (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint>  (p->mPagez));
        }

		//---------------------------------------------------------------------
        virtual void pageHide(PagingLandscapeEvent *p ) 
        {
            EventString += " | pageHide: " 
                + StringConverter::toString(static_cast <Ogre::uint>  (p->mPagex)) + ", " 
                + StringConverter::toString(static_cast <Ogre::uint>  (p->mPagez));
        }

        //---------------------------------------------------------------------
        virtual void tileLoaded (PagingLandscapeEvent *p) 
        {
           EventString += " | tileLoaded: " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez)) + " | " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilez));
        }
        //---------------------------------------------------------------------
        virtual void tileUnloaded(PagingLandscapeEvent *p) 
        {
           EventString += "  | tileUnloaded: " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez)) + " | " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilez));
        }
        //---------------------------------------------------------------------
        virtual void tileDeformed(PagingLandscapeEvent *p ) 
        {
           EventString += " | tileDeformed: " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez)) + " | " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilex)) + ", " 
               + StringConverter::toString(static_cast <Ogre::uint> (p->mTilez));
        }

		//---------------------------------------------------------------------
        virtual void tileShow(PagingLandscapeEvent *p) 
        {
//            EventString += " | tileShow: " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez)) + " | " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mTilex)) + ", " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mTilez));
        }
		//---------------------------------------------------------------------
        virtual void tileHide(PagingLandscapeEvent *p ) 
        {
//            EventString += " | tileHide: " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagex)) + ", " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mPagez)) + " | " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mTilex)) + ", " 
//                + StringConverter::toString(static_cast <Ogre::uint> (p->mTilez));
        }
        //---------------------------------------------------------------------
        String getEventString () 
        {
            #ifdef _DEBUG
                std::cout << EventString << '\n';
            #endif //_DEBUG
            return EventString;
        }
        //---------------------------------------------------------------------
        void clearEventString () 
        {
            EventString.clear ();
        }
        //---------------------------------------------------------------------
        bool hasEvent ()
        {
            return !(EventString.empty ());
        }

    protected:
        SceneManager *mSceneMgr;

		String EventString;
		
		PagingLandscapeDelegate *mterrainreadylistener;

		PagingLandscapeDelegate *mpagepreloadlistener;
		PagingLandscapeDelegate *mpageloadlistener;
		PagingLandscapeDelegate *mpageunloadlistener;
		PagingLandscapeDelegate *mpagepostunloadlistener;
		PagingLandscapeDelegate *mpageshowlistener;
		PagingLandscapeDelegate *mpagehidelistener;
		PagingLandscapeDelegate *mpagedeformedlistener;


		PagingLandscapeDelegate *mtileloadlistener;
		PagingLandscapeDelegate *mtileunloadlistener;
		PagingLandscapeDelegate *mtileshowlistener;
		PagingLandscapeDelegate *mtilehidelistener;
		PagingLandscapeDelegate *mtiledeformedlistener;

};
}

#endif

