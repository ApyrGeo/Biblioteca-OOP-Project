#pragma once
#include <string>
#include <ostream>
using std::string;

class Book
{
private:
	string title, author, genre;
	int year;
public:
	/// <summary>
	/// Constructor of the book entity.
	/// </summary>
	/// <param name="title">- title of the book</param>
	/// <param name="author">- author of the book</param>
	/// <param name="genre">- genre of the book</param>
	/// <param name="year">- release year of the book</param>
	Book(string title, string author, string genre, int year);

	/// <summary>
	/// Construnctor of a book
	/// </summary>
	Book() noexcept;
	/// <summary>
	/// Copy constructor of a book
	/// </summary>
	Book(const Book& other);

	/// <summary>
	/// Gets the title of the book.
	/// </summary>
	/// <returns>String containing the title.</returns>
	const string& getTitle() const noexcept;
	/// <summary>
	/// Gets the author of the book.
	/// </summary>
	/// <returns>String containing the author name.</returns>
	const string& getAuthor() const noexcept;
	/// <summary>
	/// Gets the genre of the book.
	/// </summary>
	/// <returns>String containing the genre.</returns>
	const string& getGenre() const noexcept;
	/// <summary>
	/// Gets the realease year.
	/// </summary>
	/// <returns>Int representing the year</returns>
	const int& getYear() const noexcept;
	/// <summary>
	/// Comparison function between 2 books.
	/// </summary>
	/// <param name="other">- other book to be compared</param>
	/// <returns>Bool containing the result of comparison (1 for equality, 0 for the opposite)</returns>
	bool operator==(Book other) noexcept;

	/// <summary>
	/// Comparison function between 2 books.
	/// </summary>
	/// <param name="other">- other book to be compared</param>
	/// <returns>Bool containing the result of comparison (1 for equality, 0 for the opposite)</returns>
	bool operator==(const Book& other) const noexcept;

	/// <summary>
	/// Constructor of a book
	/// </summary>
	Book(Book&& other) noexcept;

	/// <summary>
	/// Atribution function. 'Moves' all the data from the right parameter into the left parameter, without creating some form of copy.
	/// </summary>
	Book& operator=(Book&& other) noexcept;

	/// <summary>
	/// Atribution function.
	/// </summary>
	Book& operator=(const Book& other);
};

class DTO {
private:
	string tip;
	int nr;
public:
	DTO() = default;
	DTO(const string& tip, int nr = 0);
	void increment() noexcept;
	const string& getType() const noexcept;
	const int& getNr() const noexcept;

	friend std::ostream& operator<<(std::ostream& out, const DTO& dto);
};