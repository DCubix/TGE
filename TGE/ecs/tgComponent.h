#ifndef COMPONENT_H
#define COMPONENT_H

#include "../core/tgTypes.h"

typedef struct tgComponent {
	friend class tgECS;

	tgComponent() : valid(true), owner(-1) {}
	virtual ~tgComponent() = default;

	uint getOwner() const { return owner; }
	bool isValid() const { return valid; }

	operator bool() {
		return valid;
	}

protected:
	uint owner;
	bool valid;
} tgComponent;

#endif
