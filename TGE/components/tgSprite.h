#ifndef SPRITE_H
#define SPRITE_H

#include "../ecs/tgComponent.h"
#include "../math/tgMath.h"
#include "../graphics/tgTexture.h"

#include <unordered_map>

typedef struct tgAnimation {
	int frameIndex;
	float time;
	float speed;
	bool loop;
	std::vector<int> frames;
} tgAnimation;

typedef struct tgSprite : public tgComponent {
	friend class tgSpriteSystem;

	tgSprite()
		: m_texture(nullptr),
		m_origin(tgVector2(0.0f)),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f)),
		m_renderOrder(0)
	{}
	tgSprite(tgTexture *texture)
		: m_texture(texture),
		m_origin(tgVector2(0.0f)),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f)),
		m_renderOrder(0)
	{}
	tgSprite(tgTexture *texture, tgVector2 const& origin)
		: m_texture(texture),
		m_origin(origin),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f)),
		m_renderOrder(0)
	{}
	tgSprite(tgTexture *texture, tgVector2 const& origin, tgVector4 const& clip)
		: m_texture(texture),
		m_origin(origin),
		m_clipRectangle(clip),
		m_color(tgVector4(1.0f)),
		m_renderOrder(0)
	{}

	tgTexture *getTexture() const { return m_texture; }
	void setTexture(tgTexture *tex) { m_texture = tex; }

	tgVector2 getOrigin() const { return m_origin; }
	void setOrigin(tgVector2 const& origin) { m_origin = origin; }

	tgVector4 getClipRectangle() const { return m_clipRectangle; }
	void setClipRectangle(tgVector4 const& clip) { m_clipRectangle = clip; }

	tgVector4 getColor() const { return m_color; }
	void setColor(tgVector4 const& color) { m_color = color; }

	int getRenderOrder() const { return m_renderOrder; }
	void setRenderOrder(int v) { m_renderOrder = v; }

	tgSprite* setup(int nOfRows, int nOfCols);
	tgSprite* addAnimation(std::string const& name, std::vector<int> const& frames);

	tgSprite* play(std::string const& name, float speed, bool loop);

protected:
	tgTexture *m_texture;
	tgVector2 m_origin;
	tgVector4 m_clipRectangle;
	tgVector4 m_color;

	int m_rows, m_columns;
	std::vector<tgVector4> m_clipRects;
	std::unordered_map<std::string, tgAnimation*> m_animations;
	std::string m_currentAnimation;

	int m_renderOrder;
} tgSprite;

#endif
