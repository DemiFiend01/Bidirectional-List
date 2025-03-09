#include <iostream>
#include <random>//for filling the data with random values
#include <algorithm> //for std::for_each
#include <cctype>//for isdigit
#include <sstream>//for getline
#include "BList.cpp"

struct Gliwice
{
	std::string ulica;
	int numer;
public:
	Gliwice() : ulica(""), numer(0) {};
	Gliwice(std::string u, int n) : ulica(u), numer(n) {};
	Gliwice(std::string u) : ulica(u), numer(0) {};
	Gliwice(int n) :ulica(""), numer(n) {};

	void serialize(std::ostream& o) //needed to prepare the serialize and deserialize functions
	{
		size_t length = this->ulica.size();
		o.write(reinterpret_cast<const char*>(&length), sizeof(size_t)); //firstly we have to write the length of the string
		o.write(this->ulica.c_str(), length);
		o.write(reinterpret_cast<const char*>(&this->numer), sizeof(int));
	}
	void deserialize(std::ifstream& i)
	{
		size_t length;
		i.read(reinterpret_cast<char*>(&length), sizeof(size_t)); //firstly read the length of the string
		this->ulica.resize(length);
		i.read(&this->ulica[0], length);
		i.read(reinterpret_cast<char*>(&this->numer), sizeof(int));
	}

	friend std::ostream& operator<<(std::ostream& o, const Gliwice& s)
	{
		o << s.ulica << " " << s.numer;
		return o;
	}
	friend std::istream& operator>>(std::istream& o, Gliwice& s) //cannot be const in this one
	{
		s.ulica.clear();//just in case
		std::string line;
		std::getline(o, line);
		size_t last_space = line.find_last_of(" "); //just before the numer
		if (last_space != std::string::npos)
		{
			s.numer = std::stoi(line.substr(last_space + 1));//just after the found space to the end
			s.ulica = line.substr(0, last_space); //ulica
		}
		return o;
	}
	bool operator == (const Gliwice& s) const //aka will only work with another Gliwice, had some issues with implicit conversions in BList
	{
		if (s.numer == numer && s.ulica == ulica)
			return true;
		else
			return false;
	}
	bool operator >=(const Gliwice& s)
	{
		if (s.ulica == ulica)
		{
			if (s.numer >= numer)
				return true;
			else
				return false;
		}
		else if (s.ulica > ulica)
			return true;
		else
			return false;
	}
	bool operator <=(const Gliwice& s)
	{
		if (s.ulica == ulica)
		{
			if (s.numer > numer)
				return false;
			else
				return true;
		}
		else if (s.ulica > ulica)
			return false;
		else
			return true;
	}
};

class Section
{
private:
	int students;
	std::string leader;
	float mean;
public:
	Section() : students(0), leader(""), mean(0.0) {};
	Section(int _s, std::string _l, float _m) : students(_s), leader(_l), mean(_m) {}; //could do more constructors but this is just an example.
	~Section() {};
	int Count_Students()
	{
		return this->students;
	}
	void serialize(std::ostream& o)
	{
		o.write(reinterpret_cast<const char*>(&this->students), sizeof(int));
		size_t length = this->leader.size();
		o.write(reinterpret_cast<const char*>(&length), sizeof(size_t)); //firstly we have to write the length of the string
		o.write(this->leader.c_str(), length);
		o.write(reinterpret_cast<const char*>(&this->mean), sizeof(float));
	}
	void deserialize(std::ifstream& i)
	{
		i.read(reinterpret_cast<char*>(&this->students), sizeof(int));
		size_t length;
		i.read(reinterpret_cast<char*>(&length), sizeof(size_t)); //firstly read the length of the string
		this->leader.resize(length);
		i.read(&this->leader[0], length);
		i.read(reinterpret_cast<char*>(&this->mean), sizeof(float));
	}
	friend std::ostream& operator<<(std::ostream& o, Section& s) //writing
	{
		o <<s.students <<" " << s.leader <<" " << s.mean<<"\t";
		return o;
	}
	friend std::istream& operator>>(std::istream& o, Section& s) //cannot be const in this one, because we're reading
	{
		o >> s.students >> s.leader >> s.mean;
		return o;
	}
	bool operator == (const Section& s) const //aka will only compare with other Section
	{
		if (s.students==students && s.leader==leader && s.mean ==mean)
			return true;
		else
			return false;
	}
	bool operator >=(const Section& s)
	{
		//students->mean->leader
		if (s.students > students) return true;
		if (s.students < students) return false;
		//students are equal so we check the mean
		if (s.mean > mean) return true;
		if (s.mean < mean) return false;
		//the means are equal return the leader
		return s.leader>=leader;
	}
	bool operator <=(const Section& s)
	{
		//students->mean->leader
		if (s.students < students) return true;
		if (s.students > students) return false;
		//students are equal so we check the mean
		if (s.mean < mean) return true;
		if (s.mean > mean) return false;
		//the means are equal return the leader
		return s.leader <= leader;
	}
};

