#include "tgSampleData.h"

#include "../core/stb_vorbis.h"
#include "../core/tgLog.h"

tgSampleData::tgSampleData(std::istream& stream) {
	if (stream.bad()) {
		tgLog::log("The input stream is not open.");
		return;
	}
	stream.seekg(0, std::ios::end);
	std::istream::pos_type pos = stream.tellg();

	byte *data = new byte[pos];
	stream.seekg(0, std::ios::beg);
	stream.read((char*)(data), pos);

	int chan, srate;
	short *sdata;
	int samples = stb_vorbis_decode_memory(data, int(pos), &chan, &srate, &sdata);
	if (!sdata) {
		return;
	}
	
	m_channels = chan;
	m_sampleRate = srate;
	m_format = chan > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	m_data = std::vector<short>(sdata, sdata + samples);

	free(sdata);
	delete[] data;
}
