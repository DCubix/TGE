#include "tgVertexFormat.h"

#include "tgGL.h"
#include <cstdint>

tgVertexFormat::tgVertexFormat(int vertexStructSize)
	: m_structSize(vertexStructSize),
	m_index(0) {}

int tgVertexFormat::getAttrSize(tgAttributeType t) {
	switch(t) {
		case tgATTR_FLOAT: return 1;
		case tgATTR_COLOR:
		case tgATTR_FLOAT4: return 4;
		case tgATTR_NORMAL:
		case tgATTR_POSITION:
		case tgATTR_TANGENT:
		case tgATTR_FLOAT3: return 3;
		case tgATTR_FLOAT2:
		case tgATTR_TEXCOORD: return 2;
	}
	return 0;
}

void tgVertexFormat::append(int indexOffset, tgAttributeType type_, bool normalized) {
	tgAttribute attr;
	attr.index = m_index + indexOffset;
	attr.attr_type = type_;
	attr.components = getAttrSize(type_);
	attr.normalized = normalized;

	m_attributes.push_back(attr);
	m_index++;
}

void tgVertexFormat::append(tgAttributeType type_, bool normalized) {
	append(0, type_, normalized);
}

void tgVertexFormat::bind() {
	int off = 0;
	for(tgAttribute attr : m_attributes) {
		glEnableVertexAttribArray(attr.index);

		GLvoid const* off_ptr = static_cast<char const*> (0) + off;
		glVertexAttribPointer(attr.index, attr.components, GL_FLOAT, attr.normalized, m_structSize, off_ptr);
		off += attr.components * 4;
	}
}
