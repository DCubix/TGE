#include "tgSpriteNode.h"

#include "../graphics/tgSpriteRenderer.h"

tgSpriteNode::tgSpriteNode(tgTexture *tex) : m_texture(tex) {
	m_clipRectangle = tgVector4(0, 0, 1, 1);
	m_origin = tgVector2(0.5f);
}

tgSpriteNode::~tgSpriteNode() {
	tgDelete(m_texture);
	tgNode::~tgNode();
}

void tgSpriteNode::onRender(tgRenderer * ren) {
	tgSpriteRenderer *renderer = dynamic_cast<tgSpriteRenderer*>(ren);
	if(renderer && m_texture) {
		float rot = m_transform.getLocalRotation().toEuler().z();
		tgVector2 scl = m_transform.getLocalScaling().xy();
		renderer->draw(m_texture,
			m_clipRectangle,
			tgVector4(getWorldPosition().xy(), 
						float(m_texture->getWidth()) * scl.x(),
						float(m_texture->getHeight()) * scl.y()),
			m_origin, rot
		);
	}
}
