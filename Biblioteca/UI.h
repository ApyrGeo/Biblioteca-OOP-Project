#pragma once
#include <vector>
#include "Service.h"

class UI
{
private:
	Service& service;
public:
	/// <summary>
	/// Constructor of the UI. Initialises the service with the given parameter.
	/// </summary>
	/// <param name="service">- the existing service to be added in the UI object</param>
	UI(Service& service) noexcept;

	/// <summary>
	/// Function that prints a message and awaits user input. 
	/// </summary>
	void waitAndClose();
	/// <summary>
	/// Gets all the wanted user input necessary for the service 'add' function and calls it afterwards.
	/// </summary>
	void printMenuAdd();
	/// <summary>
	/// Gets all the wanted user input necessary for the service 'modify' function and calls it afterwards.
	/// </summary>
	void printMenuModify();
	/// <summary>
	/// Gets all the wanted user input necessary for the service 'delete' function and calls it afterwards.
	/// </summary>
	void printMenuDelete();
	/// <summary>
	/// Gets all the wanted user input necessary for the service 'find' function and calls it afterwards.
	/// Returns the result of that function.
	/// </summary>
	/// <returns>The list of books received from the service function.</returns>
	vector<Book> printMenuFind();
	/// <summary>
	/// Prints the main menu of the app.
	/// </summary>
	void printMainMenu();
	/// <summary>
	/// Prints all the books of the given Book list.
	/// </summary>
	/// <param name="book_list">- the given list of Books</param>
	void printBooks(const vector<Book>& book_list);
	/// <summary>
	/// Function that clears the entire console screen.
	/// </summary>
	void clearScreen() noexcept;
	/// <summary>
	/// Function that changes the text color. After 1 use it must be recalled with the "RESET" parameter so as the text would come back to standard white.
	/// </summary>
	/// <param name="color">- color of the text, which can be: red/RED/green/GREEN/cyan/CYAN/white/WHITE</param>
	void changeTextColor(string color);
	/// <summary>
	/// Gets the service of the UI object.
	/// </summary>
	/// <returns>The wanted service.</returns>
	Service& getUIService() noexcept;
	//void writeMessage(string& message, string color);

	/// <summary>
	/// Writes a message in a custom color.
	/// </summary>
	/// <param name="message">- the given message to be printed.</param>
	/// <param name="color">- the given color of the message, that can be: red/RED/green/GREEN/cyan/CYAN/white/WHITE</param>
	void writeMessage(string message, string color);

	/// <summary>
	/// Writes a message in a custom color.
	/// </summary>
	/// <param name="message">- the given message to be printed.</param>
	/// <param name="color">- the given color of the message, that can be: red/RED/green/GREEN/cyan/CYAN/white/WHITE</param>
	void writeMessage(const char* message, string color);

	/// <summary>
	/// Prints a menu for getting the user wanted filtering option. Returns the list of the found elements.
	/// Throws an exception if an option is invalid.
	/// </summary>
	/// <returns>Filtered vector of books</returns>
	vector<Book> printMenuFilter();

	/// <summary>
	/// Awaits user for input (for ascending or descending preference)
	/// </summary>
	/// <returns>A bool representing the user order preference (1- ascending, 0- descending)</returns>
	bool printAscDesc();

	/// <summary>
	/// Prints a menu for getting the user wanted sorting option. Returns the list sorted by the given user input.
	/// Throws an exception if an option is invalid.
	/// </summary>
	/// <returns>Sorted vector of books</returns>
	vector<Book> printMenuSort();
	
	void printMenuCart();
	void printMenuAddToCart();
	void printMenuGenerate();

	/// <summary>
	/// Runs the console of the app.
	/// </summary>
	void console();
};

