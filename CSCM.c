#include "pch.h"
#include "CSCM.h"

SCM_Export SCM_Void SCM_SetupDevice(SCM_Device* SCM_Device) {

	SCM_Device->mDevice = alcOpenDevice(NULL);

	if (!SCM_Device) {
		SCM_CPrintErrorMsgToConsole("Failed to get the sound device!");
	}
	else {
		SCM_CPrintMsgToConsole("Got the sound device!");
		SCM_Device->mContext = alcCreateContext(SCM_Device->mDevice, NULL);
		if (!SCM_Device->mContext) {
			SCM_CPrintErrorMsgToConsole("Failed to set the sound context!");
		}
		else {
			SCM_CPrintMsgToConsole("Got the sound context!");
		};
		if (!alcMakeContextCurrent(SCM_Device->mContext)) {
			SCM_CPrintErrorMsgToConsole("Failed to make the context current!");
		}
		else {
			SCM_CPrintMsgToConsole("made the sound context current!");
		};
	};

	SCM_Device->mName = alcGetString(SCM_Device->mDevice, ALC_DEFAULT_DEVICE_SPECIFIER);
	SCM_CPrintMsg("The name of the default device is: %s\n", SCM_Device->mName);

	SCM_CPrintMsgToConsole("Sound device has opened");
};

SCM_Export SCM_Bool SCM_ClearDevice(SCM_Device* SCM_Device) {
	alcCloseDevice(SCM_Device->mDevice);
	alcDestroyContext(SCM_Device->mContext);
	alcMakeContextCurrent(NULL);
	SCM_Device->mDevice = SCM_ZERO;
	SCM_Device->mContext = SCM_ZERO;
	SCM_Device->mName = SCM_ZERO;
	return SCM_EXIT;
};

SCM_Export SCM_Void SCM_SetupSource(SCM_Source* SCM_Source, SCM_CNTI32 SCM_Count) {
	SCM_CPrintMsgToConsole("Setting up the source manager!");
	alGenSources((ALsizei)SCM_Count, (ALuint*)SCM_Source->mSource);
	SCM_CPrintMsgToConsole("Source manager has been created!");
	SCM_Source->mNumSource = (ALsizei)SCM_Count;
};

SCM_Export SCM_Bool SCM_ClearSource(SCM_Source* SCM_Source) {
	alDeleteSources((ALsizei)SCM_Source->mNumSource, (ALuint*)SCM_Source->mSource);
	SCM_Source->mNumSource = SCM_ZERO;
	SCM_Source->mSource = SCM_ZERO;
	return SCM_EXIT;
};

SCM_Export SCM_Void SCM_SetupMUS(SCM_Music* SCM_Music, SCM_CNTI32 SCM_Count) {
	SCM_CPrintMsgToConsole("Setting up the music buffer!");
	alGenBuffers((ALsizei)SCM_Count, (ALuint*)SCM_Music->mSource);
	SCM_CPrintMsgToConsole("Music buffer has been created!");
	SCM_Music->mCount = (ALsizei)SCM_Count;
};

SCM_Export SCM_Bool SCM_ClearMUS(SCM_Music* SCM_Music) {
	SCM_UnloadMUS(SCM_Music);
	SCM_Music->mFilePath = SCM_ZERO;
	SCM_Music->mCount = (ALsizei)SCM_ZERO;
	SCM_Music->mSource = (ALuint)SCM_ZERO;
	return SCM_EXIT;
};

SCM_Export SCM_Void SCM_SetupSFX(SCM_SFX* SCM_SFX, SCM_CNTI32 SCM_Count) {
	SCM_CPrintMsgToConsole("Setting up the SFX buffer!");
	alGenBuffers((ALsizei)SCM_Count, (ALuint*)SCM_SFX->mSource);
	SCM_CPrintMsgToConsole("SFX buffer has been created!");
	SCM_SFX->mCount = (ALsizei)SCM_Count;
};

