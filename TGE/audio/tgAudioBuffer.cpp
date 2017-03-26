#include "tgAudioBuffer.h"

#include "../core/tgUtil.h"

tgAudioBuffer::tgAudioBuffer(tgSampleData& data) {
	ALCall(alGenBuffers(1, &m_buffer));
	ALCall(alBufferData(m_buffer, data.getFormat(), (ALvoid*)data.getData(), data.getDataSize(), data.getSampleRate()));
}

tgAudioBuffer::~tgAudioBuffer() {
	if (m_buffer) {
		alDeleteBuffers(1, &m_buffer);
	}
}
