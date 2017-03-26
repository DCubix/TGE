#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "tgAudioBuffer.h"
#include "../math/tgMath.h"

class tgAudioSource {
public:
	tgAudioSource(tgAudioBuffer *buffer);
	~tgAudioSource();

	void play();
	void pause();
	void stop();

	tgVector3& getPosition() { return m_position; }
	void setPosition(tgVector3 const& pos) { m_position = pos; }

	float getVolume() const { return m_volume; }
	void setVolume(float val) { m_volume = val; }

	float getPan() const { return m_pan; }
	void setPan(float val) { m_pan = val; }

	float getPitch() const { return m_pitch; }
	void setPitch(float val) { m_pitch = val; }

	bool is3D() const { return m_3d; }
	void set3D(bool val) { m_3d = val; }

	bool isLoop() const { return m_loop; }
	void setLoop(bool val) { m_loop = val; }

	bool isPlaying() const { return m_playing; }
	bool isPaused() const { return m_playing; }

	void update();

private:
	ALuint m_source;
	ALuint m_buffer;

	tgVector3 m_position;
	float m_volume, m_pan, m_pitch;
	bool m_3d, m_paused, m_playing, m_loop;
};

#endif
