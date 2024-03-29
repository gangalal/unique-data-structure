#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include "NvraRecord.h"
#include "TemplatedArray.h"
#include "Comparator.h"
#include "NvraComparator.h"
#include "Sorter.h"
#include "Search.h"
#include <algorithm>
#include "Enumerator.h"
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"
#include "NvraHasher.h"
#include "HashTableEnumerator.h"
#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "AVLTreeOrder.h"
using namespace std;

vector <string> parseLine(const string& oneLine, vector <string>& itemCounts);
NvraRecord* addLine(vector <string>& itemCounts);
bool validLine(vector <string>& itemCounts);
void validRecordId(AVLTree<NvraRecord>* tree, unsigned long lineCount, NvraRecord* newRecord);
AVLTree<NvraRecord>* readingFiles(AVLTree<NvraRecord>* tree, ifstream& fileName, unsigned long& totalLines,
	unsigned long& totalValidLines);
TemplatedArray <NvraRecord>* mergeArrayRecord(TemplatedArray<NvraRecord>* oldArray, TemplatedArray<NvraRecord>* newArray);
AVLTree<NvraRecord>* mergeRecord(AVLTree<NvraRecord>* oldTree, AVLTree<NvraRecord>* newTree);
AVLTree<NvraRecord>* purgeRecord(AVLTree<NvraRecord>* oldTree, AVLTree<NvraRecord>* newTree);
void printRecords(TemplatedArray<NvraRecord> newArray, ostream& stream);
void printLinkList(OULinkedList<NvraRecord>* list, ostream& stream);
void printHashTable(HashTable<NvraRecord>* htable, ostream& stream);
void printOrder(AVLTree<NvraRecord>* tree, ostream& stream, AVLTreeOrder order);
void manipulation(AVLTree<NvraRecord>* tree, HashTable<NvraRecord>* htable, NvraHasher* hash, OULinkedList<NvraRecord>* oldList, TemplatedArray <NvraRecord>* currentArrays, unsigned long& totalLines,
	unsigned long& totalValidLines, NvraComparator comparator);
void search(TemplatedArray<NvraRecord>* currentArrays, int sortColumn);


NvraRecord readOneLine(string oneLine)
{
	NvraRecord line1; //NvraRecord object to hold single line
	vector <string>  itemCounts(0);// vector to hold single word of every line.
	istringstream stream(oneLine);
	string word;
	while (getline(stream, word, ','))//parses every words.
	{
		itemCounts.push_back(word); //pushes every worsds to itemCount vector
	}
	(line1).addNum(stoi(itemCounts.at(0)));//converting string to integer and pushes back to vector
	(line1).addNum(stoi(itemCounts.at(1)));
	(line1).addNum(stoi(itemCounts.at(2)));
	(line1).addNum(stoi(itemCounts.at(4)));
	(line1).addNum(stoi(itemCounts.at(5)));
	(line1).addNum(stoi(itemCounts.at(6)));
	(line1).addNum(stoi(itemCounts.at(7)));
	(line1).addNum(stoi(itemCounts.at(8)));
	(line1).addNum(stoi(itemCounts.at(9)));
	(line1).addNum(stoi(itemCounts.at(10)));
	(line1).addNum(stoi(itemCounts.at(13)));
	(line1).addNum(stoi(itemCounts.at(14)));
	(line1).addNum(stoi(itemCounts.at(15)));
	(line1).addNum(stoi(itemCounts.at(16)));
	(line1).addNum(stoi(itemCounts.at(17)));
	(line1).addNum(stoi(itemCounts.at(18)));
	(line1).addNum(stoi(itemCounts.at(19)));
	(line1).addNum(stoi(itemCounts.at(20)));
	(line1).addNum(stoi(itemCounts.at(21)));
	(line1).addNum(stoi(itemCounts.at(22)));
	(line1).addNum(stoi(itemCounts.at(23)));

	(line1).addString(itemCounts.at(3));
	(line1).addString(itemCounts.at(11));
	(line1).addString(itemCounts.at(12));

	return line1; //returns NvraRecord object
}


