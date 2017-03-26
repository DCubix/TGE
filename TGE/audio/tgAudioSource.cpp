#include "tgAudioSource.h"

#include "../core/tgUtil.h"
#include "../core/tgLog.h"

tgAudioSource::tgAudioSource(tgAudioBuffer *buffer)
	: m_paused(true),
	m_playing(false),
	m_3d(false),
	m_pan(0.0f),
	m_pitch(1.0f),
	m_volume(1.0f)
{
	m_buffer = buffer->getBuffer();
	ALCall(alGenSources(1, &m_source));
	ALCall(alSourcei(m_source, AL_BUFFER, m_buffer));
}

tgAudioSource::~tgAudioSource() {
	if (m_source) {
		alDeleteSources(1, &m_source);
	}
}

void tgAudioSource::play() {
	ALCall(alSourcePlay(m_source));
}

void tgAudioSource::pause() {
	ALCall(alSourcePause(m_source));
}

void tgAudioSource::stop() {
	ALCall(alSourceStop(m_source));
}

void tgAudioSource::update() {
	ALenum state;
	ALCall(alGetSourcei(m_source, AL_SOURCE_STATE, &state));
	m_playing = state == AL_PLAYING;
	m_paused = state == AL_PAUSED;

	ALCall(alSourcef(m_source, AL_GAIN, m_volume));
	ALCall(alSourcef(m_source, AL_PITCH, m_pitch));
	ALCall(alSourcei(m_source, AL_LOOPING, m_loop ? AL_TRUE : AL_FALSE));
	if (!m_3d) {
		ALCall(alSource3f(m_source, AL_POSITION, m_pan, 0.0f, 0.0f));
		ALCall(alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f));
	} else {
		ALCall(alSource3f(m_source, AL_POSITION, m_position.x(), m_position.y(), m_position.z()));
	}
}
