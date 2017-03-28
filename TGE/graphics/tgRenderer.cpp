#include "tgRenderer.h"

#include <sstream>

tgRenderer::tgRenderer(int screen_width, int screen_height) {
	m_projection = tgMatrix4::perspective(45.0f, float(screen_width) / float(screen_height), 0.01f, 100.0f);
	m_view = tgMatrix4::identity();
}

void tgRenderer::submit(tgMesh *mesh) {
	if (mesh) {
		m_meshes.push_back(mesh);
	}
}

void tgRenderer::begin() {
	for (tgMesh *mesh : m_meshes) {
		tgDelete(mesh);
	}
	m_meshes.clear();
}

void tgRenderer::clear(int flags) {
	glClear(flags);
}