int main()
{
	NvraComparator comparator = NvraComparator(0); //comparator object with field 0
	AVLTree<NvraRecord>* tree = new AVLTree<NvraRecord>(&comparator);
	OULinkedList<NvraRecord>* list = new OULinkedList<NvraRecord>(&comparator); //linkedList object with comparator field 0
	TemplatedArray <NvraRecord>* currentArrays = new TemplatedArray <NvraRecord>(); //NvraArray object with default capacity
	NvraHasher* hash = new NvraHasher();
	HashTable<NvraRecord>* htable = new HashTable<NvraRecord>(&comparator, hash, tree->getSize());

	unsigned long totalLines = 0;
	unsigned long totalValidLines = 0;
	std::string fileName;
	std::ifstream readFile;
	cout << "Enter data file name: ";

	while (std::getline(std::cin, fileName))//breaks iff file lenght is zero or totalLines > 0
	{

		if (fileName.length() == 0)
		{
			break;
		}
		readFile.open(fileName);

		if (!readFile.is_open())
		{
			cout << "File is not available." << endl;
			cout << "Enter data file name: ";
			return 0;
		}
		else
		{
			tree = readingFiles(tree, readFile, totalLines, totalValidLines);
		}
		if (totalValidLines == 0)
		{
			cout << "No valid records found." << endl;
			cout << "Enter data file name: ";
		}
		else
		{
			//tree->remove(&readOneLine("81, 10007, 1, PA, 0, 0, 0, 0, 0, 0, 0, 01 - ADAIR, Public Assistance(Unspecified), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1"));
			AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::inorder);
			while (treeEnum.hasNext())
			{
				try
				{
					NvraRecord record = treeEnum.next();
					htable->insert(&record);
					currentArrays->add(&record);
				}
				catch (const ExceptionAVLTreeAccess* e)
				{
					delete e;
				}
			}
			break;
		}
	}
	manipulation(tree, htable, hash, list, currentArrays, totalLines, totalValidLines, comparator);
}

/**
 * parses comma separaed line
 * @param  oneLine passes line to parse
 * @param  itemCount passes vector to store parsed line as string
 * @return vector with parse line
 */
std::vector <std::string> parseLine(const std::string& oneLine, std::vector <std::string>& itemCounts)
{
	std::istringstream stream(oneLine);
	std::string word;
	while (getline(stream, word, ','))//parses every words.
	{
		itemCounts.push_back(word); //pushes every worsds to itemCount vector
	}
	return itemCounts;
}

/**
 * adds data to NvraRecord
 * @param  itemCount passes vector storing parsed line
 * @return single NvraRecord
 */
NvraRecord* addLine(vector <string>& itemCounts)
{
	NvraRecord* line1 = new NvraRecord; //NvraRecord object to hold single line
	for (unsigned int i = 0; i < 24; ++i)
	{
		if (i != 3 && i != 11 && i != 12)
		{
			(line1)->addNum(stoi(itemCounts.at(i)));//converting string to integer and pushes back to NvraRecord Object


		}
		else
		{
			(line1)->addString(itemCounts.at(i));
		}
		//returns NvraRecord object
	}
	return line1;
}

/**
 * valids the NvraRecord
 * @param  itemCount passes vector storing parsed line
 * @return true if no negative value are there across the field
 */
bool validLine(vector <string>& itemCounts)
{
	bool check = true;
	for (int i = 0; i < MAX_NUMS + MAX_STRINGS; ++i) {
		if (i != 3 && i != 11 && i != 12) {

			try
			{
				int num = stoi(itemCounts.at(i));
				if (num < 0)
				{
					check = false;
				}
			}
			catch (const std::invalid_argument&) {
				check = false;
			}

		}
	}
	return check;
}

/**
 * firstly, checks if the record is duplicate
 * secondly, inserts data to linked list in order and unique if not duplicate
 * @param  list passes linked list object to store data
 * @param lineCount passes number of line count till
 * @param newRecord passes record to be inserted
 */
void validRecordId(AVLTree<NvraRecord>* tree, unsigned long lineCount, NvraRecord* newRecord)
{
	if (!tree->insert(newRecord))//if false prints following
	{
		cout << "Duplicate record ID " << newRecord->getNum(0) << " at line " << lineCount << "." << endl;
	}
	/*else
	{
		list->insert(newRecord);//if true inserts
	}*/
}

