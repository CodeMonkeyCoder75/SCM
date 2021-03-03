#ifndef MUSIC_HPP
#define MUSIC_HPP

#define SCM_Export extern "C" __declspec(dllexport)
#define SCM_CPrintMsg printf
#define SCM_CPrintMsgToConsole(_msg) \
			SCM_CPrintMsg("%s\n", _msg)  \

#define SCM_CPrintErrMsgToConsole(_err)					\
			SCM_CPrintMsg("Error detected: %s\n", _err)	\

#define elif else if

#include "SFX.hpp"
#include "Engine/OpenAL/al.h"
#include "Engine/OpenAL/alc.h"
#include "Engine/libsndfile/sndfile.h"
#include <vector>

#pragma once
SCM_Export class SCM_Music : public SCM_SFX {
	public:
		ALuint AddMusicSource(const char* _FileName);
		void UpdateStream();
		ALuint getSource();
		bool RemoveMusicSource(const ALuint& _Source);
		ALuint SCM_Music_Source;
		static const int SCM_BUFFER_SAMPLES = 8192;
		static const int SCM_NUM_BUFFERS = 4;
		ALuint SCM_Buffers[SCM_NUM_BUFFERS];
		SNDFILE* SCM_File;
		SF_INFO SCM_FileInfo;
		short* SCM_Membuf;
		ALenum SCM_Format;
};

#endif