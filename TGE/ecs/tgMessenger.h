#ifndef MESSENGER_H
#define MESSENGER_H

#include "tgComponentManager.h"

#include <string>
#include <deque>

enum tgMessagePriority {
	tgPRIORITY_LOW = -1,
	tgPRIORITY_NORMAL = 0,
	tgPRIORITY_HIGH = 1
};

typedef struct tgMessage {
	tgEntity sender;
	tgEntity addressee;
	tgMessagePriority priority;
	std::string text;
	void *userdata;
} tgMessage;

class tgMessenger {
public:
	tgMessenger() {}

	void sendMessage(
		tgEntity from, tgEntity to,
		std::string const& text,
		tgMessagePriority priority = tgPRIORITY_NORMAL,
		void *userdata = nullptr);
	void sendMessage(
		tgEntity from,
		std::string const& text,
		tgMessagePriority priority = tgPRIORITY_NORMAL,
		void *userdata = nullptr);

	void processQueue(tgComponentManager *manager);
	void reset();
private:
	std::deque<tgMessage> m_messages;
};

#endif
