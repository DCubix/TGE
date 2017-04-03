#ifndef COMPONENT_H
#define COMPONENT_H

#include "../core/tgTypes.h"

typedef struct tgComponent {
	friend class tgECS;

	tgComponent() = default;
	virtual ~tgComponent() = default;

	uint getID() const { return id; }
	bool isValid() const { return valid; }

	operator bool() {
		return valid;
	}

protected:
	uint id;
	bool valid;
} tgComponent;

#endif
