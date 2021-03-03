#ifndef CSCM_H
#define CSCM_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "Engine/libsndfile/sndfile.h"
#include "Engine/OpenAL/al.h"
#include "Engine/OpenAL/alc.h"
#include "Engine/OpenAL/alext.h"
#include <inttypes.h>
#include <math.h>

//Macros
#define SCM_Export  extern __declspec(dllexport)
#define SCM_CPrintErrorMsgToConsole SCM_CGetErr
#define SCM_CPrintMsgToConsole SCM_CPrint
#define SCM_CPrintMsg printf
#define SCM_IsPlaying AL_PLAYING
#define elif else if

#define SCM_CPrintVariableInMemory(X) \
        SCM_CPrintMsg("This variable is located at: %p\n", X) \

//Integers
typedef int SCM_I8;
typedef int32_t SCM_I32;
typedef int64_t SCM_I64;

//Signed integers
typedef signed int SCM_SI8;
typedef signed int SCM_SI32;
typedef signed int SCM_SI64;

//Unsigned integers
typedef unsigned int SCM_USI8;
typedef unsigned int SCM_USI32;
typedef unsigned int SCM_USI64;

//Const integers
typedef const int SCM_CNTI8;
typedef const int SCM_CNTI32;
typedef const int SCM_CNTI64;

//Short
typedef short SCM_SHRT;

//Signed short
typedef signed short SCM_SSHRT;

//Unsigned short
typedef unsigned short SCM_USSHRT;

//Const short
typedef const short SCM_CNTSHRT;

//Double
typedef double SCM_DBL;

//Const double
typedef const double SCM_CNTDBL;

//Long
typedef long SCM_Long;

//Const long
typedef const SCM_Long SCM_CNTLong;

//Double long
typedef long long SCM_DBLELong;

//Boolean
#ifndef __cplusplus
typedef int SCM_Bool;
#else
typedef bool SCM_Bool;
#endif
#define SCM_ERROR -1
#define SCM_FALSE 0
#define SCM_TRUE 1
#define SCM_ZERO SCM_FALSE
#define SCM_EXIT SCM_FALSE

//Char
typedef char SCM_CHR;

//Signed char
typedef signed char SCM_SCHR;

//Unsigned char
typedef unsigned char SCM_USCHR;

//Const char
typedef const char SCM_CNTCHR;

//Void
typedef void SCM_Void;

//Data
typedef const void* SCM_Data;

//File
typedef FILE SCM_File;

//End

#ifdef __cplusplus
extern "C" {
#endif
    //Type definition
    typedef struct _SCM_Music {
        SCM_CNTCHR* mFilePath;
        ALsizei mCount;
        ALuint mSource;
    } _SCM_Music;

    typedef _SCM_Music SCM_Music; //Aka Music buffer

    //Type definition
    typedef struct _SCM_SFX {
        SCM_CNTCHR* mFilePath;
        ALuint mCount;
        ALuint mSource;
        ALenum err, format;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* membuf;
        sf_count_t num_frames;
        ALsizei num_bytes;
    } _SCM_SFX;

    typedef _SCM_SFX SCM_SFX; //Aka SFX buffer

    //Type definition
    typedef struct _SCM_Source {
        SCM_Bool mMusicIsPlaying;
        ALsizei mNumSource;
        ALuint mBuffer;
        ALuint mSource;
        ALuint mIsPlaying;
    } _SCM_Source;

    typedef _SCM_Source SCM_Source; //Aka source manager

    //Type definition
    typedef struct _SCM_Device {
        ALCdevice* mDevice;
        const ALCchar* mName;
        ALboolean mEnumeration;
        ALCcontext* mContext;
    } _SCM_Device;

    typedef _SCM_Device SCM_Device; //Aka device manager

    SCM_Export SCM_Void SCM_SetupDevice(SCM_Device* SCM_Device); //Aka SCM::CInit()
    SCM_Export SCM_Bool SCM_ClearDevice(SCM_Device* SCM_Device); //Aka SCM::CClose()
    SCM_Export SCM_Void SCM_SetupSource(SCM_Source* SCM_Source, SCM_CNTI32 SCM_Count); //Aka SCM_Source::CInit()
    SCM_Export SCM_Bool SCM_ClearSource(SCM_Source* SCM_Source); //Aka SCM_Source::DeleteSource()
    SCM_Export SCM_Void SCM_SetupMUS(SCM_Music* SCM_Music, SCM_CNTI32 SCM_Count); //Aka SCM_Music::CInit()
    SCM_Export SCM_Bool SCM_ClearMUS(SCM_Music* SCM_Music); //Aka SCM_Music::CClose()
    SCM_Export SCM_Void SCM_SetupSFX(SCM_SFX* SCM_SFX, SCM_CNTI32 SCM_Count); //Aka SCM_SFX::CInit()
    SCM_Export SCM_Bool SCM_ClearSFX(SCM_SFX* SCM_SFX); //Aka SCM_SFX::CClose()

    SCM_Export SCM_Void SCM_UnloadMUS(SCM_Music* SCM_Music); //Aka SCM_Music::RemoveMusicSource()
    SCM_Export SCM_Void SCM_UnloadSFX(SCM_SFX* SCM_SFX); //Aka SCM_SFX::RemoveSoundEffect()

    SCM_Export SCM_Void SCM_LoadMUS(SCM_Music* SCM_Music, SCM_CNTCHR* mFile); //Aka SCM_Music::AddMusicSource()
    SCM_Export SCM_Void SCM_LoadSFX(SCM_SFX* SCM_SFX, SCM_CNTCHR* mFile); //Aka SCM_SFX::AddSoundEffect()

    /*SCM_Export SCM_Source SCM_SetMusicToSource(SCM_Music* SCM_Music);
    SCM_Export SCM_Source SCM_SetSFXToSource(SCM_SFX* SCM_SFX);*/

    SCM_Export SCM_Void SCM_PlayMUS(SCM_Source* SCM_Source, SCM_Music* SCM_Music); //Aka SCM_Source::Play()
    SCM_Export SCM_Void SCM_PlaySFX(SCM_Source* SCM_Source, SCM_SFX* SCM_SFX); //Aka SCM_Source::PlayMusic()

    SCM_Export SCM_CNTCHR* SCM_SetMusicFromPath(SCM_Music* SCM_Music, SCM_CNTCHR* mFile);
    SCM_Export SCM_CNTCHR* SCM_GetMusicFromPath(SCM_Music* SCM_Music);
    SCM_Export SCM_CNTCHR* SCM_SetSFXFromPath(SCM_SFX* SCM_SFX, SCM_CNTCHR* mFile);
    SCM_Export SCM_CNTCHR* SCM_GetSFXFromPath(SCM_SFX* SCM_SFX);
    SCM_Export SCM_CNTCHR* SCM_PrintMusicFromPath(SCM_Music* SCM_Music);
    SCM_Export SCM_CNTCHR* SCM_PrintSFXFromPath(SCM_SFX* SCM_SFX);

    SCM_Export SCM_CNTCHR* SCM_CPrint(SCM_CNTCHR* _msg);
    SCM_Export SCM_CNTCHR* SCM_CGetErr(SCM_CNTCHR* _err);

#ifdef __cplusplus
};
#endif
#endif