#include "QuickGUIProgressBar.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String ProgressBar::BACKGROUND = "background";
	const Ogre::String ProgressBar::BAR = "bar";
	const Ogre::String ProgressBar::CLIPMAP = "clipmap";

	void ProgressBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ProgressBar");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(BAR);
		d->defineSkinElement(CLIPMAP);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ProgressBar",d);
	}

	ProgressBarDesc::ProgressBarDesc(const Ogre::String& id) :
		WidgetDesc(id)
	{
		resetToDefault();
	}

	void ProgressBarDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		progressbar_fillDirection = PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		progressbar_layout = PROGRESSBAR_LAYOUT_HORIZONTAL;
		progressbar_clippingEdge = PROGRESSBAR_CLIP_LEFT_BOTTOM;
		progressbar_progress = 100;
		progressbar_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		widget_transparencyPicking = false;

		textDesc.resetToDefault();
	}

	void ProgressBarDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("VerticalTextAlignment",&progressbar_verticalTextAlignment);

		textDesc.serialize(b);

		b->IO("Progress",&progressbar_progress);
		b->IO("FillDirection",&progressbar_fillDirection);
		b->IO("Layout",&progressbar_layout);
		b->IO("ClippingEdge",&progressbar_clippingEdge);
	}

	ProgressBar::ProgressBar(const Ogre::String& name) :
		Widget(name),
		mText(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	ProgressBar::~ProgressBar()
	{
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);

		// Clean up all user defined event handlers.
		for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mProgressBarEventHandlers[index].begin(); it != mProgressBarEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}

		mOutputBarTexture.setNull();
	}

	void ProgressBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<ProgressBarDesc*>(mWidgetDesc);

		ProgressBarDesc* pd = dynamic_cast<ProgressBarDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = pd->textDesc;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		mDesc->textDesc.allottedWidth = pd->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mDesc->progressbar_verticalTextAlignment = pd->progressbar_verticalTextAlignment;
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

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

		mDesc->progressbar_clippingEdge = pd->progressbar_clippingEdge;
		mDesc->progressbar_fillDirection = pd->progressbar_fillDirection;
		mDesc->progressbar_layout = pd->progressbar_layout;
		setProgress(pd->progressbar_progress);
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
		return mDesc->progressbar_clippingEdge;
	}

	ProgressBarFillDirection ProgressBar::getFillDirection()
	{
		return mDesc->progressbar_fillDirection;
	}

	HorizontalTextAlignment ProgressBar::getHorizontalTextAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	ProgressBarLayout ProgressBar::setLayout()
	{
		return mDesc->progressbar_layout;
	}

	float ProgressBar::getProgress()
	{
		return mDesc->progressbar_progress;
	}

	Ogre::UTFString ProgressBar::getText()
	{
		return mText->getText();
	}

	float ProgressBar::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	VerticalTextAlignment ProgressBar::getVerticalTextAlignment()
	{
		return mDesc->progressbar_verticalTextAlignment;
	}

	void ProgressBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		brush->setTexture(mOutputBarTexture);
		Rect barRect = mClientDimensions;
		barRect.translate(mTexturePosition);
		brush->drawRectangle(barRect,UVRect(0,0,1,1));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		if(mText->empty())
			return;

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->progressbar_verticalTextAlignment)
		{
		case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
			yPos = mDesc->widget_dimensions.size.height - st->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM) - textHeight;
			break;
		case TEXT_ALIGNMENT_VERTICAL_CENTER:
			yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);
			break;
		case TEXT_ALIGNMENT_VERTICAL_TOP:
			yPos = st->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP);
			break;
		}

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.position.y = yPos;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void ProgressBar::setClippingEdge(ProgressBarClippingEdge e)
	{
		mDesc->progressbar_clippingEdge = e;

		setProgress(mDesc->progressbar_progress);
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
		mDesc->progressbar_fillDirection = d;

		setProgress(mDesc->progressbar_progress);
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

	void ProgressBar::setHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		redraw();
	}

	void ProgressBar::setLayout(ProgressBarLayout l)
	{
		mDesc->progressbar_layout = l;

		setProgress(mDesc->progressbar_progress);
	}

	void ProgressBar::setProgress(float percent)
	{
		if(percent > 100)
			percent = 100;
		else if(percent < 0)
			percent = 0;

		mDesc->progressbar_progress = percent;

		// Update Bar Output Texture to match percentage
		{
			// Determine the Rect to blit from the bar texture. All values are percentages
			Rect sourceArea;
			Rect destArea;

			if(mDesc->progressbar_layout == PROGRESSBAR_LAYOUT_HORIZONTAL)
			{
				// Left to right
				if(mDesc->progressbar_fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progressbar_progress / 100.0;
					destArea.size.height = 1.0;
				}
				// Right to left
				else
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progressbar_progress / 100.0;
					destArea.size.height = 1.0;
				}
			}
			else
			{
				// Bottom to top
				if(mDesc->progressbar_fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}

					destArea.position.x = 0;
					destArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progressbar_progress / 100.0;
				}
				// Top to Bottom
				else
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					
					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progressbar_progress / 100.0;
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

			if(mDesc->progressbar_progress <= 0.0)
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

	void ProgressBar::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		redraw();
	}

	void ProgressBar::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}

	void ProgressBar::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->progressbar_verticalTextAlignment = a;

		redraw();
	}

	void ProgressBar::updateClientDimensions()
	{
		Widget::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		redraw();
	}
}
