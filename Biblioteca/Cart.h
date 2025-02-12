#pragma once

#include <vector>
#include "Domain.h"
#include "Observer.h"

class BorrowCart : public Observable
{
private:
	std::vector<Book> borrowList;
public:
	/// <summary>
	/// Gets the size of the cart.
	/// </summary>
	/// <returns>Number of elements in cart.</returns>
	size_t getSize() noexcept;

	/// <summary>
	/// Deletes all elements from the cart
	/// </summary>
	void deleteBorrowCart() noexcept;

	/// <summary>
	/// Adds a new element to the cart
	/// </summary>
	/// <param name="to_add">- element to be added (Book)</param>
	void addToBorrowCart(const Book& to_add);

	/// <summary>
	/// Generates a given amount of random books avaible from a list of books. 
	/// </summary>
	/// <param name="book_list">- list of books which must be randomly generated from</param>
	/// <param name="amount">- number of books to be generated</param>
	
	void generateBorrowCart(const std::vector<Book>& book_list, int amount);
	
	/// <summary>
	/// Gets the entire cart list.
	/// </summary>
	/// <returns>Vector of books that are in the cart</returns>
	std::vector<Book>& getBorrowCartList() noexcept;

	/// <summary>
	/// Exports the cart in a HTML form. If it`s missing the extension, it will be added afterwards.
	/// </summary>
	/// <param name="path">- path of the .html file</param>
	/// <param name="open_after">- 1 if you want to open the site afterwards, 0 if not</param>
	void exportToHtml(string path, bool open_after);

	/// <summary>
	/// Exports the cart in a CSV form. If it`s missing the extension, it will be added afterwards.
	/// </summary>
	/// <param name="path">- path of the .csv file</param>
	void exportToCsv(string path);
};

