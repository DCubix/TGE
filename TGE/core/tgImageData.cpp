#include "tgImageData.h"

#include "stb.h"
#include "tgLog.h"

tgImageData::tgImageData(int width, int height)
	: m_width(width),
	m_height(height) {
	m_data.reserve(width * height * 4);
}

tgImageData::tgImageData(std::istream &stream) {
	if(stream.bad()) {
		tgLog::log("The input stream is not open.");
		return;
	}
	stream.seekg(0, std::ios::end);
	std::istream::pos_type pos = stream.tellg();

	byte *data = new byte[pos];
	stream.seekg(0, std::ios::beg);
	stream.read((char*) (data), pos);

	int comp;
	byte *dat = stbi_load_from_memory(data, int(pos), &m_width, &m_height, &comp, STBI_rgb_alpha);
	if(!dat) {
		tgLog::log("The image failed to load.");
		return;
	}

	const unsigned data_size = m_width * m_height * comp;
	const unsigned final_data_size = m_width * m_height * 4;
	m_data.resize(final_data_size);
	for (unsigned i = 0; i < final_data_size; i+= 4) {
		m_data[i + 0] = dat[i + 0];
		m_data[i + 1] = dat[i + 1];
		m_data[i + 2] = dat[i + 2];
		if (data_size == final_data_size) {
			m_data[i + 3] = dat[i + 3];
		} else {
			m_data[i + 3] = (byte)255;
		}
	}

	stbi_image_free(dat);
	delete[] data;
}

tgImageData tgImageData::getSubData(int x, int y, int w, int h) {
	tgImageData ndata(w, h);
	for(int py = y; py < y + h; py++) {
		for(int px = x; px < x + w; px++) {
			Byte4 b = getByteAt(px, py);
			ndata.m_data.push_back(b.r);
			ndata.m_data.push_back(b.g);
			ndata.m_data.push_back(b.b);
			ndata.m_data.push_back(b.a);
		}
	}
	return ndata;
}

tgImageData& tgImageData::flipX() {
	std::vector<byte> ndata;
	ndata.reserve(m_width * m_height * 4);

	for(int y = 0; y < m_height; y++) {
		for(int x = m_width - 1; x >= 0; x--) {
			Byte4 b = getByteAt(x, y);
			ndata.push_back(b.r);
			ndata.push_back(b.g);
			ndata.push_back(b.b);
			ndata.push_back(b.a);
		}
	}
	m_data = std::vector<byte>(ndata);
	return *this;
}

tgImageData& tgImageData::flipY() {
	std::vector<byte> ndata;
	ndata.reserve(m_width * m_height * 4);

	for(int y = m_height - 1; y >= 0; y--) {
		for(int x = 0; x < m_width; x++) {
			Byte4 b = getByteAt(x, y);
			ndata.push_back(b.r);
			ndata.push_back(b.g);
			ndata.push_back(b.b);
			ndata.push_back(b.a);
		}
	}
	m_data = std::vector<byte>(ndata);
	return *this;
}

bool tgImageData::saveAs(std::ostream &stream) {
	if(stream.bad()) {
		tgLog::log("The output stream is not open.");
		return false;
	}
	int len;
	byte *png = stbi_write_png_to_mem(m_data.data(), m_width * 4, m_width, m_height, 4, &len);
	if(png) {
		stream.write(reinterpret_cast<const char*> (png), len);
		STBI_FREE(png);
	}
	return true;
}

tgImageData::Byte4 tgImageData::getByteAt(int x, int y) {
	Byte4 b;
	int i = (y * m_width + x) * 4;
	b.r = m_data[i + 0];
	b.g = m_data[i + 1];
	b.b = m_data[i + 2];
	b.a = m_data[i + 3];
	return b;
}
