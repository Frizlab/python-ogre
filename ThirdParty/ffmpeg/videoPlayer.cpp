#include "videoPlayer.h"

#include <stdlib.h>
//#include <unistd.h>
#include <stdio.h>





//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------
// Original code by: juliusctw
// Modified by Hansel
//
// Modified by Dirso (4/21/7)
//	you should now put the clips in the Ogre's resource file
//  a few parameters changing to make it easy to configure
//	printf's replaced with Ogre log methods

// compile with:
// windows:
// g++ -o player cVideoPlayer.cpp -lavformat -lavcodec -lavutil -lz -I/c/code/iamar/irrlicht-1.2/include/ -L/c/code/iamar/irrlicht-1.2/bin/Win32-gcc/ -lIrrlicht
//
// linux:
// g++ -o player cVideoPlayer.cpp -lavformat -lavcodec -lavutil -lz -I../irrlicht/include/ -L../irrlicht -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

cVideoPlayer::cVideoPlayer(String mName)
{
    // Register all formats and codecs
    av_register_all();

    materialName = mName;

    state = stopped;
    streamOpen = false;
    actualFrame = 0;
    loop = false;
    replayCont = 0;
    seekFactor = 15;
    lastTime = 0;

    bytesRemaining=0;
    fFirstTime=true;
    firstTime = true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

cVideoPlayer::cVideoPlayer(String mName, String filename)
{
    // Register all formats and codecs
    av_register_all();

    materialName = mName;

	if (! open((char*)filename.c_str()))
    {
		Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Couldn't open the file: ")
			+ Ogre::String(filename));
        exit(1);
    }


	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("-*-* START Movie clip file loaded: ")
		+ Ogre::String(filename));

    state = stopped;
    streamOpen = false;
    actualFrame = 0;
    loop = false;
    replayCont = 0;
    seekFactor = 15;
    lastTime = 0;

    bytesRemaining=0;
    fFirstTime=true;
    firstTime = true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


