#include "pch.h"
#include "Music.hpp"
#include "Engine/OpenAL/alext.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

ALuint SCM_Music::AddMusicSource(const char* _FileName) {
	alGenSources(1, &SCM_Music_Source);
	alGenBuffers(SCM_NUM_BUFFERS, SCM_Buffers);

	std::size_t frame_size;

	SCM_File = sf_open(_FileName, SFM_READ, &SCM_FileInfo);
	if (!SCM_File) {
		throw("could not open provided music file -- check path");
	}

	if (SCM_FileInfo.channels == 1)
		SCM_Format = AL_FORMAT_MONO16;
	else if (SCM_FileInfo.channels == 2)
		SCM_Format = AL_FORMAT_STEREO16;
	else if (SCM_FileInfo.channels == 3)
	{
		if (sf_command(SCM_File, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			SCM_Format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (SCM_FileInfo.channels == 4)
	{
		if (sf_command(SCM_File, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			SCM_Format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!SCM_Format) {
		sf_close(SCM_File);
		SCM_File = NULL;
		throw("Unsupported channel count from file");
	};

	frame_size = ((size_t)SCM_BUFFER_SAMPLES * (size_t)SCM_FileInfo.channels) * sizeof(short);
	SCM_Membuf = static_cast<short*>(malloc(frame_size));
	return SCM_Buffers;

};

void SCM_Music::UpdateStream() {
	ALint processed, state;

	alGetError();

	alGetSourcei(SCM_Music_Source, AL_SOURCE_STATE, &state);
	alGetSourcei(SCM_Music_Source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR) {
		throw("error checking music source state");
	}

	while (processed > 0) {
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(SCM_Music_Source, 1, &bufid);
		processed--;

		slen = sf_readf_short(SCM_File, SCM_Membuf, SCM_BUFFER_SAMPLES);
		if (slen > 0) {
			slen *= SCM_FileInfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, SCM_Format, SCM_Membuf, (ALsizei)slen,
				SCM_FileInfo.samplerate);
			alSourceQueueBuffers(SCM_Music_Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR) {
			throw("error buffering music data");
		};
	}

	if (state != AL_PLAYING && state != AL_PAUSED) {
		ALint queued;

		alGetSourcei(SCM_Music_Source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return;

		alSourcePlay(SCM_Music_Source);
		if (alGetError() != AL_NO_ERROR) {
			throw("error restarting music playback");
		};
	};
};

ALuint SCM_Music::getSource() {
	return SCM_Music_Source;
};

bool SCM_Music::RemoveMusicSource(const ALuint& _Source) {
	alDeleteSources(SCM_Music_Source, &_Source);

	if (SCM_File)
		sf_close(SCM_File);

	SCM_File = nullptr;

	free(SCM_Membuf);

	alDeleteBuffers(SCM_NUM_BUFFERS, SCM_Buffers);

	return false;
};