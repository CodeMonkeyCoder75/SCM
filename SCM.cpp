#include "pch.h"
#include "SCM.hpp"

bool SCM::CInit() {
	SCM_Device = alcOpenDevice(nullptr);
	if (!SCM_Device) {
		throw("Failed to get the sound device!");
	};
	SCM_Context = alcCreateContext(SCM_Device, nullptr);
	if (!SCM_Context) {
		throw("Failed to set the sound context!");
	};
	if (!alcMakeContextCurrent(SCM_Context)) {
		throw("Failed to make the context current!");
	};
	const ALCchar* naam = nullptr;
	if (alcIsExtensionPresent(SCM_Device, "ALC_ENUMERATE_ALL_EXT")) {
		naam = alcGetString(SCM_Device, ALC_ALL_DEVICES_SPECIFIER);
	};
	if (!naam || alcGetError(SCM_Device) != ALC_NO_ERROR) {
		naam = alcGetString(SCM_Device, ALC_DEVICE_SPECIFIER);
	};
	SCM_CPrintMsg("Current sound device has opened");
};

bool SCM::CClose() {
	alcCloseDevice(SCM_Device);
	return false;
};