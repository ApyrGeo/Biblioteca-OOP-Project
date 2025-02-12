#include "UI.h"
#include "Exception.h"
#include "Service.h"
#include <vector>
#include <map>
#include <iostream>
#include <print>
#include <string>

using std::pair;
using std::map;
using std::vector;
using std::cout;
using std::cin;
using std::string;
using std::println;

UI::UI(Service& service) noexcept :service{ service }{}

void UI::waitAndClose()
{
	println("Press Enter to continue!");
	string a;
	std::getline(cin, a);
	std::getline(cin, a);

}

void UI::printMenuAdd()
{
	clearScreen();
	char a;
	string title, author, genre, year;

	cout << "Introdu titlul cartii: ";
	cin.get(a);
	std::getline(cin, title);

	cout << "Introdu autorul cartii: ";
	//cin.get(a);
	std::getline(cin, author);
	
	cout <<  "Introdu genul cartii: ";
	//cin.get(a);
	std::getline(cin, genre);

	cout <<  "Introdu anul aparitiei al cartii: ";
	//cin.get(a);
	std::getline(cin, year);

	const int year_int = std::stoi(year);
	
	getUIService().addBook(title, author, genre, year_int);
}

void UI::printMenuModify()
{
	clearScreen();

	printBooks(getUIService().getServiceRepo().getRepoList());
	int option;
	cout << "\nAlege cartea pe care vrei sa o modifici: ";
	cin >> option;
	
	clearScreen();
	char a;
	string title, author, genre, year;

	cout << "Introdu noul titlu al cartii: ";
	cin.get(a);
	std::getline(cin, title);

	cout << "Introdu noul autor al cartii: ";
	std::getline(cin, author);

	cout << "Introdu noul gen al cartii: ";
	std::getline(cin, genre);

	cout << "Introdu noul an de aparitiei al cartii: ";
	std::getline(cin, year);

	const int year_int = std::stoi(year);

	getUIService().modifyBook(option-1, title, author, genre, year_int);
}

void UI::printMenuDelete()
{
	clearScreen();

	printBooks(getUIService().getServiceRepo().getRepoList());
	int option;
	cout << "\nAlege cartea pe care vrei sa o stergi: ";
	cin >> option;

	getUIService().deleteBook(option - 1);
}

vector<Book> UI::printMenuFind()
{
	
	clearScreen();
	string title, genre, author, year;
	cout << "Introdu titlul cartii de cautat: ";
	cin >> title;

	cout << "Introdu noul autor al cartii: ";
	std::getline(cin, author);

	cout << "Introdu noul gen al cartii: ";
	std::getline(cin, genre);

	cout << "Introdu noul an de aparitiei al cartii: ";
	std::getline(cin, year);

	const int year_int = std::stoi(year);
	return getUIService().findBook(title, author, genre, year_int);
}

void UI::printMainMenu()
{
	clearScreen();
	writeMessage(string("Produse in cos: ") + std::to_string(getUIService().getBorrowCart().getSize()) + "\n", "CYAN");
	println("Optiuni:\n");
	println("1. Adaugare carte");
	println("2. Modificare carte");
	println("3. Stergere carte");
	println("4. Cautare carte dupa nume");
	println("5. Afisare carti");
	println("6. Filtrare carti");
	println("7. Sortare carti\n");
	println("8. Gestionare cos");
	println("9. Raport\n");
	println("a. Undo\n");
	println("0. Iesire");
	cout << "\n\nOptiune: ";
}

void UI::printBooks(const vector<Book>& book_list)
{
	cout << "Lista carti: \n";
	changeTextColor("CYAN");
	int i = 0;
	for(const Book& b : book_list)
	{
		println("Cartea nr. {}, Titlu: {}, Autor: {}, Gen: {}, An aparitie: {}", i + 1, b.getTitle(), b.getAuthor(), b.getGenre(), b.getYear());
		i++;
	}
	/*for (int i = 0; i < book_list.size(); i++)
	{
		println("Cartea nr. {}, Titlu: {}, Autor: {}, Gen: {}, An aparitie: {}", i + 1, book_list.at(i).getTitle(), book_list.at(i).getAuthor(), book_list.at(i).getGenre(), book_list.at(i).getYear());
	}*/
	changeTextColor("RESET");
}

void UI::clearScreen() noexcept
{
	system("cls");
}

void UI::changeTextColor(string color)
{
	if (color == "green")
		println("\033[0;32m");
	else if (color == "GREEN")
		println("\033[1;32m");

	else if (color == "red")
		println("\033[0;31m");
	else if (color == "RED")
		println("\033[1;31m");

	else if (color == "cyan")
		println("\033[0;36m");
	else if (color == "CYAN")
		println("\033[1;36m");

	else if (color == "RESET"|| color == "white")
		println("\033[0;37m");
	else if (color == "WHITE")
		println("\033[1;37m");
}

