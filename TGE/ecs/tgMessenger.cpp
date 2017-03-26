#include "tgMessenger.h"

#include <algorithm>

void tgMessenger::sendMessage(
	tgEntity from, tgEntity to,
	std::string const& text,
	tgMessagePriority priority, void *userdata)
{
	tgMessage msg;
	msg.sender = from;
	msg.addressee = to;
	msg.text = text;
	msg.priority = priority;
	msg.userdata = userdata;
	m_messages.push_back(msg);
}

void tgMessenger::sendMessage(tgEntity from, std::string const& text, tgMessagePriority priority, void * userdata) {
	sendMessage(from, TG_INVALID_ENTITY, text, priority, userdata);
}

void tgMessenger::processQueue(tgComponentManager* manager) {
	if (!m_messages.empty()) {
		std::sort(m_messages.begin(), m_messages.end(),
			[](tgMessage const& a, tgMessage const& b) {
			return a.priority > b.priority;
		});

		while (!m_messages.empty()) {
			for (tgComponent *component : manager->getAllComponents()) {
				tgMessage &msg = m_messages.front();
				if (msg.addressee != TG_INVALID_ENTITY) {
					if (component->getOwner() == msg.addressee) {
						component->receive(msg);
					}
				} else {
					component->receive(msg);
				}
			}
			m_messages.pop_front();
		}
	}
}
