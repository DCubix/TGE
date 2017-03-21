#ifndef SPRITE_H
#define SPRITE_H

#include "../ecs/tgComponent.h"
#include "../graphics/tgTexture.h"
#include "../math/tgMath.h"

class tgSpriteComponent : public tgComponent {
public:
	tgSpriteComponent()
		: m_texture(nullptr), m_origin(tgVector2(0.0f)), m_clipRectangle(tgVector4(0, 0, 1, 1))
	{}
	tgSpriteComponent(tgTexture *texture)
		: m_texture(texture), m_origin(tgVector2(0.0f)), m_clipRectangle(tgVector4(0, 0, 1, 1))
	{}
	tgSpriteComponent(tgTexture *texture, tgVector2 const& origin)
		: m_texture(texture), m_origin(origin), m_clipRectangle(tgVector4(0, 0, 1, 1))
	{}
	tgSpriteComponent(tgTexture *texture, tgVector2 const& origin, tgVector4 const& clip)
		: m_texture(texture), m_origin(origin), m_clipRectangle(clip)
	{}

	tgTexture *getTexture() const { return m_texture; }
	void setTexture(tgTexture *tex) { m_texture = tex; }

	tgVector2 getOrigin() const { return m_origin; }
	void setOrigin(tgVector2 const& origin) { m_origin = origin; }

	tgVector4 getClipRectangle() const { return m_clipRectangle; }
	void setClipRectangle(tgVector4 const& clip) { m_clipRectangle = clip; }

private:
	tgTexture *m_texture;
	tgVector2 m_origin;
	tgVector4 m_clipRectangle;
};

#endif