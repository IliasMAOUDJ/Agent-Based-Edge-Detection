#include "Agent.h"
#include "ReadyMessage.h"
#include "TypeTreeNode.h"

TypeTreeNode * ReadyMessage::readymessagetreenode = NULL;

// Accessing
set<Agent *> ReadyMessage::getSubscribers() {
	set<Agent *> subscribers;
	ReadyMessage::getTypetreenode()->getAgents(subscribers);
	return subscribers;
}
string ReadyMessage::getType() {
	return ReadyMessage::getTypetreenode()->getType();
}
TypeTreeNode * ReadyMessage::getTypetreenode() {
	if (!readymessagetreenode) {
		readymessagetreenode = new TypeTreeNode();
		readymessagetreenode->initialize(Message::getTypetreenode(), "ReadyMessage");
		Message::getTypetreenode()->addChildNode(ReadyMessage::getTypetreenode());
	}
	return readymessagetreenode;
}

// Broadcasting
void ReadyMessage::broadcast(Message * message) {
	set<Agent *> subscribers = ReadyMessage::getSubscribers();
	for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
		if (Agent::exist(*it)) {
			message->sendTo(*it);
		}
	}
	delete message;
}

// Constructing
ReadyMessage::ReadyMessage():HolonMessage() {
	if (!readymessagetreenode) {
		readymessagetreenode = new TypeTreeNode();
		readymessagetreenode->initialize(Message::getTypetreenode(), "ReadyMessage");
		Message::getTypetreenode()->addChildNode(ReadyMessage::getTypetreenode());
	}
	this->setSender(NULL);
}

// Destructing
ReadyMessage::~ReadyMessage() {
}

// Sending
void ReadyMessage::sendTo(Agent * receiver) {
	ReadyMessage * newMessage = new ReadyMessage(*this);
	receiver->saveMessage(newMessage);
}

// Testing
bool ReadyMessage::isA(string type) {
	return ReadyMessage::getTypetreenode()->isA(type);
}