Service& UI::getUIService() noexcept
{
	return this->service;
}
//void UI::writeMessage(string& message, string color)
//{
//	changeTextColor(color);
//	println(message);
//	changeTextColor("RESET");
//}
void UI::writeMessage(string message, string color)
{
	changeTextColor(color);
	cout << message;
	changeTextColor("RESET");
}

void UI::writeMessage(const char* message, string color)
{
	changeTextColor(color);
	cout<<message;
	changeTextColor("RESET");
}

vector<Book> UI::printMenuFilter()
{
	while (1)
	{
		clearScreen();
		println("1. Filtrare carti dupa titlu");
		println("2. Filtrare carti dupa anul aparitiei");
		println("0. Revenire");

		cout << "\nOptiune: ";
		try {
			char c;
			cin >> c;
			switch (c)
			{
			case '1':
			{
				clearScreen();
				string title;
				cout << "Introdu numele cartii: ";
				cin >> title;

				return service.filterBooksByTitle(title);

				break;
			}
			case '2':
			{
				clearScreen();
				int inceput, final;
				cout << "Introdu anul de inceput: ";
				cin >> inceput;
				cout << "Introdu anul de final: ";
				cin >> final;
				 
				return service.filterBooksByYear(inceput, final);
				
				break;
			}
			case '0':
			{
				vector<Book> a;
				a.push_back(Book{ "","","",-1 });
				return a;
				break;
			}
			default:
				throw ServiceException(string("Optiune invalida!"));
			}
		}
		catch (UIException e) {
			writeMessage(e.what(), "RED");
			waitAndClose();
		}
	}
}

bool UI::printAscDesc()
{
	char opt = 0;
	while (1)
	{
		clearScreen();
		cout << "Ascendent - 1/Descendent -0\n\nOptiune: ";
		cin >> opt;

		if (opt == '1')
			return 1;
		else if (opt == '0')
			return 0;
		else
		{
			writeMessage("Optiune invalida!", "RED");
			waitAndClose();
		}
	}
	
}

vector<Book> UI::printMenuSort()
{
	while (1)
	{
		clearScreen();
		println("1. Sortare carti dupa titlu");
		println("2. Sortare carti dupa autor");
		println("3. Sortare carti dupa an aparitie + gen");
		println("0. Revenire");

		cout << "\nOptiune: ";
		try {
			char c;
			cin >> c;
			switch (c)
			{
			case '1':
			{
				return service.getSortedBooksByCriteria("title", printAscDesc());
				break;
			}
			case '2':
			{
				return service.getSortedBooksByCriteria("author", printAscDesc());
				break;
			}
			case '3':
			{
				return service.getSortedBooksByCriteria("year", printAscDesc());
				break;
			}
			case '0':
			{
				vector<Book> a;
				a.push_back(Book{ "","","",-1 });
				return a;
				break;
			}
			default:
				throw UIException(string("Optiune invalida!"));
			}
		}
		catch (UIException e) {
			writeMessage(e.what(), "RED");
			waitAndClose();
		}
	}
}

void UI::printMenuCart()
{
	clearScreen();
	println("Optiuni:\n");
	println("1. Sterge intregul cos");
	println("2. Adauga carte in cos");
	println("3. Genereaza carti in cos");
	println("4. Export to HTML");
	println("5. Export to CSV");
	println("6. Afisare cos\n");

	println("0. Revenire");

	cout << "\nOptiune: ";
}

void UI::printMenuAddToCart()
{
	clearScreen();
	
	printBooks(getUIService().getServiceRepo().getRepoList());
	int option;
	cout << "\nAlege cartea pe care vrei sa o adaugi in cos: ";
	cin >> option;

	getUIService().getBorrowCart().addToBorrowCart(getUIService().getServiceRepo().getElementList(option-1));

}

void UI::printMenuGenerate()
{
	clearScreen();
	int nr;
	cout << "Introdu un numar de carti de generat: ";
	cin >> nr;
	getUIService().getBorrowCart().generateBorrowCart(getUIService().getServiceRepo().getRepoList(), nr);
}

