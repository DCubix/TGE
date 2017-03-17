#ifndef VERTEX_FMT_H
#define VERTEX_FMT_H

#include <vector>

class tgVertexFormat {
public:
	enum tgAttributeType {
		tgATTR_POSITION = 0,
		tgATTR_NORMAL,
		tgATTR_TANGENT,
		tgATTR_TEXCOORD,
		tgATTR_COLOR,
		tgATTR_FLOAT,
		tgATTR_FLOAT3,
		tgATTR_FLOAT4
	};

	tgVertexFormat(int vertexStructSize);

	void append(int indexOffset, tgAttributeType type_, bool normalized);
	void append(tgAttributeType type_, bool normalized);

	void bind();

	bool isEmpty() const { return m_attributes.size() == 0; }
private:
	typedef struct tgAttribute {
		int index, components;
		bool normalized;
		tgAttributeType attr_type;
	} tgAttribute;

	std::vector<tgAttribute> m_attributes;

	int m_structSize;
	int m_index;

	int getAttrSize(tgAttributeType t);
};

#endif // VERTEX_FMT_H
