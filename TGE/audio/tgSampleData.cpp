#include "tgSampleData.h"

#include "../core/stb_vorbis.h"
#include "../core/tgLog.h"

tgSampleData::tgSampleData(tgAssetData *data) {
	int chan, srate;
	short *sdata;
	int samples = stb_vorbis_decode_memory(data->data, data->size, &chan, &srate, &sdata);
	if (!sdata) {
		return;
	}
	
	m_channels = chan;
	m_sampleRate = srate;
	m_format = chan > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	m_data = std::vector<short>(sdata, sdata + samples);

	free(sdata);
}
