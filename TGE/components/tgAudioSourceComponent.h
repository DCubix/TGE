#ifndef AUDIO_SOURCE_COMPONENT_H
#define AUDIO_SOURCE_COMPONENT_H

#include "../ecs/tgComponent.h"
#include "../audio/tgAudioBuffer.h"
#include "../audio/tgAudioSource.h"
#include "../audio/tgAudioSystem.h"

#include <unordered_map>
#include <string>

class tgAudioSourceComponent : public tgComponent {
public:
	tgAudioSourceComponent(tgAudioSystem* sys) : m_audioSystem(sys) {}

	void add(std::string const& name, tgAudioBuffer* buff);
	tgAudioSource* play(std::string const& name);

	tgAudioSystem* getAudioSystem() { return m_audioSystem; }
private:
	tgAudioSystem *m_audioSystem;
	std::unordered_map<std::string, tgAudioBuffer*> m_library;
};

#endif
