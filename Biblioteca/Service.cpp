#include "Service.h"
#include "UndoAction.h"
#include "Validator.h"
#include "Exception.h"
#include <vector>
#include <map>
#include <algorithm>

using std::vector;
using std::map;

AbstractRepository& Service::getServiceRepo() noexcept
{
	return repo;
}
Service::~Service()
{
	for (UndoAction* bk : undoActions)
		delete bk;
}
vector<UndoAction*> Service::getUndoActions()
{
	return undoActions;
}
BorrowCart& Service::getBorrowCart() noexcept
{
	return borrow_cart;
}
Service::Service(AbstractRepository& repo): repo{repo}
{
	borrow_cart = BorrowCart();
}
/// lambda function: [](int a) {return a%2}
/// lambda function: [=/&](Book b1, Book b2) {return b1.getAuthor() < b2.getAuthor()}
/// 
void Service::addBook(string title, string author, string genre, int year)
{
	if (!validateTitle(title)) throw ServiceException(string("Titlu carte invalid!"));
	if (!validateAuthor(author)) throw ServiceException(string("Nume autor invalid!"));
	if (!validateGenre(genre)) throw ServiceException(string("Gen carte invalid!"));
	if (!validateYear(year)) throw ServiceException(string("An aparitie invalid"));


	getServiceRepo().addToList(Book(title, author, genre, year));

	undoActions.push_back(new UndoAdd{ getServiceRepo(), getServiceRepo().getRepoSize() - 1 });

}
void Service::modifyBook(size_t index, string title, string author, string genre, int year)
{
	if (!validateTitle(title)) throw ServiceException(string("Titlu carte invalid!"));
	if (!validateAuthor(author)) throw ServiceException(string("Nume autor invalid!"));
	if (!validateGenre(genre)) throw ServiceException(string("Gen carte invalid!"));
	if (!validateYear(year)) throw ServiceException(string("An aparitie invalid"));
	if (index >= getServiceRepo().getRepoSize() || index < 0) throw ServiceException(string("Index invalid!"));
	
	undoActions.push_back(new UndoModify{ getServiceRepo(), index, getServiceRepo().getElementList(index)});

	getServiceRepo().modifyInList(index, Book(title, author, genre, year));

}
void Service::deleteBook(size_t index)
{
	if (index >= getServiceRepo().getRepoSize() || index < 0) throw ServiceException(string("Index invalid!"));
	
	undoActions.push_back(new UndoDelete{getServiceRepo(), index,  getServiceRepo().getElementList(index)});

	getServiceRepo().deleteFromList(index);
}
vector<Book> Service::findBook(string title, string author, string genre, int year)
{
	vector<Book> received = getServiceRepo().getRepoList();
	Book to_find {title, author, genre, year};
	//std::copy_if(getServiceRepo().getRepoList().begin(), getServiceRepo().getRepoList().end(), std::back_inserter(received))
	std::erase_if(received, [to_find](const Book& bk) noexcept {return bk != to_find; }); //<--

	/*for (int i = 0; i < received.size(); i++)
	{
		if (!(received.at(i) == to_find))
		{
			received.erase(received.begin()+ i);
			i--;
		}
	}*/
	if (received.size() == 0)
		throw ServiceException(string("Nu exista cartea dorita!"));

	return received;
}
vector<Book> Service::filterBooksByTitle(string title)
{
	vector<Book> received = getServiceRepo().getRepoList();

	std::erase_if(received, [title](const Book& bk) noexcept {return bk.getTitle() != title; }); //<--

	/*for (int i = 0; i < received.size(); i++)
	{
		if (!(received.at(i).getTitle() == title))
		{
			received.erase(received.begin() + i);
			i--;
		}
	}*/

	/*if (received.size() == 0)
		throw ServiceException(string("Nu exista carti!"));*/

	return received;
}

vector<Book> Service::filterBooksByYear(int start, int final)
{
	vector<Book> received = getServiceRepo().getRepoList();

	std::erase_if(received, [start, final](const Book& bk) noexcept {return !(start <= bk.getYear() && bk.getYear() <= final); }); //<--

	/*for (int i = 0; i < received.size(); i++)
	{
		if (!(start <= received.at(i).getYear() && received.at(i).getYear() <= final))
		{
			received.erase(received.begin() + i);
			i--;
		}
	}*/

	/*if (received.size() == 0)
		throw ServiceException(string("Nu exista carti!"));*/

	return received;
}



template<typename Lambda>
static void sortBooks(vector<Book>& v, Lambda lambda)
{
	bool changed = 1;
	while (changed)
	{
		changed = 0;
		for (size_t i = 0; i < v.size() - 1; i++)
		{
			if (lambda(v.at(i), v.at(i + 1)) == 0)
			{
				changed = 1;
				std::swap(v.at(i), v.at(i+1));
				/*Book aux = v.at(i);
				v.at(i) = v.at(i + 1);
				v.at(i + 1) = aux;*/
			}
		}
	}
}

vector<Book> Service::getSortedBooksByCriteria(const string& criteria, bool ascending)
{
	//vector<Book> received = getServiceRepo().getRepoList();
	vector<Book> received2 = getServiceRepo().getRepoList();
	if (criteria == "title" && ascending)
	{
		sort(received2.begin(),received2.end(), [](const Book& a, const Book& b) {return a.getTitle() < b.getTitle(); });
	}
	else if (criteria == "title" && !ascending)
	{
		sort(received2.begin(), received2.end(), [](const Book& a, const Book& b) {return a.getTitle() > b.getTitle(); });
	}
	else if (criteria == "author" && ascending)
	{
		sort(received2.begin(), received2.end(), [](const Book& a, const Book& b) {return a.getAuthor() < b.getAuthor(); });
	}
	else if (criteria == "author" && !ascending)
	{
		sort(received2.begin(), received2.end(), [](const Book& a, const Book& b) {return a.getAuthor() > b.getAuthor(); });
	}
	else if (criteria == "year" && ascending)
	{
		sort(received2.begin(), received2.end(), [](const Book& a, const Book& b) noexcept {if (a.getYear() == b.getYear()) { return a.getGenre() < b.getGenre(); } return a.getYear() <= b.getYear(); });
	}
	else if (criteria == "year" && !ascending)
	{
		sort(received2.begin(), received2.end(), [](const Book& a, const Book& b) noexcept {if (a.getYear() == b.getYear()) { return a.getGenre() > b.getGenre(); } return a.getYear() >= b.getYear(); });
	}
	else
		throw ServiceException(string("Invalid criteria!"));

	return received2;
}

map<string, DTO> Service::generateReport()
{
	std::map<string, DTO> raport;
	for (const Book& bk : getServiceRepo().getRepoList())
	{
		if (raport.count(bk.getGenre()) == 0)
		{
			raport[bk.getGenre()] = DTO{ bk.getGenre(), 1 };
		}
		else
		{
			raport[bk.getGenre()].increment();
		}
	}
	return raport;
}


void Service::undo()
{
	if (undoActions.empty())
		throw ServiceException(string("Nu exista operatii precedente!"));
	
	UndoAction* act = undoActions.back();
	act->doUndo();
	undoActions.pop_back();
	delete act;
}