#include "tgAudioSystem.h"

#include "../core/tgLog.h"
#include "../core/tgUtil.h"

tgAudioSystem::tgAudioSystem() : m_initialized(false) {
	const char* devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	m_alc_device = alcOpenDevice(devicename);
	if (!m_alc_device) {
		tgLog::log("Could not open device.");
	} else {
		m_alc_context = alcCreateContext(m_alc_device, 0);
		if (!m_alc_context) {
			tgLog::log("Could not create context.");
		} else {
			alcMakeContextCurrent(m_alc_context);
			m_initialized = true;
		}
	}
}

tgAudioSystem::~tgAudioSystem() {
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_alc_context);
	alcCloseDevice(m_alc_device);
	m_initialized = false;
}

void tgAudioSystem::cleanup() {
	std::vector<tgAudioSource*> rem;
	for (tgAudioSource *src : m_sources) {
		if (!src->isPlaying() && !src->isPaused()) {
			rem.push_back(src);
		}
	}
	for (tgAudioSource *src : rem) {
		src->stop();
		m_sources.erase(std::remove(m_sources.begin(), m_sources.end(), src), m_sources.end());
		tgDelete(src);
	}
}

tgAudioSource* tgAudioSystem::play(tgAudioBuffer* buff) {
	tgAudioSource* src = new tgAudioSource(buff);
	src->play();
	m_sources.push_back(src);
	return src;
}

void tgAudioSystem::update() {
	for (tgAudioSource *src : m_sources) {
		src->update();
	}
	cleanup();
}
