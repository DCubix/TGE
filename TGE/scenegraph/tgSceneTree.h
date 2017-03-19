#ifndef SCENE_TREE_H
#define SCENE_TREE_H

#include "tgNode.h"

class tgSceneTree {
public:
	tgSceneTree();
	~tgSceneTree();

	tgNode* getRoot();

	void addChild(tgNode *node);
	void removeChild(tgNode *node);
	
	tgNode* find(tgNodeID id);

	void render(tgRenderer *renderer);
	void update(float dt);

private:
	tgNode *m_root;
};

#endif