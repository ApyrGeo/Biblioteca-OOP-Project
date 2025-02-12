#pragma once

#include "Repository.h"
#include "UndoAction.h"
#include "Cart.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;

class Service
{
private:
	vector<UndoAction*> undoActions;
	BorrowCart borrow_cart;
	AbstractRepository& repo;

public:
	~Service();

	vector<UndoAction*> getUndoActions();

	BorrowCart& getBorrowCart() noexcept;

	/// <summary>
	/// Gets Service`s Repo.
	/// </summary>
	/// <returns>The repository of the Service object.</returns>
	AbstractRepository& getServiceRepo() noexcept;

	/// <summary>
	/// Constructor of the Service with a given Repo. 
	/// </summary>
	/// <param name="repo">The already created repo to be added to the Service object.</param>
	Service(AbstractRepository& repo);

	/// <summary>
	/// Attempts to add a book to the repo. Raises an exception if one of the given parameters are invalid.
	/// </summary>
	/// <param name="title">- title of the book</param>
	/// <param name="author">- author of the book</param>
	/// <param name="genre">- genre of the book</param>
	/// <param name="year">- release year of the book</param>
	void addBook(string title, string author, string genre, int year);

	/// <summary>
	/// Attempts to modify a book from the repo. It will modify the one at the given 'index' position.
	/// Raises an exception if one of the given parameters are invalid.
	/// </summary>
	/// <param name="index">- the index in the repo of the wanted element</param>
	/// <param name="title">- new title of the book</param>
	/// <param name="author">- new author of the book</param>
	/// <param name="genre">- new genre of the book</param>
	/// <param name="year">- new release year of the book</param>
	void modifyBook(size_t index, string title, string author, string genre, int year);
	
	/// <summary>
	/// Attempts to delete a book from the repo. It will delete the one at the 'index' position.
	/// Raises an exception if the index is invalid.
	/// </summary>
	/// <param name="index">- the index of the wanted element</param>
	void deleteBook(size_t index);
	
	/// <summary>
	/// Attempts to get all the books that contain the given title.
	/// Raises an exception if there is no book in repo matching the title.
	/// </summary>
	/// <param name="title">- title of the books to be searched</param>
	/// <returns>List of books that mathc the given input.</returns>
	vector<Book> findBook(string title, string author, string genre, int year);

	/// <summary>
	/// Filters the repo book list by title.
	/// </summary>
	/// <param name="title">- title of the filter</param>
	/// <returns>List containing all books with the given title</returns>
	vector<Book> filterBooksByTitle(string title);

	/// <summary>
	/// Filters the repo book list by year.
	/// </summary>
	/// <param name="start">- start year of search</param>
	/// <param name="final">- final year of search</param>
	/// <returns>List containing all the books that were published between the start and final years</returns>
	vector<Book> filterBooksByYear(int start, int final);

	/// <summary>
	/// Sorts the repo book list by a criteria, ascending or descending
	/// </summary>
	/// <param name="criteria">- criteria of the search ("title"/"author"/"year")</param>
	/// <param name="ascending">- 1 for ascending, 0 for descending</param>
	/// <returns>A list sorted by all the given parameters</returns>
	vector<Book> getSortedBooksByCriteria(const string& criteria, bool ascending);

	map<string, DTO> generateReport();

	void undo();

};

