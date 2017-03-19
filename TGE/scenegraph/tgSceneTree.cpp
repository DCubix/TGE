#include "tgSceneTree.h"
#include "../core/tgUtil.h"

tgSceneTree::tgSceneTree() : m_root(nullptr) {}


tgSceneTree::~tgSceneTree() {
	tgDelete(m_root);
}

tgNode* tgSceneTree::getRoot() {
	if(!m_root) {
		m_root = new tgNode();
	}
	return m_root;
}

void tgSceneTree::addChild(tgNode *node) {
	getRoot()->addChild(node);
}

void tgSceneTree::removeChild(tgNode *node) {
	getRoot()->removeChild(node);
}

tgNode *tgSceneTree::find(tgNodeID id) {
	return getRoot()->find(id);
}

void tgSceneTree::render(tgRenderer * renderer) {
	getRoot()->render(renderer);
}

void tgSceneTree::update(float dt) {
	getRoot()->update(dt);
}
