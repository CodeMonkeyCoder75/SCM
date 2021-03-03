#include "pch.h"
#include "SCM.hpp"
#include "Engine/OpenAL/alext.h"
#include "Engine/libsndfile/sndfile.h"
#include <inttypes.h>

bool SCM_SFX::CInit() {
	SCM_SFX_SoundEffectBuffers.clear();
	return false;
};

ALuint SCM_SFX::AddSoundEffect(const char* _FileName) {
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	sndfile = sf_open(_FileName, SFM_READ, &sfinfo);
	if (!sndfile) {
		fprintf(stderr, "Could not open audio in %s: %s\n", _FileName, sf_strerror(sndfile));
		return 0;
	};
	if (sfinfo.frames < 1 || sfinfo.frames >((sf_count_t)(2147483647 / sizeof(short)) / sfinfo.channels)) {
		fprintf(stderr, "Bad sample count in %s (%s)\n", _FileName, sfinfo.frames);
		sf_close(sndfile);
		return 0;
	};

	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	elif (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	elif (sfinfo.channels == 3) {
		if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	} elif (sfinfo.channels == 4) {
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	} if (!format) {
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	};

	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1) {
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%s)\n", _FileName, num_frames);
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR) {
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	SCM_SFX_SoundEffectBuffers.push_back(buffer);

	return buffer;
};

bool SCM_SFX::RemoveSoundEffect(const ALuint& _Buffer) {
	auto it = SCM_SFX_SoundEffectBuffers.begin();
	while (it != SCM_SFX_SoundEffectBuffers.end()) {
		if (*it == _Buffer) {
			alDeleteBuffers(1, &*it);

			it = SCM_SFX_SoundEffectBuffers.erase(it);

			return true;
		}
		else {
			++it;
		}
	}
	return false;
};

bool SCM_SFX::CClose() {
	alDeleteBuffers(SCM_SFX_SoundEffectBuffers.size(), SCM_SFX_SoundEffectBuffers.data());
	SCM_SFX_SoundEffectBuffers.clear();
	return false;
};