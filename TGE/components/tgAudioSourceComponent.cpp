#include "tgAudioSourceComponent.h"

void tgAudioSourceComponent::add(std::string const& name, tgAudioBuffer* buff) {
	if (buff) {
		m_library[name] = buff;
	}
}

tgAudioSource* tgAudioSourceComponent::play(std::string const& name) {
	auto pos = m_library.find(name);
	if (pos != m_library.end()) {
		return getAudioSystem()->play(m_library[name]);
	}
	return nullptr;
}
