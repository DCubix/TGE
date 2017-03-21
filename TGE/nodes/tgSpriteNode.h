#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H

#include "../graphics/tgTexture.h"
#include "../scenegraph/tgNode.h"

class tgSpriteNode : public tgNode {
public:
	tgSpriteNode(tgTexture *tex);
	~tgSpriteNode();

	tgTexture *getTexture() const { return m_texture; }
	void setTexture(tgTexture *tex) { m_texture = tex; }

	tgVector2 getOrigin() const { return m_origin; }
	void setOrigin(tgVector2 const& origin) { m_origin = origin; }

	tgVector4 getClipRectangle() const { return m_clipRectangle; }
	void setClipRectangle(tgVector4 const& clip) { m_clipRectangle = clip; }

	void onRender(tgRenderer *ren) override;

protected:
	tgTexture *m_texture;
	tgVector2 m_origin;
	tgVector4 m_clipRectangle;
};

#endif
