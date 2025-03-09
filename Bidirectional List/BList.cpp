#include <iostream>
#include <fstream> //for file handling
#include <exception>
#include "BList.h"

template<class T>
BList<T>::BList(const BList& object) : BList() //copy constuctor, INITIALISED with a default constructor first
{
	if (object->head != nullptr)
	{
		for (Element* i = object->head.get(); i != nullptr; i = i->next.get()) //head is a pointer
			push_back(i->value);
	}
}

template<class T>
BList<T>::BList(const BList&& object) //move constructor
{
	this->head = object->head; //could use std::move here, but this is clearer for now
	this->tail = object->tail;
	object->head = nullptr; //nullptr for the object
	object->tail = nullptr;
}

template<class T>
BList<T>& BList<T>::operator=(const BList& object)//copy assigment operator
{
	if (this != &object)
	{
		this->head = object.head;
		this->tail = object.tail; //no need to nullptr the object later, because this is a copy
		this->size = object.size;
	}
	return *this;
}

template<class T>
BList<T>& BList<T>::operator=(const BList&& object)//move assigment operator
{
	if (this != &object)//because if it is, then we dont have to do all of this
	{ //no need for deletion afterwards
		this->head = object->head; //taking ownership from object
		this->tail = object->tail;
		this->size = object->size;
		object->head = object->tail = nullptr; //move so we need to delete the objects head and tail
	}
	return *this;
}

template<class T>
void BList<T>::push_back(T data) //a function that pushes back (so as a tail) an object of T data onto the list
{
	auto newElement = std::make_shared<Element>(); //newElement is a smart pointer
	newElement->value = data; //assigning the data

	/*if constexpr (std::is_same<T, std::string>::value) {
		newElement->value = std::string(data);  // Explicitly copy the string to handle memory issues
	}
	else {
		newElement->value = data;  // Default assignment for other types
	}*/

	newElement->value = data;  // Default assignment for other types
	if (head == nullptr)//if theres nothing in the blist
	{
		head = newElement; //now our head is our newElement, both shared pointers
		tail = newElement; //tail must point to the first element because it is also the last element kinda, since the size will be 1
		newElement->next = nullptr;
		newElement->prev = nullptr;
	}
	else
	{
		tail->next = newElement;
		newElement->prev = tail;
		newElement->next = nullptr; 
		tail = newElement; //the tail pointer now must point to the last element and that is newElement in our case
	}
	size++;
}

template<class T>
void BList<T>::push_front(T data) //a function that pushes to the front an object of T data
{
	auto newElement = std::make_shared<Element>(); //newElement is a smart pointer
	newElement->value = data; //assigning the data

	if (head == nullptr)//if theres nothing in the blist
	{
		head = newElement; //now our head is our newElement, both shared pointers
		tail = head; //tail must point to the first element because it is also the last element kinda, since the size will be 1
		head->prev = nullptr;
		tail->next = nullptr;
	}
	else
	{
		newElement->next = head;
		head->prev = newElement;
		head = newElement; //the head pointer now must point to the first element and that is newElement in our case
		head->prev.reset();
	}
	size++;
}

template<class T>
void BList<T>::insert(T data, unsigned long pos) //a function that inserts after pos
{
	if (pos<0 || pos>=size)//if theres nothing in the blist, check for empty list and 0 btw, need a special case for that
	{
		std::cerr << "Incorrect position" << std::endl;
		return;
	}
	else if (pos == 0)
	{
		push_front(data);
		return;
	}
	else if (pos == size-1)
	{
		push_back(data);
		return;
	}
	else
	{
		auto newElement = std::make_shared<Element>(); //newElement is a smart pointer
		newElement->value = data; //assigning the data
		Element* it = head.get(); //our start position is at the head
		for (unsigned long i = 0; i < pos; i++)
			it = it->next.get(); //iterating through the list
		newElement->prev = std::make_shared<Element>(*it);	//the types clashed. needed to fix that
		newElement->next = it->next;
		it->next = newElement;
		size++;
	}
}

template<class T>
bool BList<T>::isThere(T data) //a function that informs us whether the T data is in the list
{
	Element* it = head.get(); //our start position is at the head
	for (unsigned long i = 0; i < size; i++)
	{
		if (it->value == data)
			return 1; //found it
		it = it->next.get(); //iterating through the list
	}
	return 0; //didnt find it
}

