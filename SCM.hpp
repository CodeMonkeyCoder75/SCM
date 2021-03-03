#ifndef SCM_HPP
#define SCM_HPP

#include <stdio.h>
#include <iostream>
#include "Engine/OpenAL/al.h"
#include "Engine/OpenAL/alc.h"

#include "Music.hpp"

#pragma once
SCM_Export class SCM {
	public:
		bool CInit();
		bool CClose();
		ALCdevice* SCM_Device;
		ALCcontext* SCM_Context;
};

#pragma once
SCM_Export class SCM_Source : public SCM_Music {
	public:
		void Play(const ALuint& _Source);
		void PlayMusic();
		bool CInit(const ALsizei& _Count);
		ALuint SetSource(const ALuint& _Source);
		void GetSource();
		void DeleteSource(const ALsizei& _Count);
		ALuint _SCM_Source_;
		ALuint _SCM_Buffer_;
};

#endif