void UI::console()
{
	while (true)
	{
		inceput:
		try
		{
			//clearScreen();
			printMainMenu();

			char option;
			cin >> option;
			switch (option)
			{
				case '1':
				{
					while (true)
					{
						try{
							printMenuAdd();
							break;
						}catch (Exception& e)
						{
							writeMessage(e.what(), "RED");
							waitAndClose();
						}
					}
					writeMessage("Book added", "GREEN");
					waitAndClose();
					break;
				}
				case '2':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					while (true)
					{
						try {
							printMenuModify();
							break;
						}
						catch (Exception& e){
							writeMessage(e.what(), "RED");
							waitAndClose();
						}
					}
					writeMessage("Book modified!", "GREEN");
					waitAndClose();
					break;
				}
				case '3':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					while (true)
					{
						try {
							printMenuDelete();
							break;
						}
						catch (Exception& e) {
							writeMessage(e.what(), "RED");
							waitAndClose();
						}
					}
					writeMessage("Book deleted!", "GREEN");
					waitAndClose();
					break;
				}
				case '4':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					vector<Book> found = printMenuFind();

					clearScreen();
					printBooks(found);
					waitAndClose();

					break;

				}
				case '5':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					clearScreen();
					printBooks(getUIService().getServiceRepo().getRepoList());
					waitAndClose();

					break;
				}
				case '6':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					vector<Book> found = printMenuFilter();
					if (found.size() == 0)
						throw UIException(string("Nu s-au gasit carti!"));
					if (found.at(0).getYear() == -1)
						goto inceput;

					clearScreen();
					printBooks(found);
					waitAndClose();
					break;
				}
				case '7':
				{
					if (getUIService().getServiceRepo().getRepoSize() == 0)
						throw UIException(string("Nu exista carti in lista!"));

					vector<Book> found = printMenuSort();
					
					if (found.at(0).getYear() == -1)
						goto inceput;

					clearScreen();
					printBooks(found);
					waitAndClose();
					break;
				}
				case '8':
				{
					char option2 = -1;
					while (true)
					{
						printMenuCart();
						
						cin >> option2;
						if (option2 < '0' || option2 >= '7')
						{
							writeMessage("Optiune invalida", "RED");
							waitAndClose();
						}
						else
							break;
					}
					switch (option2)
					{
						case '1':
						{
							getUIService().getBorrowCart().deleteBorrowCart();
							writeMessage("Cos golit cu succes!", "GREEN");
							waitAndClose();
							break;
						}
						case '2':
						{
							if (getUIService().getServiceRepo().getRepoSize() == 0)
								throw UIException(string("Nu exista carti in lista!"));

							while (true)
							{
								try {
									printMenuAddToCart();
									break;
								}
								catch (RepositoryException e) {
									writeMessage(e.what(), "RED");
									waitAndClose();
								}
							}
							writeMessage("Carte adaugata in cos!", "GREEN");
							waitAndClose();
							break;
						}
						case '3':
						{
							printMenuGenerate();
							writeMessage("Carti generate cu succes!", "GREEN");
							waitAndClose();
							break;
						}
						case '4':
						{
							string path;
							cout << "Introdu numele fisierului/caii: ";
							cin >> path;
							getUIService().getBorrowCart().exportToHtml(path, 1);
							writeMessage("HTML generat!", "GREEN");
							waitAndClose();
							break;

						}
						case '5':
						{
							string path;
							cout << "Introdu numele fisierului/caii: ";
							cin >> path;
							getUIService().getBorrowCart().exportToCsv(path);
							writeMessage("CSV generat!", "GREEN");
							waitAndClose();
							break;
						}
						case '6':
						{
							if (getUIService().getBorrowCart().getSize() == 0)
							{
								writeMessage("Cosul este gol!", "RED");
								waitAndClose();
								break;
							}
							clearScreen();
							printBooks(getUIService().getBorrowCart().getBorrowCartList());
							waitAndClose();
							break;
						}
						case '0':
						{
							goto inceput;
						}
						default:
						{
							writeMessage("Optiune invalida!", "RED");
							waitAndClose();
							break;
						}
					}

					break;
				}
				case '9':
				{
					clearScreen();
					cout << "Raport: \n";
					for (const pair<string, DTO>& a : getUIService().generateReport())
					{
						cout << a.second << '\n';
					}
					cout << '\n';
					waitAndClose();
					break;
				}
				case 'a':
				{
					try {
						getUIService().undo();
					}
					catch (ServiceException e) {
						writeMessage(e.what(), "RED");
						waitAndClose();
						goto inceput;
					}
					writeMessage("Undo realizat cu succes!", "GREEN");

					waitAndClose();

					break;
				}
				case '0':
				{
					clearScreen();
					writeMessage("\xB2\xB2\xB2\xB1\xB1\xB0 Console closed! \xB0\xB1\xB1\xB2\xB2\xB2", "RED");
					return;
				}
					
				default:
				{
					writeMessage("Optiune invalida!", "RED");
					waitAndClose();
					break;
				}
					
			}
		}
		catch (Exception& e)
		{
			writeMessage(e.what(), "RED");
			waitAndClose();
		}
		
	}
}
