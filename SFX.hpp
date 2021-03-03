#ifndef SFX_HPP
#define SFX_HPP

#include "Engine/OpenAL/al.h"
#include "Engine/OpenAL/alc.h"
#include <vector>

#pragma once
SCM_Export class SCM_SFX {
	public:
		ALuint AddSoundEffect(const char* _FileName);
		bool RemoveSoundEffect(const ALuint& _Buffer);
		bool CInit();
		bool CClose();
		std::vector<ALuint> SCM_SFX_SoundEffectBuffers;
 };

#endif