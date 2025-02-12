#include "Domain.h"
#include <iostream>
#include <string>
using std::string;

Book::Book() noexcept
{
	title = "";
	author = "";
	genre = "";
	year = 0;
}
Book::Book(string title, string author, string genre, int year) :title{ title }, author{ author }, genre{ genre }, year{ year }
{

}
Book::Book(const Book& other) 
{
	this->author = other.author;
	this->title = other.title;
	this->genre = other.genre;
	this->year = other.year;

	//std::cout << "Copie\n";
}
const string& Book::getTitle() const noexcept
{
	return title;
}
const string& Book::getAuthor() const noexcept
{
	return author;
}
const string& Book::getGenre() const noexcept
{
	return genre;
}
const int& Book::getYear() const noexcept
{
	return year;
}
bool Book::operator==(Book other) noexcept
{
	if (this->getAuthor() != other.getAuthor()) return 0;
	if (this->getGenre() != other.getGenre()) return 0;
	if (this->getTitle() != other.getTitle()) return 0;
	if (this->getYear() != other.getYear()) return 0;
	return 1;
}
bool Book::operator==(const Book& other) const noexcept
{
	if (this->getAuthor() != other.getAuthor()) return 0;
	if (this->getGenre() != other.getGenre()) return 0;
	if (this->getTitle() != other.getTitle()) return 0;
	if (this->getYear() != other.getYear()) return 0;
	return 1;
}
Book& Book::operator=(const Book& other)
{
	this->author = other.author;
	this->title = other.title;
	this->genre = other.genre;
	this->year = other.year;

	//std::cout << "Copie =\n";
	return *this;
}
Book::Book(Book&& other) noexcept
{
	this->author = std::move(other.author);
	this->genre = std::move(other.genre);
	this->title = std::move(other.title);
	this->year = std::move(other.year);

	//std::cout << "Constructor nou \n";
}
Book& Book::operator=(Book&& other) noexcept
{
	this->author = std::move(other.author);
	this->genre = std::move(other.genre);
	this->title = std::move(other.title);
	this->year = std::move(other.year);

	//std::cout << "Mutat = \n";
	return *this;
}

void DTO::increment() noexcept
{
	nr++;
}
const int& DTO::getNr() const noexcept
{
	return nr;
}
const string& DTO::getType() const noexcept
{
	return tip;
}

DTO::DTO(const string& tip, int nr)
{
	this->tip = tip;
	this->nr = nr;
}
std::ostream& operator<<(std::ostream& out, const DTO& dto)
{
	out << "Carti de genul " << dto.getType() << ": " << dto.getNr();
	return out;
}