cVideoPlayer::~cVideoPlayer()
{
    // Free the RGB image
    if (Buffer != NULL)
        delete [] Buffer;

    if (FrameRGB != NULL)
        av_free(FrameRGB);

    // Free the YUV frame
    if (Frame != NULL)
        av_free(Frame);

    // Close the codec
    if (CodecCtx != NULL)
        avcodec_close(CodecCtx);

    // Close the video file
    if (FormatCtx != NULL)
        av_close_input_file(FormatCtx);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------



bool cVideoPlayer::open(Ogre::String fileName)
{
	Ogre::String path = "";
	Ogre::ResourceGroupManager* mResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::String group = mResourceGroupManager->findGroupContainingResource(fileName);
	Ogre::FileInfoListPtr resourceList = mResourceGroupManager->listResourceFileInfo(group);
	Ogre::FileInfoList::iterator itr = resourceList->begin();
	for(;itr != resourceList->end(); itr++)
	{
		if(itr->basename.compare(fileName) == 0)
		{
			path = itr->archive->getName();
			break;
		}
	}
	if(path.compare("") == 0)
	{
		throw Ogre::Exception(7, "Unable to find " + fileName + ". Is it in the resources.cfg?",
			"cVideoPlayer::open");
	}	

	path += "/";
	path += fileName;

	actualFilename = (char*)path.c_str();

    // Open video file
    if (av_open_input_file(&FormatCtx, path.c_str(), NULL, 0, NULL)!=0)
        return false; // Couldn't open file

    // Retrieve stream information
    if (av_find_stream_info(FormatCtx)<0)
        return false; // Couldn't find stream information

    // Dump information about file onto standard error
    dump_format(FormatCtx, 0, path.c_str(), false);

    // Find the first video stream
    VideoStream=-1;
    for (int i=0; i<FormatCtx->nb_streams; i++)
        if (FormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
        {
            VideoStream=i;
            break;
        }
    if (VideoStream==-1)
        return false; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    CodecCtx=FormatCtx->streams[VideoStream]->codec;

    // Get the seconds/frame of this video stream
    SecondsPerFrame = (double)FormatCtx->streams[VideoStream]->r_frame_rate.den / FormatCtx->streams[VideoStream]->r_frame_rate.num;
	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Duration: ")
		+ Ogre::StringConverter::toString((Ogre::Real)FormatCtx->duration));
	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Seconds Per Frame: ")
		+ Ogre::StringConverter::toString((Ogre::Real)SecondsPerFrame));

    // Find the decoder for the video stream
    Codec=avcodec_find_decoder(CodecCtx->codec_id);
    if (Codec==NULL) {
    	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("cVideoPlayer::open -- Codec==NULL"));
        return false; // Codec not found
}
    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    if (Codec->capabilities & CODEC_CAP_TRUNCATED)
        CodecCtx->flags|=CODEC_FLAG_TRUNCATED;

    // Open codec
    if (avcodec_open(CodecCtx, Codec)<0) {
    	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("cVideoPlayer::open -- Unable to Open the Codec"));
        return false; // Could not open codec
	}
    // Allocate video frame
    Frame=avcodec_alloc_frame();

    // Allocate an AVFrame structure
    FrameRGB=avcodec_alloc_frame();
    if (FrameRGB==NULL) {
    	Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("cVideoPlayer::open -- FrameRGB == NULL"));
        return false;
	}
    // Determine required buffer size and allocate buffer
    NumBytes=avpicture_get_size(PIX_FMT_RGB32,
                                CodecCtx->width,
                                CodecCtx->height);
    Buffer=new uint8_t[NumBytes];

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    avpicture_fill((AVPicture *)FrameRGB, Buffer, PIX_FMT_RGB32,
                   CodecCtx->width, CodecCtx->height);


    // get the movie framerate
    framerate = FormatCtx->streams[VideoStream]->r_frame_rate.num;

    Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("cVideoPlayer::open -- Success"));

    return true;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::refresh(unsigned long time)
{
    static struct SwsContext *img_convert_ctx;

    if (state == playing)
    {
        if (time - lastTime > (SecondsPerFrame*1000))
         {
             
        if (GetNextFrame(FormatCtx, CodecCtx, VideoStream, Frame))
        {
            
// //             // Convert the image into YUV format that SDL uses
// //             if(img_convert_ctx == NULL) {
// //               int w = is->video_st->codec->width;
// //               int h = is->video_st->codec->height;
// //               img_convert_ctx = sws_getContext(w, h, 
// //                                 is->video_st->codec->pix_fmt, 
// //                                 w, h, dst_pix_fmt, SWS_BICUBIC, 
// //                                 NULL, NULL, NULL);
// //               if(img_convert_ctx == NULL) {
// //             	fprintf(stderr, "Cannot initialize the conversion context!\n");
// //             	exit(1);
// //                   }
// //             }
// //             sws_scale(img_convert_ctx, pFrame->data, 
// //                       pFrame->linesize, 0, 
// //                       is->video_st->codec->height, 
// //                       pict.data, pict.linesize);

            
// //           use for older versions ffmpeg           
// //             img_convert((AVPicture *)FrameRGB, PIX_FMT_RGB32, (AVPicture*)Frame,
// //                         CodecCtx->pix_fmt, CodecCtx->width, CodecCtx->height);

				img_convert_ctx = sws_getContext(
							CodecCtx->width,
							CodecCtx->height,
							CodecCtx->pix_fmt,
							CodecCtx->width,
							CodecCtx->height,
							PIX_FMT_RGBA32,
							SWS_BICUBIC,
							NULL, NULL, NULL);
				         
						 sws_scale(
							img_convert_ctx,
							Frame->data,
							Frame->linesize,
							0,
							CodecCtx->height,
							FrameRGB->data,
							FrameRGB->linesize);
                        
				sws_freeContext(img_convert_ctx);
                        
// 			Ogre::LogManager::getSingletonPtr()->logMessage(
// 				Ogre::String("Replay Num.: ")+ Ogre::StringConverter::toString(replayCont) +
// 				Ogre::String("  ::  Dumping Frame: ") + Ogre::StringConverter::toString(actualFrame) +
// 				Ogre::String("  ::  FrameRate: ") + Ogre::StringConverter::toString(framerate));

            // Dump the frame
            DumpFrame(FrameRGB, CodecCtx->width, CodecCtx->height);

            actualFrame++;
        }
        else
        {
            state = stopped;
            Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("-*-* END: Movie clip"));

        }

         lastTime = time;
         }
    }

    if (state == stopped)
    {
        actualFrame = 0;
        restartStream();

        // if looped then replay the movie
        if (loop)
        {
            replayCont ++;
            state = playing;
            streamOpen = true;
        }
    }

	return true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------



