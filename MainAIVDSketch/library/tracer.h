#ifndef LIST_H
#define LIST_H

#include <LinkedList.h>
#include <Adafruit_Sensor.h>

// Extension of LinkedList to allow access to the first and last node
//template<typename T>
class Tracer : public LinkedList<sensors_event_t>
{
public:
	sensors_event_t* getLast()
	{
		return &last->data;
	}
	
	sensors_event_t* getFirst()
	{
		return &root->data;
	}

	ListNode<sensors_event_t> publicGetNode(int index)
	{
		return *getNode(index);
	}
};

#endif // LIST_H