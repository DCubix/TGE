#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>

#include <AL/al.h>

using byte = unsigned char;

class tgSampleData {
public:
	tgSampleData(std::istream &stream);

	ALenum getFormat() const { return m_format; }
	short* getData() { return &m_data[0]; }
	std::size_t getDataSize() { return m_data.size() * sizeof(short); }
	int getSampleRate() const { return m_sampleRate; }
private:
	std::vector<short> m_data;
	int m_sampleRate;
	int m_channels;
	ALenum m_format;
};

#endif