template<class T>
std::shared_ptr<typename BList<T>::Element> BList<T>::find_withAddress(T data) //a function that informs us whether the T data is in the list and returns its address
{
	std::shared_ptr<Element> it = head;
	for (unsigned long i = 0; i < size; i++)
	{
		if (it->value == data)
			return it; //found the T data
		it = it->next; //iterating through the list
	}
	return nullptr; //found nothing
}

template<class T>
void BList<T>::sort(bool mode) //a function initiating a sorting algorithm that is Merge Sort
{
	if (size <= 1)
		return; //the data for such a small array is already sorted if its just one element, if its empty, then we have nothing to sort
	if (head == nullptr)
		std::cout << "null";
	head = mergeSort(head, mode); //calling the proper merge sort for the head
}

template<class T>
std::shared_ptr<typename BList<T>::Element> BList<T>::mergeSort(std::shared_ptr<Element>& top, bool mode) //the Element was not seen here, i needed to explicitly point to it
{
	if (top == nullptr || top->next == nullptr) {
		return top; //stopping point
	}
	//diving the list into two
	std::shared_ptr<Element> fast = top; //will do two steps
	std::shared_ptr<Element> slow = top; //will do one step
	while (fast != nullptr && fast->next!=nullptr && fast->next->next != nullptr) //might want to test
	{
		fast = fast->next->next;
		slow = slow->next;
	} //slow is now at the middle
	std::shared_ptr<Element> secondHalf = slow->next; //using shared pointers because our head is a shared pointer after all
	slow->next = nullptr; //breaking the connection between the two lists in order to properly sort them

	std::shared_ptr<Element> firstHalf = mergeSort(top, mode);
	secondHalf = mergeSort(secondHalf, mode); //recursion in the second part of the list
	return mergeTwoHalves(firstHalf, secondHalf, mode); // recursively we are going to merge first the smaller arrays and at the end our final array
}

template<class T>
std::shared_ptr<typename BList<T>::Element> BList<T>::mergeTwoHalves(std::shared_ptr<Element>& uno, std::shared_ptr<Element>& dos, bool mode)
{
	if (uno == nullptr)
		return dos;
	if (dos == nullptr)
		return uno; //if either of those are empty, return the other

	std::shared_ptr <Element> result = nullptr; //to save my tail
	if(mode)//1 - ascending order
	{ 
		if (uno->value>=dos->value) //the first list is the one with bigger value (or the same cause that can also happen)
		{
			result = uno;
			result->next = mergeTwoHalves(uno->next, dos, mode); //recursively merge the two lists
		}
		else //the dos list has the bigger value
		{
			result = dos;
			result->next = mergeTwoHalves(uno,dos->next, mode);
		}
	}
	else //0 - descending order
	{
		if (uno->value <= dos->value) //the first list is the one with bigger value (or the same cause that can also happen)
		{
			result = uno;
			result->next = mergeTwoHalves(uno->next, dos, mode); //recursively merge the two lists
		}
		else //the dos list has the bigger value
		{
			result = dos;
			result->next = mergeTwoHalves(uno, dos->next, mode);
		}
	}
	//connect
	if (result->next != nullptr)
		result->next->prev = result;
	result->prev = nullptr;

	std::shared_ptr < Element> it = result;
	while (it->next != nullptr)
		it = it->next;

	this->tail = it;//update the tail
	return result;
}

template<class T>
void BList<T>::print() //a function that prints the whole list from head to tail
{
	Element* it = head.get(); //our start position is at the head
	for (unsigned long i = 0; i < size; i++)
	{
		std::cout << "Element: " << i << " Value: " << it->value<<std::endl;// << " Size of list: " << size << " Adress of this one: " << it << std::endl;
		it = it->next.get(); //iterating through the list
	}
}

template<class T>
void BList<T>::printAddresses() //a function that prints all the addresses in the list
{
	Element* it = head.get(); //our start position is at the head
	std::cout << "Size of the list: " << size << std::endl;
	for (unsigned long i = 0; i < size; i++)
	{
		std::cout << "Element: " << i <<" Address: " << it << std::endl;
		it = it->next.get(); //iterating through the list
	}
}

