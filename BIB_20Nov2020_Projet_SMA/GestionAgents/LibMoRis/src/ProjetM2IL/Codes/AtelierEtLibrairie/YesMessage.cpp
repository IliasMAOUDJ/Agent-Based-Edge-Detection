#include "Agent.h"
#include "TypeTreeNode.h"
#include "YesMessage.h"

TypeTreeNode * YesMessage::yesmessagetreenode = NULL;

// Accessing
set<Agent *> YesMessage::getSubscribers() {
	set<Agent *> subscribers;
	YesMessage::getTypetreenode()->getAgents(subscribers);
	return subscribers;
}

string YesMessage::getType() {
	return YesMessage::getTypetreenode()->getType();
}

TypeTreeNode * YesMessage::getTypetreenode() {
	if (!yesmessagetreenode) {
		yesmessagetreenode = new TypeTreeNode();
		yesmessagetreenode->initialize(Message::getTypetreenode(), "YesMessage");
		Message::getTypetreenode()->addChildNode(YesMessage::getTypetreenode());
	}
	return yesmessagetreenode;
}

// Broadcasting
void YesMessage::broadcast(Message * message) {
	set<Agent *> subscribers = YesMessage::getSubscribers();
	for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
		if (Agent::exist(*it)) {
			message->sendTo(*it);
		}
	}
	delete message;
}

// Constructing
YesMessage::YesMessage():HolonMessage() {
	if (!yesmessagetreenode) {
		yesmessagetreenode = new TypeTreeNode();
		yesmessagetreenode->initialize(Message::getTypetreenode(), "YesMessage");
		Message::getTypetreenode()->addChildNode(YesMessage::getTypetreenode());
	}
	this->setSender(NULL);
}

// Destructing
YesMessage::~YesMessage() {
}

// Sending
void YesMessage::sendTo(Agent * receiver) {
	YesMessage * newMessage = new YesMessage(*this);
	receiver->saveMessage(newMessage);
}

// Testing
bool YesMessage::isA(string type) {
	return YesMessage::getTypetreenode()->isA(type);
}