int main()
{ //memory usage goes up here
	int n = 3;//how many elements in some lists for one type of adding elements that is
	const char* ext[] = { "txt","bin" }; //0-5 txt, 6-11 bin, int, float, double, string, struct, class
	const char* files[] = {"Int","Float","Double","String","Struct","Class"};
	std::string filenames[12];
	int index = 0;
	for (const auto& e : ext)
	{
		std::for_each(std::begin(files), std::end(files), [&e,&filenames,&index](const char* filename) {
			filenames[index++] = std::string(filename) + "." + e;
			});
	}
	/*for (auto i : filenames)
		std::cout << i << std::endl;*/

	std::random_device rd; //setting up a random number generator, it was on the lecture
	std::mt19937 rangen(rd()); //random number generator
	std::uniform_int_distribution<int> distint(-100, 100); //range -100 to 100 //memory usage goes up here
	std::uniform_real_distribution<float> distfloat(0, 100); //range 0 to 100 float
	std::uniform_real_distribution<double> distdouble(-100, 100); //range -100 to 100 double

	{
		std::cout << "TESTS:" << std::endl << "1. Create a Bidirectional List for different variable types and\n   fill it with n elements using all three functions: push_back, push_front, insert.\n";
		std::cout << "2. Test two find functions for two different situations:\n   One when the searched element exists in the list and the other when it does not.\n   Functions: isThere, find_WithAddress.\n";
		std::cout << "3. Test two modes for sorting the list using the Merge Sort. Modes: 0 - ascending, 1 - descending.\n   Function sort(bool mode).\n" << "4. Test two functions for writing and reading into a txt file. Functions: WriteIntoFile, ReadFromFile.\n";
		std::cout << "5. Test two functions for serialisation and deserialisation of data into binary. Functions: serialize, deserialize.\n\n";
	}
	{//INTS
		std::cout << "Integers, " << n * 3 << " elements:\n1.\n";
		BList<int> listaint; //creating a list of int values
		auto PushRandomInt = [&listaint, &rangen, &distint](int n)
			{
				for (int i = 0; i < n; i++)
				{
					listaint.push_back(distint(rangen)); //pushing back
					listaint.push_front(distint(rangen)); //pushing to the front
				}
			};
		PushRandomInt(n);
		listaint.insert(distint(rangen), 0); //at the front
		listaint.insert(distint(rangen), listaint.getSize() - 1); //at the back
		listaint.insert(5, listaint.getSize() / 2); //in the middle, 5 is for us to find for sure.
		listaint.print();
		//std::shared_ptr<BList<int>::Element> found = listaint.getHead(); //will not work because Element is private
		//int size = listaint.getSize();
		std::cout << "2. Finding 5 which was inserted and 101 which is out of range.\n   5, found: " << listaint.isThere(5) << " has address: " << listaint.find_withAddress(5) << std::endl;
		std::cout << "   101, found: " << listaint.isThere(101) << " has address: " << listaint.find_withAddress(101) << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listaint.sort(0); listaint.print();
		std::cout << "Descending:\n";
		listaint.sort(1); listaint.print();
		std::cout << "4. Writing into a file: \"" << filenames[0] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listaint.WriteIntoFile(filenames[0]); //"Int.txt"
		BList<int> intcopy;
		intcopy.ReadFromFile(filenames[0]);
		intcopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[6] << "\" and then deserialisation from it into a different list\n";
		intcopy.~BList(); //no need for me to create another one
		listaint.serialize(filenames[6]);//"Int.bin"
		intcopy.deserialize(filenames[6]);
		intcopy.print();
	}
	{//FLOATS
		std::cout << "Floats, " << n * 3 << " elements:\n1.\n";
		BList<float> listafloat;
		auto PushRandomFloat = [&listafloat, &rangen, &distfloat](int n) //creating a lambda function to pushback and pushfront some random float numbers
			{
				for (int i = 0; i < n; i++)
				{
					listafloat.push_back(distfloat(rangen));
					listafloat.push_front(distfloat(rangen));
				}
			};
		PushRandomFloat(n);
		listafloat.insert(distfloat(rangen), 0);//insert at the beginning
		listafloat.insert(distfloat(rangen), listafloat.getSize() - 1);//insert at the end
		listafloat.insert(5.0, listafloat.getSize() / 2);//insert a 5 in the middle for finding it later
		listafloat.print();
		std::cout << "2. Finding 5 which was inserted and 101 which is out of range.\n   5, found: " << listafloat.isThere(5.0) << " has address: " << listafloat.find_withAddress(5.0) << std::endl;
		std::cout << "   101, found: " << listafloat.isThere(101.0) << " has address: " << listafloat.find_withAddress(101.0) << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listafloat.sort(0); listafloat.print();
		std::cout << "Descending:\n";
		listafloat.sort(1); listafloat.print();
		std::cout << "4. Writing into a file: \"" << filenames[1] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listafloat.WriteIntoFile(filenames[1]);
		BList<float> floatcopy;
		floatcopy.ReadFromFile(filenames[1]); floatcopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[7] << "\" and then deserialisation from it into a different list\n";
		floatcopy.~BList(); //no need for me to create another one
		listafloat.serialize(filenames[7]);//"Int.bin"
		floatcopy.deserialize(filenames[7]);
		floatcopy.print();
	}
	{//DOUBLES
		std::cout << "Doubles, " << n * 3 << " elements:\n1.\n";
		BList<double> listadouble;
		auto PushRandomDouble = [&listadouble, &rangen, &distfloat](int n) //creating a lambda function to pushback and pushfront some random float numbers
			{
				for (int i = 0; i < n; i++)
				{
					listadouble.push_back(distfloat(rangen));
					listadouble.push_front(distfloat(rangen));
				}
			};
		PushRandomDouble(n);
		listadouble.insert(distfloat(rangen), 0);//insert at the beginning
		listadouble.insert(distfloat(rangen), listadouble.getSize() - 1);//insert at the end
		listadouble.insert(5.0, listadouble.getSize() / 2);//insert a 5 in the middle for finding it later
		listadouble.print();
		std::cout << "2. Finding 5 which was inserted and 101 which is out of range.\n   5, found: " << listadouble.isThere(5.0) << " has address: " << listadouble.find_withAddress(5.0) << std::endl;
		std::cout << "   101, found: " << listadouble.isThere(101.0) << " has address: " << listadouble.find_withAddress(101.0) << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listadouble.sort(0); listadouble.print();
		std::cout << "Descending:\n";
		listadouble.sort(1); listadouble.print();
		std::cout << "4. Writing into a file: \"" << filenames[2] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listadouble.WriteIntoFile(filenames[2]); //"Double.txt"
		BList<double> doublecopy;
		doublecopy.ReadFromFile(filenames[2]); doublecopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[8] << "\" and then deserialisation from it into a different list\n";
		doublecopy.~BList(); //no need for me to create another one
		listadouble.serialize(filenames[8]);//"Double.bin" //does not work for doubles
		doublecopy.deserialize(filenames[8]);
		doublecopy.print();
	}
	{//STRINGS
		std::cout << "Strings, " << n * 3 << " elements:\n1.\n";
		BList<std::string> listastring;
		auto PushRandomString = [&listastring](int n) //creating a lambda function to pushback and pushfront strings from A and from a
			{
				for (int i = 0; i < n; i++)
				{
					std::string ch(1,'A'+i);
					std::string b(1,'a'+i);
					listastring.push_back(ch);
					listastring.push_front(b);
				}
			};
		PushRandomString(n);
		listastring.insert("alpha", 0);//insert at the beginning
		listastring.insert("omega", listastring.getSize() - 1);//insert at the end
		listastring.insert("theta", listastring.getSize() / 2);//insert a theta in the middle for finding it later
		listastring.print();
		std::cout << "2. Finding \"theta\" which was inserted and \"beta\" which is not in the list.\n   \"theta\", found: " << listastring.isThere("theta") << " has address: " << listastring.find_withAddress("theta") << std::endl;
		std::cout << "   \"beta\", found: " << listastring.isThere("beta") << " has address: " << listastring.find_withAddress("beta") << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listastring.sort(0); listastring.print();
		std::cout << "Descending:\n";
		listastring.sort(1); listastring.print();
		std::cout << "4. Writing into a file: \"" << filenames[3] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listastring.WriteIntoFile(filenames[3]); //"String.txt"
		BList<std::string> stringcopy;
		stringcopy.ReadFromFile(filenames[3]); stringcopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[9] << "\" and then deserialisation from it into a different list\n";
		stringcopy.~BList(); //no need for me to create another one
		listastring.serialize(filenames[9]);//"String.bin"
		stringcopy.deserialize(filenames[9]);
		stringcopy.print();
	}
	{//STRUCTS
		std::cout << "Struct Gliwice, has std::string ulica and int numer, " << n * 2+1 << " elements:\n1.\n";
		BList<Gliwice> listastruck;
		listastruck.push_back({ "Kaszubska", 28 });//nowa hala
		listastruck.push_back({ "Kaszubska", 23}); //szkocka kawiarenka mniam
		listastruck.push_front({ "Akademicka",2}); //jêzyki na górniczym
		listastruck.push_front({ "Stanislawa Konarskiego",22 });//CNT
		listastruck.insert({ "Dworcowa",47 },0);//Elektronik my beloved, at the beginning
		listastruck.insert({ "Bohaterow Getta Warszawskiego", 12}, listastruck.getSize() - 1);//dworzec, at the end
		listastruck.insert({ "Akademicka",16 }, listastruck.getSize() / 2);//wydzial AEI hehe in the middle
		listastruck.print();

		std::cout << "2. Finding \"Akademicka\",16 which was inserted and \"Rynek\",44 which is not in the list.\n   \"Akademicka\", 16, found: " << listastruck.isThere({ "Akademicka",16 }) << " has address: " << listastruck.find_withAddress({ "Akademicka",16 }) << std::endl;
		std::cout << "   \"Rynek\", 44, found: " << listastruck.isThere({ "Rynek",44 }) << " has address: " << listastruck.find_withAddress({ "Rynek",44 }) << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listastruck.sort(0); listastruck.print();
		std::cout << "Descending:\n";
		listastruck.sort(1); listastruck.print();
		std::cout << "4. Writing into a file: \"" << filenames[4] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listastruck.WriteIntoFile(filenames[4]); //"Struck.txt"
		BList<Gliwice> struckcopy;
		struckcopy.ReadFromFile(filenames[4]); struckcopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[10] << "\" and then deserialisation from it into a different list\n";
		struckcopy.~BList(); //no need for me to create another one
		listastruck.serialize(filenames[10]);//"Struck.bin"
		struckcopy.deserialize(filenames[10]);
		struckcopy.print();
	}
	{//Class
		std::cout << "Class Section, has int students, std::string leader and float mean, " << n * 2 + 1 << " elements:\n1.\n";
		BList<Section> listaclass;
		listaclass.push_back({20,"Szymon",3.2});
		listaclass.push_back({21,"Maciej",3.6});
		listaclass.push_front({30, "Andrzej",3.5});
		listaclass.push_front({25, "Mikolaj",3.8});
		listaclass.insert({ 22,"Karolina",3.5}, 0);// at the beginning
		listaclass.insert({ 25, "Pola",3.1}, listaclass.getSize() - 1);//dworzec, at the end
		listaclass.insert({ 28,"Krzysztof",3.7}, listaclass.getSize() / 2);//wydzial AEI hehe in the middle
		listaclass.print();

		std::cout << "2. Finding 22, \"Karolina\", 3.5 which was inserted and 24, \"Marcin\", 3.6 which is not in the list.\n   22, \"Karolina\", 3.5, found: " << listaclass.isThere({ 22,"Karolina",3.5 }) << " has address: " << listaclass.find_withAddress({ 22,"Karolina",3.5 }) << std::endl;
		std::cout << "   24, \"Marcin\", 3.6, found: " << listaclass.isThere({ 24, "Marcin",3.6 }) << " has address: " << listaclass.find_withAddress({ 24, "Marcin",3.6 }) << std::endl;
		std::cout << "3. Sorting the list.\nAscending:\n";
		listaclass.sort(0); listaclass.print();
		std::cout << "Descending:\n";
		listaclass.sort(1); listaclass.print();
		std::cout << "4. Writing into a file: \"" << filenames[5] << "\" and then reading from it into a different list.\n"; //the list will be sorted
		listaclass.WriteIntoFile(filenames[5]); //"Struck.txt" //does not work. ReadFromFile does not see the operator.
		BList<Section> classcopy;
		classcopy.ReadFromFile(filenames[5]); classcopy.print();
		std::cout << "5. Serialisation into a file: \"" << filenames[11] << "\" and then deserialisation from it into a different list\n";
		classcopy.~BList(); //no need for me to create another one
		listaclass.serialize(filenames[11]);//"Struck.bin"
		classcopy.deserialize(filenames[11]);
		classcopy.print();
	}

	BList<int> longlist; //testing for a huge array
	auto PushRandomInt = [&longlist,&rangen, &distint](int n)
		{
			for (int i = 0; i < n; i++)
			{
				longlist.push_back(distint(rangen)); //pushing back
				longlist.push_front(distint(rangen)); //pushing to the front
			}
		};
	PushRandomInt(5000);
	longlist.WriteIntoFile("LongListIntExample.txt");
	longlist.serialize("LongListIntExample.bin");
	BList<int> longcopied;
	longcopied.deserialize("LongListIntExample.bin");
	longcopied.WriteIntoFile("CopiedLongListExample.txt");
	//for each will not work for my bidirectional list because for each requires an iterator which i do not have
	
	//test int array
	std::cout << "\nAdditionally testing an array of integers, size [4]:\nAdding 1 to the [0], 2 and 22 to the [1] and 3 to [2]:\n";
	BList<int> array[4];
	array[0].push_back(1);
	array[1].push_back(2);
	array[1].push_back(22);
	array[2].push_back(3);
	std::cout << "Printing only [1] which should have the values 2 and 22:\n";
	array[1].print();
	std::cout << "Printing the whole array using std::for_each():\n";
	std::for_each(array, array+4, [](BList<int>& lista) {
		lista.print(); });

	return 0;
}