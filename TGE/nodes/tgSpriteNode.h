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

	void onRender(tgRenderer *ren) override;

private:
	tgTexture *m_texture;
};

#endif
