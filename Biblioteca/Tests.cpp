#include <string>
#include <cassert>
#include "Cart.h"
#include "Exception.h"
#include "Domain.h"
#include "Repository.h"
#include "Service.h"
#include "UndoAction.h"
#include "Validator.h"
#include <vector>
#include <fstream>
#include <iostream>

using std::vector;

void testValidators()
{
	assert(validateAuthor("Mihai Eminescu") == 1);
	assert(validateAuthor("Mihai Eminescu2") == 0);
	assert(validateAuthor("Calinescu-George") == 1);

	assert(validateGenre("R0man") == 0);
	assert(validateGenre("Roman") == 1);

	assert(validateTitle("") == 0);
	assert(validateTitle("Morometii") == 1);

	assert(validateYear(2024) == 1);
	assert(validateYear(-1) == 0);
}
void testDomain()
{
	Book book{ "Ion","Liviu Rebreanu","Roman",1918};
	assert(book.getAuthor() == "Liviu Rebreanu");
	assert(book.getGenre() == "Roman");
	assert(book.getTitle() == "Ion");
	assert(book.getYear() == 1918);
	Book book2{ book };
	assert(book2 == book);
}
void testRepository()
{
	Repository repo = Repository();

	repo.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918));

	/*try{
		repo.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918)), assert(false);
	}catch (std::exception&){
		assert(true);
	}*/

	assert(repo.getRepoSize() == 1);
	repo.addToList(Book("Ciuleandra", "Liviu Rebreanu", "Roman", 1907));
	assert(repo.getRepoSize() == 2);

	repo.modifyInList(0, Book("Maitreyi", "Mircea Eliade", "Roman", 1937));
	assert(repo.getElementList(0).getTitle() == "Maitreyi");

	repo.deleteFromList(0);
	assert(repo.getRepoSize() == 1);
	repo.deleteFromList(0);
	assert(repo.getRepoSize() == 0);

	Repository aux = Repository();
	aux.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918));
	repo.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918));

	//assert(aux == repo);
	
	
	aux.addToList(Book("Ciuleandra", "Liviu Rebreanu", "Roman", 1917));
	//assert(!(repo == aux));
	repo.addToList(Book("Ciuleandra", "Liviu Rebreanu", "Roman", 1907));
	//assert(!(repo == aux));

	try {
		assert(aux.getElementList(4) == Book ("","","",0));
	}
	catch (RepositoryException e) {
		assert(e.what() == "Index invalid!");
	}

	repo.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918));
	assert(repo.getRepoSize() == 3);
	repo.addToListOnPosition(2, Book("a", "a", "a", 1234));
	assert(repo.getRepoSize() == 4);
	assert(repo.getElementList(2).getTitle() == "a");

	try {
		repo.addToListOnPosition(10, Book("a", "a", "b", 1234));
	}
	catch (RepositoryException e)
	{
		assert(e.what() == "Index invalid!");
	}
}
void testService()
{

	Repository repo = Repository();
	Service service = Service(repo);

	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	assert(service.getServiceRepo().getRepoSize() == 1);
	service.addBook("Ciuleandra", "Liviu Rebreanu", "Roman", 1907);
	assert(service.getServiceRepo().getRepoSize() == 2);

	service.modifyBook(1, "Maitreyi", "Mircea Eliade", "Roman", 1930);
	assert(service.getServiceRepo().getElementList(1) == Book("Maitreyi", "Mircea Eliade", "Roman", 1930));

	service.deleteBook(1);
	assert(service.getServiceRepo().getRepoSize() == 1);
	service.deleteBook(0);
	assert(service.getServiceRepo().getRepoSize() == 0);


	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1919);

	assert(service.findBook("Ion", "Liviu Rebreanu", "Roman", 1918).at(0).getYear() == 1918);

	try { 
		assert(service.findBook("Noi", "Liviu Rebreanu", "Roman", 1918).at(1).getYear() == 1919);
	}catch (ServiceException e) { 
		assert(e.what() == "Nu exista cartea dorita!");
	}
	//Filters
	assert(service.filterBooksByTitle("Ion").size() == 2);
	assert(service.filterBooksByTitle("Ion").at(1).getYear() == 1919);
	try {
		assert(service.filterBooksByTitle("Iossasn").at(1).getYear() == 1919);
	}catch(ServiceException e){
		assert(e.what() == "Nu exista cartea dorita!");
	}

	assert(service.filterBooksByYear(1917, 1920).size() == 2);
	assert(service.filterBooksByYear(1918, 1918).size() == 1);
	try {
		assert(service.filterBooksByYear(2000,3000).at(1).getYear() == 1919);
	}catch (ServiceException e) {
		assert(e.what() == "Nu exista cartea dorita!");
	}

	//Sorts
	service.addBook("Maitreyi", "Mircea Eliade", "Roman", 1930);
	service.addBook("Baltagul", "Mihail Sadoveanu", "Roman", 1918);
	service.addBook("Baltagul", "Mihail Sadoveanu", "Roman", 1920);

	{
		vector<Book> expected;

		expected.push_back(Book("Baltagul", "Mihail Sadoveanu", "Roman", 1918));
		expected.push_back(Book("Baltagul", "Mihail Sadoveanu", "Roman", 1920));
		expected.push_back(Book("Ion", "Liviu Rebreanu", "Roman", 1918));
		expected.push_back(Book("Ion", "Liviu Rebreanu", "Roman", 1919));
		expected.push_back(Book("Maitreyi", "Mircea Eliade", "Roman", 1930));

		vector<Book> received = service.getSortedBooksByCriteria("title", 1);
		assert(received == expected);
	}
	{
		vector<Book> expected;

		expected.emplace_back("Maitreyi", "Mircea Eliade", "Roman", 1930);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1918);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1919);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1918);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1920);

		assert(service.getSortedBooksByCriteria("title", 0) == expected);
	}
	{
		vector<Book> expected;
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1918);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1919);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1918);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1920);
		expected.emplace_back("Maitreyi", "Mircea Eliade", "Roman", 1930);

		assert(service.getSortedBooksByCriteria("author", 1) == expected);
	}
	{
		vector<Book> expected;
		expected.emplace_back("Maitreyi", "Mircea Eliade", "Roman", 1930);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1918);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman", 1920);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1918);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1919);

		assert(service.getSortedBooksByCriteria("author", 0) == expected);
	}
	Repository repo2 = Repository();
	Service service2 = Service(repo2);
	service2.addBook("Ion", "Liviu Rebreanu", "Roman", 1919);
	service2.addBook("Maitreyi", "Mircea Eliade", "Roman", 1930);
	service2.addBook("Baltagul", "Mihail Sadoveanu", "Roman al transhumantei", 1930);
	service2.addBook("Ion", "Liviu Rebreanu", "Roman realist", 1918);
	service2.addBook("Baltagul", "Mihail Sadoveanu", "Roman traditional", 1918);
	
	{
		vector<Book> expected;
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman realist", 1918);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman traditional", 1918);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1919);
		expected.emplace_back("Maitreyi", "Mircea Eliade", "Roman", 1930);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman al transhumantei", 1930);

		assert(service2.getSortedBooksByCriteria("year", 1) == expected);
	}
	{
		vector<Book> expected;
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman al transhumantei", 1930);
		expected.emplace_back("Maitreyi", "Mircea Eliade", "Roman", 1930);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman", 1919);
		expected.emplace_back("Baltagul", "Mihail Sadoveanu", "Roman traditional", 1918);
		expected.emplace_back("Ion", "Liviu Rebreanu", "Roman realist", 1918);

		assert(service2.getSortedBooksByCriteria("year", 0) == expected);

	}
	try {
		assert(service.getSortedBooksByCriteria("das", 1) == vector<Book>());
	}catch (ServiceException) {
		assert(true);
	}

	assert(service.getBorrowCart().getSize() == 0);

}
void testCart()
{
	BorrowCart brcrt;
	brcrt.addToBorrowCart(Book("Baltagul", "Mihail Sadoveanu", "Roman al transhumantei", 1930));
	brcrt.addToBorrowCart(Book());
	assert(brcrt.getSize() == 2);

	brcrt.deleteBorrowCart();
	assert(brcrt.getSize() == 0);

	Repository repo = Repository();
	Service service = Service(repo);
	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1919);
	service.addBook("Maitreyi", "Mircea Eliade", "Roman", 1930);
	service.addBook("Enigma Otiliei", "G. Calinescu", "Roman", 1938);
	brcrt.generateBorrowCart(service.getServiceRepo().getRepoList(), 90);
	assert(brcrt.getSize() == 90);

	{
		brcrt.exportToHtml("Data/test.html", 0);
		std::ofstream f("Data/test.html");
		assert(f.bad() == 0);
		f.close();
	}

	{
		brcrt.exportToCsv("Data/test.csv");
		std::ofstream f("Data/test.csv");
		assert(f.bad() == 0);
		f.close();
	}

}


