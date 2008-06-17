#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__


#include "Ogre.h"

// // ffmpeg includes
// #include <ffmpeg/avcodec.h>
// #include <ffmpeg/avformat.h>
extern "C" {
#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

using namespace Ogre;


typedef signed int s32;



class cVideoPlayer
{
private:

    unsigned long lastTime;

    double SecondsPerFrame;

    AVFormatContext *FormatCtx;
    int             VideoStream;
    AVCodecContext  *CodecCtx;
    AVCodec         *Codec;
    AVFrame         *Frame;
    AVFrame         *FrameRGB;
    int             NumBytes;
    uint8_t         *Buffer;

    bool GetNextFrame(AVFormatContext *pFormatCtx,
                      AVCodecContext *pCodecCtx,
                      int videoStream,
                      AVFrame *pFrame);

    bool DumpFrame(AVFrame *pFrame, int width, int height);

    s32* p;
    s32* pimage;

    enum cState {playing, paused, stopped, manual};
    cState state;
    bool streamOpen;
    char* actualFilename;
    int actualFrame;
    bool loop;
    int replayCont;
    float framerate;
    int seekFactor;


    AVPacket packet;
    int      bytesRemaining;
    uint8_t  *rawData;
    bool     fFirstTime;
    int             bytesDecoded;
    int             frameFinished;

    bool firstTime;

    uint8* pImage;
    uint8* pDest;



    TexturePtr texture;
    MaterialPtr material;
    HardwarePixelBufferSharedPtr pixelBuffer;

    String materialName;


    void buildTexture(AVFrame *pFrame,
                             int width,
                             int height);

public:
    // constructors
    cVideoPlayer(String mName);
    cVideoPlayer(String mName, String filename);

    // destructor
    ~cVideoPlayer();

    // functions to open, play, and stop the media
    bool open(Ogre::String fileName);

    // refresh the video
    bool refresh(unsigned long time);

    // play the video
    bool play();

    // stop the video
    bool stop();

    // pause the video
    bool pause();

    // get the video texture
    TexturePtr getVideoTexture();

    // close the video stream
    bool restartStream();

    // set looped
    void setLoop(bool l);

    // go to selected second
    void goToSecond(int numSecond);

    // get selected frame
    bool goToFrame(int numFrame);

    // get the movie framerate
    float getFrameRate();

    // get the seek factor (15 by default)
    int getSeekFactor();

    // set the seek factor
    void setSeekFactor(int sf);

	bool isPlaying() { return state == playing; }
};

#endif
