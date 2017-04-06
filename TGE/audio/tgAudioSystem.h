#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

#include "tgAudioSource.h"
#include "tgAudioBuffer.h"

class tgAudioSystem {
public:
	tgAudioSystem();
	~tgAudioSystem();

	tgAudioSource* play(tgAudioBuffer *buff);
	void update();

	bool isInitialized() const { return m_initialized; }

private:
	std::vector<tgAudioSource*> m_sources;

	ALCdevice *m_alc_device;
	ALCcontext *m_alc_context;

	bool m_initialized;

	void cleanup();
};

#endif
