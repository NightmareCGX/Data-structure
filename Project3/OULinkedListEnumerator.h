#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
	// you may add additional member variables and functions here to support the operation of your code
public:
	OULinkedListEnumerator(OULink<T>* first);
	virtual ~OULinkedListEnumerator();  // oops! forgot this the first time
	bool hasNext() const;
	T next();			// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;		// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for OULinkedListEnumerator here

#endif // !OU_LINKED_LIST_ENUMERATOR