/**
* reads all the line present in file and calls insert function if valid
* @param  list passes linked list object to store data
* @param totalLine passes number of line count till
* @param totalValidLine passes number of validline count till
* @param fileName passes faleName to be inserted
*/
AVLTree<NvraRecord>* readingFiles(AVLTree<NvraRecord>* tree, std::ifstream& fileName, unsigned long& totalLines, unsigned long& totalValidLines)
{
	std::string line;
	unsigned long lineCounter = 0L;
	std::getline(fileName, line); //removes the head line

	while (std::getline(fileName, line))
	{
		++lineCounter;
		std::vector <std::string> vRecords(0);
		vRecords = parseLine(line, vRecords);
		NvraRecord* records = addLine(vRecords);
		if (validLine(vRecords))
		{
			validRecordId(tree, lineCounter, records);
			++totalValidLines;
		}
		else
		{
			std::cout << "Invalid data at line " << lineCounter << "." << endl;

		}
	}
	totalLines += lineCounter;
	return tree;
}

/**
* merges two TemplatedArray list
* @param  oldList passes current list
* @param newList passes recent list
*/
TemplatedArray <NvraRecord>* mergeArrayRecord(TemplatedArray<NvraRecord>* oldArray, TemplatedArray<NvraRecord>* newArray)
{
	TemplatedArray <NvraRecord>* margedRecord = new TemplatedArray <NvraRecord>();
	NvraComparator comp = NvraComparator(0);
	unsigned long oldIndex = 0;
	unsigned long newIndex = 0;
	while (oldIndex < oldArray->getSize() && newIndex < newArray->getSize())//inserts until the highest commmon data
	{
		if (comp.compare(oldArray->get(oldIndex), newArray->get(newIndex)) == -1)
		{
			NvraRecord record = oldArray->get(oldIndex);
			margedRecord->add(&record);
			++oldIndex;
		}
		else if (comp.compare(oldArray->get(oldIndex), newArray->get(newIndex)) == 0)
		{
			NvraRecord record = newArray->get(newIndex);
			margedRecord->add(&record);
			++newIndex;
			++oldIndex;
		}
		else
		{
			NvraRecord record = newArray->get(newIndex);
			margedRecord->add(&record);
			++newIndex;
		}
	}
	while (oldIndex < oldArray->getSize())//inserts remaining data from old array
	{
		NvraRecord record = oldArray->get(oldIndex);
		margedRecord->add(&record);
		++oldIndex;
	}
	while (newIndex < newArray->getSize())//inserts remaining data from new array


	{
		NvraRecord record = newArray->get(newIndex);
		margedRecord->add(&record);
		++newIndex;
	}

	return margedRecord;
}

/**
* merges two list
* @param  oldList passes current list
* @param newList passes recent list
*/
AVLTree<NvraRecord>* mergeRecord(AVLTree<NvraRecord>* oldTree, AVLTree<NvraRecord>* newTree)
{
	NvraComparator comp = NvraComparator(0);
	AVLTree<NvraRecord>* margedTree = new AVLTree<NvraRecord>(&comp);
	AVLTreeEnumerator<NvraRecord> oldEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
	AVLTreeEnumerator<NvraRecord> newEnum = AVLTreeEnumerator<NvraRecord>(newTree, AVLTreeOrder::inorder);

	while (oldEnum.hasNext() && newEnum.hasNext())//merges until the higest common data
	{
		if (comp.compare(oldEnum.peek(), newEnum.peek()) == -1)
		{
			NvraRecord record = oldEnum.next();
			margedTree->insert(&record);
		}
		else if (comp.compare(oldEnum.peek(), newEnum.peek()) == 0)
		{
			NvraRecord record = newEnum.next();
			margedTree->insert(&record);
			oldEnum.next();
		}
		else
		{
			NvraRecord record = newEnum.next();
			margedTree->insert(&record);
		}
	}
	while (oldEnum.hasNext())//apends remaining data from the old list
	{
		NvraRecord record = oldEnum.next();
		margedTree->insert(&record);
	}
	while (newEnum.hasNext())//apends remaining data from new list
	{
		NvraRecord record = newEnum.next();
		margedTree->insert(&record);
	}
	delete newTree;
	delete oldTree;
	return margedTree;
}