SCM_Export SCM_Bool SCM_ClearSFX(SCM_SFX* SCM_SFX) {
	SCM_UnloadSFX(SCM_SFX);
	SCM_SFX->mFilePath = SCM_ZERO;
	SCM_SFX->mCount = (ALsizei)SCM_ZERO;
	SCM_SFX->mSource = (ALuint)SCM_ZERO;
	return SCM_EXIT;
};

SCM_Export SCM_Void SCM_UnloadMUS(SCM_Music* SCM_Music) {
	alDeleteBuffers((ALsizei)SCM_Music->mCount, (ALuint*)SCM_Music->mSource);
};

SCM_Export SCM_Void SCM_UnloadSFX(SCM_SFX* SCM_SFX) {
	alDeleteBuffers((ALsizei)SCM_SFX->mCount, (ALuint*)SCM_SFX->mSource);
};

SCM_Export SCM_Void SCM_LoadMUS(SCM_Music* mBuffer, SCM_CNTCHR* mFile) {
	SCM_CPrintMsgToConsole("You have to implement this function you bitch!");
};

SCM_Export SCM_Void SCM_LoadSFX(SCM_SFX* mBuffer, SCM_CNTCHR* mFile) {
	mBuffer->mFilePath = mFile;
	mBuffer->sndfile = sf_open(mFile, SFM_READ, &mBuffer->sfinfo);
	if (!mBuffer->sndfile) {
		fprintf(stderr, "Could not open audio in %s: %s\n", mFile, sf_strerror(mBuffer->sndfile));
	}
	else {
		SCM_CPrintMsgToConsole("Audio is opened!");
	};

	if (mBuffer->sfinfo.frames < 1 || mBuffer->sfinfo.frames >((sf_count_t)(2147483647 / sizeof(short)) / mBuffer->sfinfo.channels)) {
		fprintf(stderr, "Bad sample count in %s (%I64d)\n", mFile, mBuffer->sfinfo.frames);
		sf_close(mBuffer->sndfile);
	} else {
		SCM_CPrintMsgToConsole("The Samples are all good!");
	};

	mBuffer->format = AL_NONE;
	if (mBuffer->sfinfo.channels == 1)
		mBuffer->format = AL_FORMAT_MONO16;
	elif(mBuffer->sfinfo.channels == 2)
		mBuffer->format = AL_FORMAT_STEREO16;
	elif(mBuffer->sfinfo.channels == 3) {
		if (sf_command(mBuffer->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			mBuffer->format = AL_FORMAT_BFORMAT2D_16;
	} elif(mBuffer->sfinfo.channels == 4) {
		if (sf_command(mBuffer->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			mBuffer->format = AL_FORMAT_BFORMAT3D_16;
	};
	if (!mBuffer->format) {
		fprintf(stderr, "Unsupported channel count: %d\n", mBuffer->sfinfo.channels);
		sf_close(mBuffer->sndfile);
	} else {
		SCM_CPrintMsgToConsole("Channel count is all supported!");
	};

	(mBuffer->membuf = (short*)(malloc((size_t)(mBuffer->sfinfo.frames * mBuffer->sfinfo.channels) * sizeof(short))));

	mBuffer->num_frames = sf_readf_short(mBuffer->sndfile, mBuffer->membuf, mBuffer->sfinfo.frames);
	if (mBuffer->num_frames < 1) {
		free(mBuffer->membuf);
		sf_close(mBuffer->sndfile);
		fprintf(stderr, "Failed to read samples in %s (%I64d)\n", mFile, mBuffer->num_frames);
	}
	else { //Muggenziften
		SCM_CPrintMsgToConsole("Successfully read the samples!");
	};
	mBuffer->num_bytes = (ALsizei)(mBuffer->num_frames * mBuffer->sfinfo.channels) * (ALsizei)sizeof(short);

	//alGenBuffers(mBuffer->mCount, mBuffer->mSource);
	alBufferData(mBuffer->mSource, mBuffer->format, mBuffer->membuf, mBuffer->num_bytes, mBuffer->sfinfo.samplerate);

	free(mBuffer->membuf);
	sf_close(mBuffer->sndfile);

	mBuffer->err = alGetError();
	if (mBuffer->err != AL_NO_ERROR) {
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(mBuffer->err));
	} else {
		SCM_CPrintMsgToConsole("Everything went well!");
	};
};

SCM_Export SCM_Void SCM_PlayMUS(SCM_Source* SCM_Source, SCM_Music* SCM_Music) {
	SCM_CPrintMsgToConsole("You have to implement this function you bitch!");
	SCM_Source->mMusicIsPlaying = SCM_TRUE;
};

SCM_Export SCM_Void SCM_PlaySFX(SCM_Source* SCM_Source, SCM_SFX* SCM_SFX) {

	SCM_PrintSFXFromPath(SCM_SFX);

	alSourcei(SCM_Source->mSource, AL_BUFFER, SCM_SFX->mSource);

	SCM_CPrintMsg("Ready to play the SFX file!\nNow playing!!\n");
	alSourcePlay(SCM_Source->mSource);
	SCM_CPrintMsgToConsole("playing sound");
	alGetSourcei(SCM_Source->mSource, AL_SOURCE_STATE, &SCM_Source->mIsPlaying);

	while (SCM_Source->mIsPlaying == SCM_IsPlaying) {
		//SCM_CPrintMsgToConsole("currently playing sound");
		alGetSourcei(SCM_Source->mSource, AL_SOURCE_STATE, &SCM_Source->mIsPlaying);
	};
	SCM_CPrintMsgToConsole("done playing sound");
};

SCM_Export SCM_CNTCHR* SCM_SetMusicFromPath(SCM_Music* SCM_Music, SCM_CNTCHR* mFile) {
	if (SCM_Music != SCM_ZERO || !SCM_Music) {
		SCM_CPrintErrorMsgToConsole("The Music object already has data in it!");
	}
	else {
		SCM_CPrintMsgToConsole("Copying the data to the Music object");
		SCM_Music->mFilePath = mFile;
		SCM_CPrintMsgToConsole("Successfully copied the data!");
	};
	return SCM_Music->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_GetMusicFromPath(SCM_Music* SCM_Music) {
	return SCM_Music->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_SetSFXFromPath(SCM_SFX* SCM_SFX, SCM_CNTCHR* mFile) {
	if (SCM_SFX != SCM_ZERO) {
		SCM_CPrintErrorMsgToConsole("The SFX object already has data in it!");
	}
	else {
		SCM_CPrintMsgToConsole("Copying the data to the SFX object");
		SCM_SFX->mFilePath = mFile;
		SCM_CPrintMsgToConsole("Successfully copied the data!");
	};
	return SCM_SFX->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_GetSFXFromPath(SCM_SFX* SCM_SFX) {
	return SCM_SFX->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_PrintMusicFromPath(SCM_Music* SCM_Music) {
	SCM_CPrintMsg("This music file is located at: %s\n", SCM_GetMusicFromPath(SCM_Music));
	return SCM_Music->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_PrintSFXFromPath(SCM_SFX* SCM_SFX) {
	SCM_CPrintMsg("This sfx file is located at: %s\n", SCM_GetSFXFromPath(SCM_SFX));
	return SCM_SFX->mFilePath;
};

SCM_Export SCM_CNTCHR* SCM_CPrint(SCM_CNTCHR* _msg) {
	SCM_CPrintMsg("%s\n", _msg);
	return _msg;
};

SCM_Export SCM_CNTCHR* SCM_CGetErr(SCM_CNTCHR* _err) {
	SCM_CPrintMsg("Error: %s\n", _err);
	return _err;
};