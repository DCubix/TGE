#include "tgNode.h"

#include <algorithm>

#include "../core/tgUtil.h"

tgNode::tgNode() : m_parent(nullptr), m_id(INITIAL_NODE_ID) {}

tgNode::~tgNode() {
	for(tgNode *c : m_children) {
		tgDelete(c);
	}
	m_children.clear();
}

tgMatrix4 tgNode::getTransformationMatrix() {
	tgMatrix4 parent = tgMatrix4::identity();
	if(m_parent) {
		parent = m_parent->getTransformationMatrix();
	}
	return parent * m_transform.getTransformationMatrix();
}

tgVector3 tgNode::getWorldPosition() {
	return getParentTransformationMatrix() * m_transform.getLocalPosition();
}

tgVector3 tgNode::getWorldScaling() {
	return getParentTransformationMatrix() * m_transform.getLocalScaling();
}

tgQuaternion tgNode::getWorldRotation() {
	tgQuaternion rot(0, 0, 0, 1);
	if(m_parent) {
		rot = m_parent->getWorldRotation();
	}
	return rot * m_transform.getLocalRotation();
}

void tgNode::addChild(tgNode *node) {
	node->m_parent = this;
	node->m_id++;
	m_children.push_back(node);
}

void tgNode::removeChild(tgNode *node) {
	auto pos = std::find(m_children.begin(), m_children.end(), node);
	if(pos != m_children.end()) {
		m_children.erase(pos);
	}
}

void tgNode::reparent(tgNode *parent) {
	if(m_parent == nullptr) {
		if(parent != nullptr) {
			parent->addChild(this);
			tgVector3 WtoL = m_transform.getLocalPosition() - parent->getWorldPosition();
			m_transform.setLocalPosition(WtoL);
		}
	} else {
		if(parent == nullptr) {
			tgVector3 LtoW = m_transform.getLocalPosition() + m_parent->getWorldPosition();
			m_parent->removeChild(this);
			m_parent = nullptr;
			m_transform.setLocalPosition(LtoW);
		} else {
			m_parent->removeChild(this);
			m_parent = nullptr;
			reparent(parent);
		}
	}
}

tgNode* tgNode::find(tgNodeID id) {
	if(m_id == id) {
		return this;
	} else {
		for(tgNode *c : m_children) {
			tgNode *found = c->find(id);
			if(found) { return found; }
		}
	}
	return nullptr;
}

void tgNode::render(tgRenderer *renderer) {
	onRender(renderer);
	for(tgNode *node : m_children) {
		node->onRender(renderer);
	}
}

void tgNode::update(float dt) {
	onUpdate(dt);
	for(tgNode *node : m_children) {
		node->onUpdate(dt);
	}
}

tgMatrix4 tgNode::getParentTransformationMatrix() {
	tgMatrix4 parent = tgMatrix4::identity();
	if(m_parent) {
		parent = m_parent->getTransformationMatrix();
	}
	return parent;
}