//void testvector()
//{
//	vector<int> list;
//	assert(list.capacity() == 0); //initialise + resize
//	assert(list.size() == 0);
//	list.push_back(1);
//	list.push_back(2);
//	list.push_back(3);
//
//	assert(list.capacity() == 4);
//	assert(list.size() == 3);
//
//	vector<int> list2; 
//	list2 = list;
//	assert(list2.capacity() == 4);
//	assert(list2.size() == 3);
//	
//
//	try {
//		assert(list.at(5) == 6);
//	}
//	catch (std::exception e) {
//		assert(true);
//	}
//
//	const vector<int> list3 = list2;
//
//	assert(list3.at(1) == 2);
//	try {
//		assert(list3.at(5) == 6);
//	}
//	catch (std::exception e) {
//		assert(true);
//	}
//}
void testReport()
{
	Repository repo = Repository();
	Service service = Service(repo);

	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1919);
	service.addBook("Maitreyi", "Mircea Eliade", "Roman", 1930);
	service.addBook("Enigma Otiliei", "G. Calinescu", "Roman", 1938);
	service.addBook("Poezii", "M. Eminescu", "Poezii", 1888);

	map<string, DTO> report = service.generateReport();
	assert(report.size() == 2);

	std::cout << report["Roman"];
}
void testFileRepo()
{
	FileRepository repo("Data/teste.csv");
	assert(repo.getPath() == "Data/teste.csv");
	assert(repo.getRepoSize() == 1);
	repo.deleteFromList(0);

	repo.addToList(Book("Ion","Liviu Rebreanu","Roman",1918));
	assert(repo.getElementList(0).getYear() == 1918);

	repo.modifyInList(0, Book("Ion", "Liviu Rebreanu", "Roman", 1919));
	assert(repo.getElementList(0).getYear() != 1918);

	repo.deleteFromList(0);
	assert(repo.getRepoSize() == 0);

	repo.addToList(Book("Ion", "Liviu Rebreanu", "Roman", 1918));//reinitializare file
}
void testUndo()
{
	UndoAction* undo_operation;
	Repository repo = Repository();
	Service service = Service(repo);
	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	undo_operation = new UndoAdd{ service.getServiceRepo(), 0 };
	assert(service.getServiceRepo().getRepoSize() == 1);

	undo_operation->doUndo();
	assert(service.getServiceRepo().getRepoSize() == 0);
	delete undo_operation;

	service.addBook("Ion", "Liviu Rebreanu", "Roman", 1918);
	service.modifyBook(0, "Ion", "Liviu Rebreanu", "Roman", 1919);
	undo_operation = new UndoModify{service.getServiceRepo(), 0, Book("Ion", "Liviu Rebreanu", "Roman", 1918)};
	assert(service.getServiceRepo().getElementList(0).getYear() == 1919);
	undo_operation->doUndo();
	assert(service.getServiceRepo().getElementList(0).getYear() == 1918);
	delete undo_operation;

	service.deleteBook(0);
	undo_operation = new UndoDelete{ service.getServiceRepo(), 0, Book("Ion", "Liviu Rebreanu", "Roman", 1918) };
	assert(service.getServiceRepo().getRepoSize() == 0);
	undo_operation->doUndo();
	assert(service.getServiceRepo().getRepoSize() == 1);
	delete undo_operation;

	
	Repository r = Repository();
	Service s = Service(r);
	s.addBook("a","a","a", 1234);
	assert(s.getUndoActions().size() == 1);
	s.undo();
	assert(s.getUndoActions().size() == 0);

	try {
		s.undo(); assert(false);
	}
	catch (ServiceException e)
	{
		assert(e.what() == string("Nu exista operatii precedente!"));
	}
}