template<class T>
std::shared_ptr<typename BList<T>::Element> BList<T>::getHead() //a function that returns the head of the list
{
	return this->head;
}

template<class T>
std::shared_ptr<typename BList<T>::Element> BList<T>::getTail() //a function that returns the tail of the list
{
	if (size == 1)
		return this->head;
	return this->tail;
}

template<class T>
unsigned long BList<T>::getSize() //a function that returns the size of the list
{
	return this->size;
}

template<class T> //a function that serializes a list into a binary file
void BList<T>::serialize(const std::string& filename) //const aka we cannot modify the filename inside this function, its for safety
{
	std::ofstream o(filename, std::ios::binary);//we need to output in binary 
	if (!o.is_open()) //just checking
	{
		std::cerr << "File for serialisation cannot open." << std::endl; //cout error
		return; //so that it doesnt proceed
	}
	Element* it = head.get();//start of our list
	while (it)
	{
		if constexpr (std::is_same<T,std::string>::value)
		{
			size_t length = it->value.size();
			o.write(reinterpret_cast<const char*>(&length), sizeof(size_t)); //first writing the length of the list
			o.write(it->value.c_str(), length); //c_str() - returns a pointer to an array of characters with null char at the end
		}
		else if constexpr (std::is_class_v<T>)
		{
			try {
				it->value.serialize(o); //for a custom type
			}
			catch (const std::exception& e)
			{
				std::cerr << "The custom type does not have a serialize function. Exception caught: " << e.what() << std::endl;
			}
			
		}
		else
		{
			o.write(reinterpret_cast<char*>(&it->value), sizeof(it->value)); //normal types
		}
		it = it->next.get(); //iterate through the list
	}
	o.close();
}

template<class T> //a function that deserializes a list from a binary file
void BList<T>::deserialize(const std::string& filename) //const aka we cannot modify the filename inside this function, its for safety
{
	while (head)
	{
		std::shared_ptr<Element> temp = head->next;
		head->next.reset(); //reset because this is a shared pointer with reference counts, reset is preferable to nullptr
		head->prev.reset();
		head = std::move(temp);
	}
	tail.reset(); size = 0;
	std::ifstream i(filename, std::ios::binary);
	if (!i.is_open()) //just checking
	{
		std::cerr << "File for deserialisation cannot open." << std::endl; //cout error
		return; //so that it doesnt proceed
	}
	T _value; //because that is what we're going to read from the binary file after all
	while (i.peek()!=EOF) //are we at the end of the file?
	{
		if constexpr (std::is_same<T, std::string>::value)
		{
			size_t length;
			if (!i.read(reinterpret_cast<char*>(&length), sizeof(size_t))) break;
			_value.resize(length);
			if (!i.read(&_value[0], length)) break;
		}
		else if constexpr (std::is_class_v<T>)
		{
			try {
				_value.deserialize(i); //for custom type
			}
			catch (const std::exception& e) {
				std::cerr << "The custom type does not have a deserialize function. Exception caught:" << e.what() << std::endl;
			}
		}
		else
		{
			if (!i.read(reinterpret_cast<char*>(&_value), sizeof(T))) break;
		}
		this->push_back(_value);
	}
	i.close();
}

template<class T> //a function for reading the data from a file .txt into a list 
void BList<T>::ReadFromFile(const std::string& filename)
{
	while (head)
	{
		std::shared_ptr<Element> temp = head->next;
		head->next.reset(); //reset because this is a shared pointer with reference counts, reset is preferable to nullptr
		head->prev.reset();
		head = std::move(temp);
	}
	std::ifstream i(filename);
	if (!i.is_open())
	{
		std::cerr << "File for reading data cannot open." << std::endl;
		return;
	}
	T _value;
	while (i >> _value)
		this->push_back(_value);
	i.close();
}

template<class T> //a function for writing the data from a list into a file .txt
void BList<T>::WriteIntoFile(const std::string& filename)
{
	std::ofstream o(filename);
	if (!o.is_open())
	{
		std::cerr << "File for writing data cannot open." << std::endl;
		return;
	}
	Element* it = head.get();
	while (it)
	{
		o << it->value << "\n";
		it = it->next.get(); //iterate through the list
	}
	o.close();
}
