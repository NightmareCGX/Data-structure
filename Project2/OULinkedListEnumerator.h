#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	virtual ~OULinkedListEnumerator();
	bool hasNext() const;
	T next();
	T peek() const;
};

// Implementation goes here

#endif // !OU_LINKED_LIST_ENUMERATOR
