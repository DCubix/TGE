#ifndef SPRITE_H
#define SPRITE_H

#include "../ecs/tgComponent.h"
#include "../graphics/tgSpriteBatch.h"
#include "../math/tgMath.h"

#include <unordered_map>

class tgSpriteComponent : public tgComponent {
public:
	tgSpriteComponent()
		: m_texture(nullptr),
		m_origin(tgVector2(0.0f)),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f))
	{}
	tgSpriteComponent(tgTexture *texture)
		: m_texture(texture),
		m_origin(tgVector2(0.0f)),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f))
	{}
	tgSpriteComponent(tgTexture *texture, tgVector2 const& origin)
		: m_texture(texture),
		m_origin(origin),
		m_clipRectangle(tgVector4(0, 0, 1, 1)),
		m_color(tgVector4(1.0f))
	{}
	tgSpriteComponent(tgTexture *texture, tgVector2 const& origin, tgVector4 const& clip)
		: m_texture(texture),
		m_origin(origin),
		m_clipRectangle(clip),
		m_color(tgVector4(1.0f))
	{}

	tgTexture *getTexture() const { return m_texture; }
	void setTexture(tgTexture *tex) { m_texture = tex; }

	tgVector2 getOrigin() const { return m_origin; }
	void setOrigin(tgVector2 const& origin) { m_origin = origin; }

	tgVector4 getClipRectangle() const { return m_clipRectangle; }
	void setClipRectangle(tgVector4 const& clip) { m_clipRectangle = clip; }

	tgVector4 getColor() const { return m_color; }
	void setColor(tgVector4 const& color) { m_color = color; }

	void render(tgRenderer *renderer) override;
	void update(float dt) override;

	tgSpriteComponent* setup(int nOfRows, int nOfCols);
	tgSpriteComponent* addAnimation(std::string const& name, std::vector<int> const& frames);

	tgSpriteComponent* play(std::string const& name, float speed, bool loop);

private:
	tgTexture *m_texture;
	tgVector2 m_origin;
	tgVector4 m_clipRectangle;
	tgVector4 m_color;

	typedef struct tgAnimation {
		int frameIndex;
		float time;
		float speed;
		bool loop;
		std::vector<int> frames;
	} tgAnimation;

	int m_rows, m_columns;
	std::vector<tgVector4> m_clipRects;
	std::unordered_map<std::string, tgAnimation*> m_animations;
	std::string m_currentAnimation;
};

#endif