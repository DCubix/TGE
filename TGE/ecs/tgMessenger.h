#ifndef MESSENGER_H
#define MESSENGER_H

#include <string>
#include <deque>

enum tgMessagePriority {
	tgPRIORITY_LOW = -1,
	tgPRIORITY_NORMAL = 0,
	tgPRIORITY_HIGH = 1
};

class tgECS;
class tgEntity;

typedef struct tgMessage {
	tgEntity* sender;
	tgMessagePriority priority;
	std::string text;
	void *userdata;
} tgMessage;

class tgMessenger {
public:
	tgMessenger() {}

	void sendMessage(
		tgEntity* from,
		std::string const& text,
		tgMessagePriority priority = tgPRIORITY_NORMAL,
		void *userdata = nullptr);

	void processQueue(tgECS *manager);
	void reset();

private:
	std::deque<tgMessage> m_messages;
};

#endif