/**
* purges two list
* @param  oldList passes current list
* @param newList passes recent list
*/
AVLTree<NvraRecord>* purgeRecord(AVLTree<NvraRecord>* oldTree, AVLTree<NvraRecord>* newTree)
{
	//NvraComparator comp = NvraComparator(0);
	//AVLTree<NvraRecord>* purgedTree = new AVLTree<NvraRecord>(&comp);
	AVLTreeEnumerator<NvraRecord> newEnum = AVLTreeEnumerator<NvraRecord>(newTree, AVLTreeOrder::inorder);
	while (newEnum.hasNext())//removes same duplicate data until the highest common
	{
		try
		{
			NvraRecord newRecord = newEnum.next();
			oldTree->remove(&newRecord);
		}
		catch (const ExceptionAVLTreeAccess* e)
		{
			delete e;
		}
	}
		delete newTree;
		return oldTree;
	
}

/**
* prints all the records from Templated array
* @param  newArray passes current Array
* @param stream passes ostream to write
*/
void printRecords(TemplatedArray<NvraRecord> newArray, ostream& stream)
{
	for (unsigned long i = 0; i < newArray.getSize(); i++)
	{
		try
		{
			stream << newArray.get(i) << endl;

		}
		catch (ExceptionIndexOutOfRange& e)
		{
			cout << "ExceptionIndexOutOfRange" << i << endl;
		}

	}

}

/**
* prints all the records from the linked list
* @param  list passes current list
* @param stream passes ostream to write
*/
void printLinkList(OULinkedList<NvraRecord>* list, ostream& stream)
{
	OULinkedListEnumerator <NvraRecord> LLE = list->enumerator();
	while(LLE.hasNext())
	{

		stream << LLE.next() << endl;
	}
}

/**
* prints all the records from the linked list
* @param  list passes current list
* @param stream passes ostream to write
*/
void printOrder(AVLTree<NvraRecord>* tree, ostream& stream, AVLTreeOrder order)
{
	if (order == AVLTreeOrder::inorder)
	{
		AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::inorder);
		while (treeEnum.hasNext())
		{
			try
			{
				stream << treeEnum.next() << endl;
			}
			catch (const ExceptionAVLTreeAccess* e)
			{
				delete e;
			}
		}
	}
	
	if (order == AVLTreeOrder::postorder)
	{
		AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::postorder);
		while (treeEnum.hasNext())
		{
			try
			{
				stream << treeEnum.next() << endl;
			}
			catch (const ExceptionAVLTreeAccess* e)
			{
				delete e;
			}
		}
	}
	
	if (order == AVLTreeOrder::preorder)
	{
		AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::preorder);
		while (treeEnum.hasNext())
		{
			try
			{
				stream << treeEnum.next() << endl;
			}
			catch (const ExceptionAVLTreeAccess* e)
			{
				delete e;
			}
		}
	}
}





/**
* prints all the records from the linked list
* @param  list passes current list
* @param stream passes ostream to write
*/
void printHashTable(HashTable<NvraRecord>* htable, ostream& stream)
{
	HashTableEnumerator<NvraRecord> hEnum = HashTableEnumerator<NvraRecord>(htable);
	if (hEnum.hasNext())
	{
		try
		{
			NvraRecord item = hEnum.next();
			unsigned long newBucketNum = htable->getBucketNumber(&item);

			stream << newBucketNum << ": " << item << endl;
			unsigned long prevBucket = newBucketNum;
			while (hEnum.hasNext())
			{
				NvraRecord newItem = hEnum.next();
				newBucketNum = htable->getBucketNumber(&newItem);
				if (prevBucket == newBucketNum)
				{
					stream << "OVERFLOW: " << newItem << endl;

				}
				else
				{
					stream << endl;
					stream << newBucketNum << ": " << newItem << endl;
					prevBucket = newBucketNum;
				}
			}


		}
		catch (const ExceptionEnumerationBeyondEnd* e)
		{
			delete e;
		}
	}
}


