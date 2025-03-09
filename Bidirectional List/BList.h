#pragma once

//template class BList<T>
template<class T>
class BList {
private:
	//private struct Element that has T value and two shared pointers of type Element called next and prev
	struct Element {
		//value of type T
		T value; 
		//a shared pointer to the next element
		std::shared_ptr<Element>next; 
		//a pointer to the previous element, we can go back and the deletion will be okay, because of the shared ptr handling it
		std::shared_ptr<Element> prev=nullptr; 
		//default constructor, nullptr pointers and does not set value
		Element() : value(), next(nullptr), prev(nullptr) {};
		//constructor, sets value, explicit because some type (probably string or struck) didnt work otherwise
		explicit Element(const T _value) : value(_value), next(nullptr), prev(nullptr) {}; 
		//move constructor, copy constructor is not needed (one will be automatically created if its needed)
		explicit Element(T&& _value) : value(std::move(_value)), next(nullptr), prev(nullptr) {}; 
		//~Element() { value.clear(); };
	};
	//need to be an shared ptr in order to later destruct safely itself and others in the chain
	 //shared pointer of Element head - points to the first element in the list
	std::shared_ptr<Element>head;
	//shared pointer of Element tail - points to the last element in the list
	std::shared_ptr<Element>tail; 
	//will be increased after every addition, size is counted like 1-n not like 0-n-1
	//unsigned long size - keeps track of the size of the list
	unsigned long size; 
public:
	//default constructor, there is nothing yet in this b list so nullptr
	BList() : head(nullptr), tail(nullptr), size(0) {} 
	//default destructor
	~BList() {
		while (head)
		{
			std::shared_ptr<Element> temp = head->next;
			head->next.reset(); //reset because this is a shared pointer with reference counts, reset is preferable to nullptr
			head->prev.reset();
			head = std::move(temp);
		}
		tail.reset(); size = 0;
	}
	//copy constuctor, INITIALISED with a default constructor first
	BList(const BList& object);
	//move constructor
	BList(const BList&& object);

	//assigment operator
	BList& operator=(const BList& object);
	//move assigment operator
	BList& operator=(const BList&& object);

	//a function that takes T data and returns nothing, it pushes a T value to the back of the list
	void push_back(T data);
	//a function that takes T data and returns nothing, it pushes a T value to the front of the list
	void push_front(T data); 
	//a function that takes T data and unsigned long pos. it returns nothing. Inserts a T value after pos
	void insert(T data, unsigned long pos); 

	//FIND functions
	//a function that takes T data and returns a boolean value. 1 if T data is in the list, 0 if it is not there
	bool isThere(T data); 
	//a function that takes a T data and returns a shared pointer of Element (address to the data inside the list if it is found, nullptr if it doesn't exist)
	std::shared_ptr<Element> find_withAddress(T data); 

	//MERGE SORT
	//mode
	//0 - ascending
	//1 - descending
	//merge sort initializer
	void sort(bool mode); 
private:
	//a function that takes a shared pointer of Element (head) and sorts the list according to the boolean value mode (order), returns a shared pointer of Element
	//0 - ascending
	//1 - descending
	//The function is used recursively
	std::shared_ptr<Element> mergeSort(std::shared_ptr<Element>& s,bool mode); 
	//a function that takes two shared pointers of Element pointing to the heads of the two seperated lists and merges them using the merge sort algorithm
	// it also takes a bool mode
	//0 - ascending
	//1 - descending
	//returns a shared pointer to the head of the new merged list
	std::shared_ptr<Element> mergeTwoHalves(std::shared_ptr<Element>& uno, std::shared_ptr<Element>& dos, bool mode);
public:

	// a function that takes nothing, returns nothing and prints the values of all the nodes in the list
	void print();
	// a function that takes nothing, returns nothing and prints the addresses of all the nodes in the list
	void printAddresses();
	//a function that takes nothing and returns a shared pointer of Element to the first element in the list
	std::shared_ptr<Element> getHead();
	//a function that takes nothing and returns a shared pointer of Element to the last element in the list
	std::shared_ptr<Element> getTail();
	//a function that takes nothing and returns an unsigned long value of the size of the list
	unsigned long getSize();

	//a function that takes a const string filename and returns nothing, it turns all the values in the list into binary and prints them in a filename.bin file
	void serialize(const std::string& filename); //turning into a binary file
	//a function that takes a const string filename and returns nothing, it reads all the values from a file filename.bin and pushes them back into a given list
	void deserialize(const std::string& filename);

	//a function that takes a const string filename and returns nothing, it reads all the values from a file filename.txt and pushed them all back into a given list
	void ReadFromFile(const std::string& filename);
	//a function that takes a const string filename and returns nothing, it prints all the values in the list into a filename.txt
	void WriteIntoFile(const std::string& filename);
};
