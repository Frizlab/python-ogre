#include "QuickGUIProgressBar.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String ProgressBar::BACKGROUND = "background";
	const Ogre::String ProgressBar::BAR = "bar";
	const Ogre::String ProgressBar::CLIPMAP = "clipmap";

	void ProgressBar::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("ProgressBar");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(BAR);
		d->defineSkinElement(CLIPMAP);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ProgressBar",d);
	}

	ProgressBarDesc::ProgressBarDesc() :
		WidgetDesc()
	{
		fillDirection = PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		layout = PROGRESSBAR_LAYOUT_HORIZONTAL;
		clippingEdge = PROGRESSBAR_CLIP_LEFT_BOTTOM;
		progress = 100;

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}
	}

	void ProgressBarDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);
		}

		textDesc.serialize(b);

		b->IO("Progress",&progress);
		b->IO("FillDirection",&fillDirection);
		b->IO("Layout",&layout);
		b->IO("ClippingEdge",&clippingEdge);
	}

	ProgressBar::ProgressBar(const Ogre::String& name) :
		Widget(name)
	{
		mSkinElementName = BACKGROUND;
	}

	ProgressBar::~ProgressBar()
	{
		mOutputBarTexture.setNull();
	}

	void ProgressBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<ProgressBarDesc*>(mWidgetDesc);

		ProgressBarDesc* pd = dynamic_cast<ProgressBarDesc*>(d);

		setSkinType(d->skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = pd->textDesc;

		mDesc->textDesc.allottedWidth = pd->dimensions.size.width - (pd->padding[PADDING_LEFT] + pd->padding[PADDING_RIGHT]);
		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,pd->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,pd->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,pd->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,pd->padding[PADDING_TOP]);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = Ogre::ColourValue::White;

		// Create Output Bar texture
		mOutputBarTexture = Ogre::TextureManager::getSingletonPtr()->createManual(getName() + ".bar.output", 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::TEX_TYPE_2D,
			static_cast<Ogre::uint>(mBarImage.getWidth()),
			static_cast<Ogre::uint>(mBarImage.getHeight()),
			0, 
			Ogre::PF_B8G8R8A8,
			Ogre::TU_STATIC);

		mDesc->clippingEdge = pd->clippingEdge;
		mDesc->fillDirection = pd->fillDirection;
		mDesc->layout = pd->layout;
		setProgress(pd->progress);
	}

	void ProgressBar::_processClipMap()
	{
		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(CLIPMAP);
			Ogre::Image i = se->getTextureImage();

			int width = static_cast<int>(i.getWidth());
			int height = static_cast<int>(i.getHeight());

			mClipMap.assign(width * height,true);

			for(int row = 0; row < height; ++row)
			{
				for(int col = 0; col < width; ++col)
				{
					if(i.getColourAt(col,row,0).a < 0.01)
						mClipMap[(row * width) + col] = false;
				}
			}
		}
	}

	void ProgressBar::addProgressBarEventHandler(ProgressBarEvent EVENT, EventHandlerSlot* function)
	{
		mProgressBarEventHandlers[EVENT].push_back(function);
	}

	Widget* ProgressBar::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new ProgressBar(widgetName);

		newWidget->_createDescObject("ProgressBarDesc");

		return newWidget;
	}

	bool ProgressBar::fireProgressBarEvent(ProgressBarEvent e, EventArgs& args)
	{
		if(mProgressBarEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mProgressBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String ProgressBar::getClass()
	{
		return "ProgressBar";
	}

	ProgressBarClippingEdge ProgressBar::getClippingEdge()
	{
		return mDesc->clippingEdge;
	}

	ProgressBarFillDirection ProgressBar::getFillDirection()
	{
		return mDesc->fillDirection;
	}

	ProgressBarLayout ProgressBar::setLayout()
	{
		return mDesc->layout;
	}

	float ProgressBar::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","ProgressBar::getPadding");

		return mDesc->padding[p];
	}

	float ProgressBar::getProgress()
	{
		return mDesc->progress;
	}

	Ogre::UTFString ProgressBar::getText()
	{
		return mText->getText();
	}

	float ProgressBar::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	void ProgressBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		brush->setTexture(mOutputBarTexture);
		Rect barRect = mClientDimensions;
		barRect.translate(mTexturePosition);
		brush->drawRectangle(barRect,UVRect(0,0,1,1));

		/*
		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT],
				mDesc->dimensions.size.height - mDesc->padding[PADDING_BOTTOM]);
		clipRegion.position = mTexturePosition;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);

		Brush::getSingleton().setColor(prevColor);
		*/
	}

	void ProgressBar::setClippingEdge(ProgressBarClippingEdge e)
	{
		mDesc->clippingEdge = e;

		setProgress(mDesc->progress);
	}

	void ProgressBar::setColor(const Ogre::ColourValue& cv)
	{
		mCurrentColourValue = cv;

		mText->setColor(cv);

		redraw();
	}

	void ProgressBar::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void ProgressBar::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void ProgressBar::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void ProgressBar::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void ProgressBar::setFillDirection(ProgressBarFillDirection d)
	{
		mDesc->fillDirection = d;

		setProgress(mDesc->progress);
	}

	void ProgressBar::setFont(const Ogre::String& fontName)
	{
		mCurrentFontName = fontName;

		mText->setFont(fontName);

		redraw();
	}

	void ProgressBar::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void ProgressBar::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void ProgressBar::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void ProgressBar::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void ProgressBar::setLayout(ProgressBarLayout l)
	{
		mDesc->layout = l;

		setProgress(mDesc->progress);
	}

	void ProgressBar::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","ProgressBar::setPadding");

		mDesc->padding[p] = distance;
		mText->setAllottedWidth(mWidgetDesc->dimensions.size.width - (mDesc->padding[PADDING_LEFT] + mDesc->padding[PADDING_RIGHT]));

		redraw();
	}

	void ProgressBar::setProgress(float percent)
	{
		if(percent > 100)
			percent = 100;
		else if(percent < 0)
			percent = 0;

		mDesc->progress = percent;

		// Update Bar Output Texture to match percentage
		{
			// Determine the Rect to blit from the bar texture. All values are percentages
			Rect sourceArea;
			Rect destArea;

			if(mDesc->layout == PROGRESSBAR_LAYOUT_HORIZONTAL)
			{
				// Left to right
				if(mDesc->fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progress / 100.0;
					destArea.size.height = 1.0;
				}
				// Right to left
				else
				{
					if(mDesc->clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = (100 - mDesc->progress) / 100.0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progress / 100.0;
					destArea.size.height = 1.0;
				}
			}
			else
			{
				// Bottom to top
				if(mDesc->fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progress / 100.0;
					}

					destArea.position.x = 0;
					destArea.position.y = (100 - mDesc->progress) / 100.0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progress / 100.0;
				}
				// Top to Bottom
				else
				{
					if(mDesc->clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progress / 100.0;
					}
					
					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progress / 100.0;
				}
			}

			// Clear our output bar texture and blit to it - src rect from Bar texture, dst rect is location in output texture
			
			int barWidth = static_cast<int>(mBarImage.getWidth());
			int barHeight = static_cast<int>(mBarImage.getHeight());

			// Hardware Pixel Buffer for moving pixels around, and correctly creating the barTexture.
			Ogre::HardwarePixelBufferSharedPtr buf = mOutputBarTexture->getBuffer();
			buf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
			Ogre::uint8* bufPtr = static_cast<Ogre::uint8*>((buf->getCurrentLock()).data);
			for( int i = 0; i < barHeight; ++i )
			{
				for( int j = 0; j < barWidth; ++j )
				{
					*bufPtr++ = 0;	// R
					*bufPtr++ = 0;	// B
					*bufPtr++ = 0;	// G
					*bufPtr++ = 0;	// A
				}
			}
			buf->unlock();

			if(mDesc->progress <= 0.0)
				return;

			// Blit the area of the Bar texture to the output texture. (Grab pixels from Src in Bar texture, and add to Dest in Output Texture)

			Ogre::PixelBox srcBox = mBarImage.getPixelBox().getSubVolume(Ogre::Box(sourceArea.position.x * barWidth,sourceArea.position.y * barHeight,(sourceArea.position.x * barWidth) + (sourceArea.size.width * barWidth),(sourceArea.position.y * barHeight) + (sourceArea.size.height * barHeight)));
			Ogre::Image::Box dstBox = Ogre::Image::Box(destArea.position.x * barWidth,destArea.position.y * barHeight,(destArea.position.x * barWidth) + (destArea.size.width * barWidth),(destArea.position.y * barHeight) + (destArea.size.height * barHeight));

			buf->blitFromMemory(srcBox,dstBox);

			// Use clipmap to clip pixels

			buf->lock(Ogre::HardwareBuffer::HBL_NORMAL);
			bufPtr = static_cast<Ogre::uint8*>((buf->getCurrentLock()).data);
			for( int i = 0; i < barHeight; ++i )
			{
				for( int j = 0; j < barWidth; ++j )
				{
					bufPtr += 3; // Not concerned with RBG at this point

					if(!mClipMap[(i * barWidth) + j])
						*bufPtr = 0;	// Set transparent

					bufPtr++;
				}
			}
			buf->unlock();
		}

		redraw();
	}

	void ProgressBar::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		mBarImage = mSkinType->getSkinElement(BAR)->getTextureImage();
		_processClipMap();
	}

	void ProgressBar::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void ProgressBar::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void ProgressBar::setText(Ogre::UTFString s)
	{
		setText(s,mCurrentFontName,mCurrentColourValue);
	}

	void ProgressBar::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}
}