/**
* after storing atleast one data manipulation are done
* @param  oldList passes current linked list object
* @param currenArray passes current TemplatedArray object
* @param totalLine passes number of line count till
* @param totalValidLine passes number of validline count till
* @param comparator passes current NvraComparator object
*/
void manipulation(AVLTree<NvraRecord>* oldTree, HashTable<NvraRecord>* htable, NvraHasher* hash, OULinkedList<NvraRecord>* oldList, TemplatedArray <NvraRecord>* currentArrays,
	unsigned long& totalLines, unsigned long& totalValidLines, NvraComparator comparator)
{
	string  prompt = " ";
	int sortColumn = 0;
	bool sortSelected = false;
	bool mergeSelected = false;
	bool purgeSelected = false;
	while (prompt != "q")
	{
		cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, (h)ash table, (pre)order, (in)order, (post)order, or (q)uit: ";
		// user input
		getline(cin, prompt);
		if (prompt == "m")
		{
			mergeSelected = true;
			string fileName;
			cout << "Enter data file name: ";
			while (std::getline(std::cin, fileName))//breaks iff length is zero of one file is benig read
			{
				if (fileName.length() == 0)
				{
					break;
				}
				ifstream readFile;
				readFile.open(fileName);
				if (!readFile.is_open())
				{
					cout << "File is not available." << endl;
				}
				else
				{
					AVLTree<NvraRecord>* newTree = new AVLTree<NvraRecord>(&comparator);
					newTree = readingFiles(newTree, readFile, totalLines, totalValidLines);
					oldTree = mergeRecord(oldTree, newTree);
					break;
				}
				cout << "Enter data file name: ";

			}
			
			if (sortSelected)
			{
				TemplatedArray <NvraRecord>* newArray = new TemplatedArray <NvraRecord>(); //NvraArray object with 1 capacity initially
				OULinkedListEnumerator <NvraRecord> lEnum = oldList->enumerator();
				while (lEnum.hasNext())
				{
					NvraRecord record = lEnum.next();
					newArray->add(&record);
				}
				currentArrays = mergeArrayRecord(currentArrays, newArray);//merges
				delete newArray;
			}
			else
			{
				delete currentArrays;
				delete htable;
				currentArrays = new TemplatedArray <NvraRecord>(); //NvraArray object with 1 capacity initially
				htable = new HashTable <NvraRecord>(&comparator, hash, oldList->getSize()); //NvraArray object with 1 capacity initially
				OULinkedListEnumerator <NvraRecord> lEnum = oldList->enumerator();
				AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
				while (treeEnum.hasNext())
				{
					try
					{
						NvraRecord record = treeEnum.next();
						htable->insert(&record);
						currentArrays->add(&record);
					}
					catch (const ExceptionAVLTreeAccess* e)
					{
						delete e;
					}
				}
			}			
		}

		if (prompt == "p")
		{
			purgeSelected = true;
			string fileName;
			cout << flush << "Enter data file name: ";
			while (std::getline(std::cin, fileName))//breaks iff length is zero of one file is benig read
			{
				if (fileName.length() == 0)
				{
					break;
				}
				ifstream readFile;
				readFile.open(fileName);
				if (!readFile.is_open())
				{
					cout << "File is not available." << endl;
					//break;
				}
				else
				{
					if (oldTree->getSize() != 0)
					{
						AVLTree<NvraRecord>* newTree = new AVLTree<NvraRecord>(&comparator);
						newTree = readingFiles(newTree, readFile, totalLines, totalValidLines);
						oldTree = purgeRecord(oldTree, newTree);//purges
					}
					break;
				}
				cout << "Enter data file name: ";
			}
			
			
			if (sortSelected)
			{
				TemplatedArray <NvraRecord>* newArray = new TemplatedArray <NvraRecord>(); //NvraArray object with 1 capacity initially
				OULinkedListEnumerator <NvraRecord> lEnum = oldList->enumerator();
				while (lEnum.hasNext())
				{
					NvraRecord record = lEnum.next();
					newArray->add(&record);
				}
				currentArrays = mergeArrayRecord(currentArrays, newArray);
				delete newArray;
			}
			else
			{
				delete currentArrays;
				delete htable;
				currentArrays = new TemplatedArray <NvraRecord>(); //NvraArray object with 1 capacity initially
				htable = new HashTable <NvraRecord>(&comparator, hash, oldList->getSize()); //NvraArray object with 1 capacity initially
				OULinkedListEnumerator <NvraRecord> lEnum = oldList->enumerator();
				if (oldTree->getSize() != 0)
				{
					AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
					while (treeEnum.hasNext())
					{
						try
						{
							NvraRecord record = treeEnum.next();
							htable->insert(&record);
							currentArrays->add(&record);
						}
						catch (const ExceptionAVLTreeAccess* e)
						{
							delete e;
						}
					}
				}
			}
			
		}
		if (prompt == "r")
		{
			AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
			while (treeEnum.hasNext())
			{
				
					NvraRecord record = treeEnum.next();
					oldList->insert(&record);
				
			}
			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName)) //breaks iff length is zero or file name does not exist
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			printLinkList(oldList, *outputPtr);
			*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldList->getSize() << endl;
		}

		if (prompt == "o")
		{
			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName))//breaks iff length is zero or one file is benig read
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			if (sortSelected || mergeSelected || purgeSelected) //if sortSelected or mergeSelected or purgeSelected prints from TemplatedArray
			{
				printRecords(*currentArrays, *outputPtr);
				*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << currentArrays->getSize() << endl;
			}
			else //prints from LinkedList
			{
				printRecords(*currentArrays, *outputPtr);
				*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << currentArrays->getSize() << endl;
			}
		}

		if (prompt == "pre")
		{

			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName)) //breaks iff length is zero or file name does not exist
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			printOrder(oldTree, *outputPtr, AVLTreeOrder::preorder);
			*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldTree->getSize() << endl;
		}


		if (prompt == "in")
		{

			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName)) //breaks iff length is zero or file name does not exist
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			if (oldTree->getSize() != 0)
			{
				printOrder(oldTree, *outputPtr, AVLTreeOrder::inorder);
				*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldTree->getSize() << endl;

			}
			else
			{
				*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldTree->getSize() << endl;
			}
		}


		if (prompt == "post")
		{

			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName)) //breaks iff length is zero or file name does not exist
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			printOrder(oldTree, *outputPtr, AVLTreeOrder::postorder);
			*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldTree->getSize() << endl;
		}		

		if (prompt == "s")
		{
			sortSelected = true;
			TemplatedArray<NvraRecord>* newArrays = new TemplatedArray <NvraRecord>();//copying data to TemplatedArray 
			AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
			while (treeEnum.hasNext())
			{
				try
				{
					NvraRecord record = treeEnum.next();
					newArrays->add(&record);
				}
				catch (const ExceptionAVLTreeAccess* e)
				{
					delete e;
				}
			}

			cout << "Enter search field(0 - 23): " << endl;
			string sortInput;
			getline(cin, sortInput);
			sortColumn = stoi(sortInput);

			if ((sortColumn >= 0) && (sortColumn <= 23))
			{

				NvraComparator compare(sortColumn);
				Sorter<NvraRecord>::sort(*newArrays, compare);
			}
			delete currentArrays;
			currentArrays = new TemplatedArray <NvraRecord>();
			for (unsigned long i = 0; i < newArrays->getSize(); i++)
			{
				try
				{
					NvraRecord record = newArrays->get(i);
					currentArrays->add(&record);

				}
				catch (ExceptionIndexOutOfRange& e)
				{
					cout << "ExceptionIndexOutOfRange" << i << endl;
				}

			}
			delete newArrays;
		}

		if (prompt == "h")
		{
			OULinkedListEnumerator <NvraRecord> lnum = oldList->enumerator();
			while (lnum.hasNext())
			{
				//cout << lnum.next() << endl;
				NvraRecord item = lnum.next();

				htable->insert(&item);
			}
			string fileName;
			ostream* outputPtr = NULL;
			ofstream writingFile;
			cout << "Enter output file name: ";
			while (getline(cin, fileName)) //breaks iff length is zero or file name does not exist
			{

				if (fileName.length() == 0)
				{
					outputPtr = &cout;
					break;
				}

				else
				{
					writingFile.open(fileName);
					if (writingFile.is_open())
					{
						outputPtr = &writingFile;
						break;
					}
					else
					{
						cout << "File is not available." << endl;
						cout << "Enter output file name: ";
					}
				}

			}
			printHashTable(htable, *outputPtr);
			*outputPtr << endl;
			*outputPtr << "Base Capacity: " << htable->getBaseCapacity() << "; Total Capacity: " << htable->getTotalCapacity() << "; Load Factor: " << htable->getLoadFactor() << endl;
			*outputPtr << "Data lines read: " << totalLines << "; Valid NVRA records read: " << totalValidLines << "; NVRA records in memory: " << oldList->getSize() << endl;
		}

		if (prompt == "f")
		{

			if (sortSelected)
			{
				search(currentArrays, sortColumn); // sort is not selected search from the TemplateArray
			}
			else
			{
				TemplatedArray<NvraRecord>* newArrays = new TemplatedArray <NvraRecord>(); //copying data to TemplatedArray 
				AVLTreeEnumerator<NvraRecord> treeEnum = AVLTreeEnumerator<NvraRecord>(oldTree, AVLTreeOrder::inorder);
				while (treeEnum.hasNext())
				{
					try
					{
						NvraRecord record = treeEnum.next();
						newArrays->add(&record);
					}
					catch (const ExceptionAVLTreeAccess* e)
					{
						delete e;
					}
				}

				search(newArrays, sortColumn);
				delete currentArrays;
				currentArrays = new TemplatedArray <NvraRecord>();
				for (unsigned long i = 0; i < newArrays->getSize(); i++)
				{
					try
					{
						NvraRecord record = newArrays->get(i);
						currentArrays->add(&record);

					}
					catch (ExceptionIndexOutOfRange& e)
					{
						cout << "ExceptionIndexOutOfRange" << i << endl;
					}

				}
				delete newArrays;
			}
		}
		if (prompt == "q")
		{
			cout << "Thanks for using VoteR." << endl;
			break;
		}
	}
}

