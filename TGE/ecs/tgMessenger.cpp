#include "tgMessenger.h"

#include "tgECS.h"

#include <algorithm>

void tgMessenger::sendMessage(tgEntity *from, std::string const& text, tgMessagePriority priority, void * userdata) {
	tgMessage msg;
	msg.sender = from;
	msg.text = text;
	msg.priority = priority;
	msg.userdata = userdata;
	m_messages.push_back(msg);
}

void tgMessenger::processQueue(tgECS* manager) {
	if (!m_messages.empty()) {
		std::sort(m_messages.begin(), m_messages.end(),
			[](tgMessage const& a, tgMessage const& b) {
			return a.priority > b.priority;
		});

		while (!m_messages.empty()) {
			
			m_messages.pop_front();
		}
	}
}

void tgMessenger::reset() {
	m_messages.clear();
}
