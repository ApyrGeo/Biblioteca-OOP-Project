#include "Repository.h"
#include "Exception.h"
#include <vector>
#include <sstream>
#include <string>
#include <optional>
#include <tuple>
#include <print>
#include <fstream>
#include <random>
#include <iostream>

using std::tuple;
using std::string;
using std::vector;

size_t Repository::getRepoSize() noexcept
{
	return getRepoList().size();
}
void Repository::addToList(const Book& element)
{
	/*if (findElement(element))
		throw std::exception("Carte deja existenta!");
	else
		*/
	getRepoList().push_back(element);
}
void Repository::addToListOnPosition(size_t position, const Book& element)
{
	if (position > getRepoSize())
		throw RepositoryException(string("Index invalid!"));

	List.insert(List.begin() + position, element);
}

Book Repository::getElementList(size_t index)
{
	if (index >= getRepoSize())
		throw RepositoryException(string("Index invalid!"));

	return getRepoList().at(index);
}
void Repository::modifyInList(size_t index, const Book& element)
{
	getRepoList().at(index) = element;
}
void Repository::deleteFromList(size_t index) noexcept
{
	getRepoList().erase(List.begin() + index);
}

vector<Book>& Repository::getRepoList() noexcept
{
	return List;
}



FileRepository::FileRepository(const string& path)
{
	this->path = path;
	loadFromFile();
}
string FileRepository::getPath()
{
	return this->path;
}
void FileRepository::saveInFile()
{
	std::ofstream f(path);
	for (const Book& bk : getRepoList())
	{
		std::println(f, "{},{},{},{}", bk.getTitle(), bk.getAuthor(), bk.getGenre(), bk.getYear());
	}
	f.close();
}
vector<string> FileRepository::split(const string& str, const char& delimiter)
{
	std::stringstream ss(str);
	string word;
	vector<string> result;
	while (std::getline(ss, word, delimiter))
	{
		result.push_back(word);
	}
	return result;
}
void FileRepository::loadFromFile()
{
	std::ifstream f(path);
	string line;
	while (std::getline(f,line))
	{
		vector<string> result = split(line, ',');
		addToList(Book(result.at(0), result.at(1), result.at(2), atoi(result.at(3).c_str())));
	}
	f.close();
}

void FileRepository::addToList(const Book& element)
{
	Repository::addToList(element);
	saveInFile();
}
void FileRepository::modifyInList(size_t index, const Book& new_element) 
{
	Repository::modifyInList(index, new_element);
	saveInFile();
}
void FileRepository::deleteFromList(size_t index) noexcept
{
	Repository::deleteFromList(index);
	saveInFile();
}
void MapRepository::tryThrowException()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 100);
	int a = distr(gen);
	
	std::cout << a;
	if (a < probability) throw RepositoryException(string("Exceptie repo map"));
}
MapRepository::MapRepository(int prob)
{
	probability = prob;
}
vector<Book>& MapRepository::getRepoList() noexcept
{
	TransformedList.clear();
	for (std::pair<size_t, Book> a : List)
	{
		TransformedList.push_back(a.second);
	}
	return TransformedList;
}

size_t MapRepository::getRepoSize() noexcept
{
	return List.size();
}

Book MapRepository::getElementList(size_t index)
{
	return List.at(index);
}

void MapRepository::addToList(const Book& element)
{
	tryThrowException();

	List.emplace(getRepoSize(), element);
}

void MapRepository::addToListOnPosition(size_t index, const Book& element)
{
	tryThrowException();

	List.insert({index, element});
}

void MapRepository::modifyInList(size_t index, const Book& new_element)
{
	tryThrowException();

	List.at(index) = new_element;
}

void MapRepository::deleteFromList(size_t index)
{
	tryThrowException();

	List.erase(index);
}



