#include "pch.h"
#include "SCM.hpp"

bool SCM_Source::CInit(const ALsizei& _Count) {
	alGenSources(_Count, &_SCM_Source_);
	return false;
};

void SCM_Source::Play(const ALuint& _Source) {
	if (_Source != _SCM_Buffer_) {
		_SCM_Buffer_ = _Source;
		alSourcei(_SCM_Source_, AL_BUFFER, (ALint)_SCM_Buffer_);
	}

	alSourcePlay(_SCM_Source_);

	ALint state = AL_PLAYING;
	SCM_CPrintMsgToConsole("playing sound");
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR) {
		SCM_CPrintMsgToConsole("currently playing sound");
		alGetSourcei(_SCM_Source_, AL_SOURCE_STATE, &state);
	}
	SCM_CPrintMsgToConsole("done playing sound");
};

void SCM_Source::PlayMusic() {
	ALsizei i;

	alGetError();

	alSourceRewind(SCM_Music_Source);
	alSourcei(SCM_Music_Source, AL_BUFFER, 0);

	for (i = 0; i < SCM_NUM_BUFFERS; i++) {

		sf_count_t slen = sf_readf_short(SCM_File, SCM_Membuf, SCM_BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= SCM_FileInfo.channels * (sf_count_t)sizeof(short);
		alBufferData(SCM_Buffers[i], SCM_Format, SCM_Membuf, (ALsizei)slen, SCM_FileInfo.samplerate);
	};

	if (alGetError() != AL_NO_ERROR) {
		throw("Error buffering for playback");
	};

	alSourceQueueBuffers(SCM_Music_Source, i, SCM_Buffers);
	alSourcePlay(SCM_Music_Source);
	if (alGetError() != AL_NO_ERROR) {
		throw("Error starting playback");
	};
};

ALuint SCM_Source::SetSource(const ALuint& _Source) {
	_SCM_Source_ = _Source;
	return _SCM_Source_;
};

void SCM_Source::GetSource() {
	if (_SCM_Source_ == 0) {
		SCM_CPrintErrMsgToConsole("cannot get the source!");
	} else {
		SCM_CPrintMsg("successfully got the source: %c\n", (const char*)_SCM_Source_);
	};
};

void SCM_Source::DeleteSource(const ALsizei& _Count) {
	alDeleteSources(_Count, &_SCM_Source_);
};