void testRepoNou()
{
	MapRepository mr = MapRepository(0);
	mr.addToList(Book("a", "a", "a", 1234));
	mr.addToList(Book("b", "b", "b", 4321));

	assert(mr.getRepoSize() == 2);

	mr.modifyInList(1, Book("c", "c", "c", 4231));
	assert(mr.getElementList(1).getYear() == 4231);

	mr.deleteFromList(1);
	assert(mr.getRepoSize() == 1);

	mr.addToListOnPosition(1, Book("b", "b", "b", 4321));
	assert(mr.getRepoSize() == 2);
	assert(mr.getElementList(1).getTitle() == "b");

	assert(mr.getRepoList().size() == 2);

	mr.deleteFromList(0);
	mr.deleteFromList(1);
	assert(mr.getRepoSize() == 0);

	Service s = Service(mr);

	s.addBook("a", "a", "a", 1234);
	assert(s.getServiceRepo().getRepoSize() == 1);
	s.undo();
	assert(s.getServiceRepo().getRepoSize() == 0);


	s.addBook("a", "a", "a", 1234);
	s.deleteBook(0);
	s.undo();
	assert(s.getServiceRepo().getRepoSize() == 1);

	MapRepository mm = MapRepository(100);
	try {
		mm.addToList(Book("a", "a", "a", 1111));
		assert(false);
	}
	catch(RepositoryException e){
		assert(true);
	}

}
void testAll()
{
	testValidators();
	testDomain();
	testRepository();
	testService();
	testCart();
	testReport();
	testFileRepo();
	testUndo();
	testRepoNou();

	//testvector();
}