bool cVideoPlayer::play()
{
    if (state == stopped) replayCont++;
    state = playing;
    streamOpen = true;

    return true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::stop()
{
    state = stopped;

    return true;
};


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

bool cVideoPlayer::pause()
{
    state = paused;

    return true;
};


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::GetNextFrame(AVFormatContext *pFormatCtx,
                                AVCodecContext *pCodecCtx,
                                int videoStream,
                                AVFrame *pFrame)
{
    // First time we're called, set packet.data to NULL to indicate it
    // doesn't have to be freed
    if (fFirstTime)
    {
        fFirstTime=false;
        packet.data=NULL;
    }

    // Decode packets until we have decoded a complete frame
    while (true)
    {
        // Work on the current packet until we have decoded all of it
        while (bytesRemaining > 0)
        {
            // Decode the next chunk of data
            bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame,
                                              &frameFinished, rawData, bytesRemaining);

            // Was there an error?
            if (bytesDecoded < 0)
            {
				Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Error while decoding frame"));
                return false;
            }

            bytesRemaining-=bytesDecoded;
            rawData+=bytesDecoded;

            // Did we finish the current frame? Then we can return
            if (frameFinished)
                return true;
        }

        // Read the next packet, skipping all packets that aren't for this
        // stream
        do
        {
            // Free old packet
            if (packet.data!=NULL)
                av_free_packet(&packet);

            // Read new packet
            if (av_read_packet(pFormatCtx, &packet)<0)
                goto loop_exit;
        }
        while (packet.stream_index!=videoStream);

        bytesRemaining=packet.size;
        rawData=packet.data;
    }

loop_exit:

    // Decode the rest of the last frame
    bytesDecoded=avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,
                                      rawData, bytesRemaining);

    // Free last packet
    if (packet.data!=NULL)
        av_free_packet(&packet);

    return frameFinished!=0;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------



bool cVideoPlayer::DumpFrame(AVFrame *pFrame,
                             int width,
                             int height)
{
    // buil the video texture
    buildTexture(pFrame, width, height);


    return true;

}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

TexturePtr cVideoPlayer::getVideoTexture()
{
    return texture;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


bool cVideoPlayer::restartStream()
{
    //Close stream
    if ( FormatCtx && streamOpen)
    {
        streamOpen = false;

        //if (av_open_input_file(&FormatCtx, actualFilename, NULL, 0, NULL)!=0) return false; // Couldn't open file
        goToFrame(0);
    }
	return true;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


void cVideoPlayer::setLoop(bool l)
{
    loop = l;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


void cVideoPlayer::goToSecond(int numSecond)
{
    restartStream();

    state = manual;


    goToFrame((int)framerate*numSecond);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------




bool cVideoPlayer::goToFrame(int numFrame)
{
    // seek the movie
    if (numFrame < seekFactor) av_seek_frame(FormatCtx, VideoStream, numFrame, AVSEEK_FLAG_ANY);
    else
    {
        av_seek_frame(FormatCtx, VideoStream, numFrame-seekFactor, AVSEEK_FLAG_ANY);

        // advance to the real selected frame
        for (int i = 0; i < seekFactor; i++) GetNextFrame(FormatCtx, CodecCtx, VideoStream, Frame);
    }

    actualFrame = numFrame;

	Ogre::LogManager::getSingletonPtr()->logMessage(
		Ogre::String("Replay Num.: ")+ Ogre::StringConverter::toString(replayCont) +
		Ogre::String("  ::  Dumping Frame: ") + Ogre::StringConverter::toString(actualFrame) +
		Ogre::String("  ::  FrameRate: ") + Ogre::StringConverter::toString(framerate));

    return true;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


float cVideoPlayer::getFrameRate()
{
    return framerate;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------


int videoCont = 0;

void cVideoPlayer::buildTexture(AVFrame *pFrame,
                                int width,
                                int height)
{
    if (firstTime)
    {
        firstTime = false;
        videoCont++;

        texture = TextureManager::getSingleton().createManual(
                                 "videoTexture" + StringConverter::toString(videoCont), // name
                                 ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                 TEX_TYPE_2D,      // type
                                 width, height,         // width & height
                                 0,                // number of mipmaps
                                 PF_BYTE_BGR,     // pixel format
                                 TU_DEFAULT);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for textures updated very often (e.g. each frame)

        // Create a material using the texture
        material = MaterialManager::getSingleton().create(materialName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        material->getTechnique(0)->getPass(0)->createTextureUnitState("videoTexture" + StringConverter::toString(videoCont));
    }

    // Get the pixel buffer
    pixelBuffer = texture->getBuffer();
//     // Lock the pixel buffer and get a pixel box
//     pixelBuffer->lock (HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
//     const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

//     pImage = pFrame->data[0];
//     pDest = (uint8*)(pixelBox.data);

//     // copy the image data
//     memcpy(pDest, pImage, width*height*4);

//     // Unlock the pixel buffer
//     pixelBuffer->unlock();
    
    pixelBuffer->blitFromMemory(Ogre::PixelBox(width, height, 1, Ogre::PF_BYTE_BGRA, pFrame->data[0]));
}