/**
*if search column and user selected field is same then serches data using binary search else searches linearly
*@param currentArray passes current Array
*@param sortColumn passes sort column
*/
void search(TemplatedArray<NvraRecord>* currentArrays, int sortColumn)
{
	NvraRecord tempRecord;
	cout << "Enter search field (0-23): " << endl;
	string findInput;
	getline(cin, findInput);
	int field;
	try {
		field = stoi(findInput);
	}
	catch (const invalid_argument& e) {
		return;
	}

	if ((field >= 0) && (field <= 23) && (field != 3) && (field != 11) && (field != 12))
	{
		cout << "Enter non-negative field value: ";
		int nonNegativeValue;
		cin >> nonNegativeValue;
		cin.ignore();
		if (field < 3) {
			tempRecord.setNum(nonNegativeValue, field);
		}
		else if (field < 11) {
			tempRecord.setNum(nonNegativeValue, field - 1);
		}
		else {
			tempRecord.setNum(nonNegativeValue, field - 3);
		}

	}
	if (field == 3 || field == 11 || field == 12)
	{
		cout << "Enter exact text on which to search: ";
		string inputText;
		getline(cin, inputText);

		if (inputText.length() == 0) {
			return;
		}
		if (field == 3) {
			tempRecord.setString(inputText, 0L);
		}
		else if (field == 11) {
			tempRecord.setString(inputText, 1L);
		}
		else {
			tempRecord.setString(inputText, 2L);
		}
	}
	if (field < 0 || field > 23)
	{
		return;
	}

	NvraComparator comp(field);
	unsigned long counter = 0;
	bool flag = false;
	if (field == sortColumn)
	{
		flag = true;
	}
	if (flag)//if search column and user selected field is same then serches data using binary search else searches linearly
	{
		long long location = binarySearch(tempRecord, *currentArrays, comp);
		if (location >= 0) {
			TemplatedArray<NvraRecord>* searchRecords = new TemplatedArray<NvraRecord>();
			NvraRecord NRecord = currentArrays->get(location);
			searchRecords->add(&NRecord);
			++counter;
			unsigned long locations = location + counter;
			while (locations < currentArrays->getSize() && comp.compare(currentArrays->get(locations), NRecord) == 0)
			{
				NRecord = currentArrays->get(locations);
				searchRecords->add(&NRecord);
				++locations;
				++counter;
			}

			printRecords(*searchRecords, cout);
		}
	}
	else//searches linearly
	{
		TemplatedArray<NvraRecord>* records = new TemplatedArray<NvraRecord>();
		for (unsigned long i = 0; i < currentArrays->getSize(); ++i) {
			NvraRecord NRecord = currentArrays->get(i);
			if (comp.compare(NRecord, tempRecord) == 0) {
				records->add(&NRecord);
				++counter;
			}
		}
		printRecords(*records, cout);
	}
	cout << "NVRA records found: " << counter << "." << endl;

}


