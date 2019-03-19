#ifndef OU_LINK_H
#define OU_LINK_H

#include <cstdlib>

template <typename T>
class OULink {
	template <typename T>
	friend class OULinkedList;
	template <typename T>
	friend class OULinkedListEnumerator;
private:
	T* data = NULL;										// pointer to data item of any type
	OULink* next = NULL;								// pointer to next link
public:
	OULink(const T* item);
	virtual ~OULink();
};

template<typename T>
OULink<T>::OULink(const T* item)
{
	data = new T(*item);
	next = NULL;
}

template<typename T>
OULink<T>::~OULink()
{
	delete data;
}

#endif // !OU_LINK_H