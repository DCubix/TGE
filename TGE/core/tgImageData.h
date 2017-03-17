#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <iostream>
#include <fstream>
#include <vector>

using byte = unsigned char;

class tgImageData {
public:
	typedef struct Byte4 {
		byte r, g, b, a;
	} Byte4;

	tgImageData(int width, int height);
	tgImageData(std::ifstream &stream);

	std::vector<byte>& getData() { return m_data; }
	tgImageData getSubData(int x, int y, int w, int h);

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	tgImageData& flipX();
	tgImageData& flipY();

	bool saveAs(std::ofstream &stream);

private:
	int m_width, m_height;
	std::vector<byte> m_data;

	Byte4 getByteAt(int x, int y);
};

#endif // IMAGE_DATA_H
