#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include "tgSampleData.h"

class tgAudioBuffer {
public:
	tgAudioBuffer(tgSampleData& data);
	~tgAudioBuffer();

	ALuint getBuffer() const { return m_buffer; }
private:
	ALuint m_buffer;
};

#endif
