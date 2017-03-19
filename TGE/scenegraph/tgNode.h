#ifndef NODE_H
#define NODE_H

#include "../math/tgMath.h"
#include "../math/tgTransform.h"
#include "../math/tgQuaternion.h"
#include "../graphics/tgRenderer.h"
#include <vector>

#define INITIAL_NODE_ID 0xDED

typedef uint32_t tgNodeID;

class tgNode {
	using tgNodeList = std::vector<tgNode*>;
public:
	tgNode();
	virtual ~tgNode();

	tgMatrix4 getTransformationMatrix();

	tgVector3 getWorldPosition();
	tgVector3 getWorldScaling();
	tgQuaternion getWorldRotation();

	tgTransform& getTransform() { return m_transform; }
	tgNodeID getID() const { return m_id; }

	void addChild(tgNode *node);
	void removeChild(tgNode *node);

	const tgNode *getParent() const { return m_parent; }
	void reparent(tgNode *parent);

	const tgNodeList& getChildren() const { return m_children; }
	tgNode* find(tgNodeID id);

	void render(tgRenderer *renderer);
	void update(float dt);

	virtual void onRender(tgRenderer *renderer) {}
	virtual void onUpdate(float dt) {}

protected:
	tgNode *m_parent;
	tgNodeList m_children;

	tgNodeID m_id; // This is for finding it easily.
	tgTransform m_transform;

	tgMatrix4 getParentTransformationMatrix();
};

#endif