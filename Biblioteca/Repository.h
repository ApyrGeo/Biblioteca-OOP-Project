#pragma once
#include "Domain.h"
#include <map>
#include <vector>

using std::string;
using std::vector;
using std::map;


class AbstractRepository
{
public:
	virtual vector<Book>& getRepoList() noexcept = 0;
	virtual Book getElementList(size_t index) = 0;
	virtual size_t getRepoSize() noexcept = 0;
	virtual void addToListOnPosition(size_t position, const Book& element) = 0;
	virtual void addToList(const Book& bk) = 0;
	virtual void modifyInList(size_t index, const Book& bk) = 0;
	virtual void deleteFromList(size_t index) = 0;
	virtual ~AbstractRepository() {};
};

class MapRepository:public AbstractRepository
{
private:
	map<size_t, Book> List;
	vector<Book> TransformedList;
	int probability;

	void tryThrowException();
public:
	MapRepository(int prob);
	vector<Book>& getRepoList() noexcept override;
	size_t getRepoSize() noexcept override;
	Book getElementList(size_t index);
	void addToList(const Book& element) override;
	void addToListOnPosition(size_t index, const Book& element) override;
	void modifyInList(size_t index, const Book& new_element) override;
	void deleteFromList(size_t index) override;
	//~MapRepository();
};

class Repository:public AbstractRepository
{
private:
	vector<Book> List;
public:

	/// <summary>
	/// Gets the Repo List.
	/// </summary>
	/// <returns>The list of 'Book' elements.</returns>
	vector<Book>& getRepoList() noexcept override;
	/// <summary>
	/// Finds the size of the list.
	/// </summary>
	/// <returns>The length of the repo list.</returns>
	size_t getRepoSize() noexcept override;

	/// <summary>
	/// Gets the element at the given position. Throws an exception if the index is invalid.
	/// </summary>
	/// <param name="index">- element index in list</param>
	/// <returns>The wanted element</returns>
	Book getElementList(size_t index);

	///bool findElement(const Book& element);
	
	/// <summary>
	/// Adds a new element to the list. If it already exists, throws an exception.
	/// </summary>
	/// <param name="element">The element to be added.</param>
	virtual void addToList(const Book& element);

	void addToListOnPosition(size_t position, const Book& element);

	/// <summary>
	/// Modifies an element from the list.
	/// </summary>
	/// <param name="index">- index of the element to be modified.</param>
	/// <param name="new_element">- the new element data to be substituted.</param>
	virtual void modifyInList(size_t index, const Book& new_element);
	/// <summary>
	/// Deletes an element from the list.
	/// </summary>
	/// <param name="index">- the index of the element to be deleted.</param>
	virtual void deleteFromList(size_t index) noexcept;

};

class FileRepository : public Repository 
{
private:
	string path;
	vector<string> split(const string& str, const char& delimiter);
public:
	FileRepository(const string& path);

	string getPath();
	void saveInFile();
	void loadFromFile();

	void addToList(const Book& element) override;
	void modifyInList(size_t index, const Book& new_element) override;
	void deleteFromList(size_t index) noexcept override;
};

