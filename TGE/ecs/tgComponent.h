#ifndef COMPONENT_H
#define COMPONENT_H

#include "../core/tgTypes.h"

typedef struct tgComponent {
	friend class tgECS;

	tgComponent() = default;
	virtual ~tgComponent() = default;

	uint getOwner() const { return owner; }
	uint getID() const { return id; }

	float getRenderingOrder() const { return renderingOrder; }
	void setRenderingOrder(float v) { renderingOrder = v; }

	operator bool() {
		return owner != -1;
	}

protected:
	uint owner, id;
	float renderingOrder;
} tgComponent;

#endif
