#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

#include "tgAudioSource.h"
#include "tgAudioBuffer.h"

class tgAudioSystem {
public:
	void create();
	void destroy();

	tgAudioSource* play(tgAudioBuffer* buff);

	void update();

	bool isInitialized() const { return m_initialized; }

	static tgAudioSystem* getSingleton();
private:
	tgAudioSystem();
	static tgAudioSystem *m_instance;

	std::vector<tgAudioSource*> m_sources;

	ALCdevice *m_alc_device;
	ALCcontext *m_alc_context;

	bool m_initialized;

	void cleanup();
};

#endif
