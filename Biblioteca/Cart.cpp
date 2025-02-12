#include "Cart.h"
#include <print>
#include <fstream>
#include "Service.h"
#include <random>

//using namespace std::string_literals; // pentru stringul de scriere in HTML

size_t BorrowCart::getSize() noexcept
{
	return getBorrowCartList().size();
}

void BorrowCart::deleteBorrowCart() noexcept
{
	getBorrowCartList().clear();
	notify();
}
void BorrowCart::addToBorrowCart(const Book& to_add)
{
	getBorrowCartList().push_back(to_add);
	notify();
}
void BorrowCart::generateBorrowCart(const std::vector<Book>& book_list, int amount)
{
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> dist(0, int(book_list.size() - 1));

	for (int i = 0; i < amount; i++)
	{
		const int rndNr = dist(mt);// numar aleator intre [0,size-1]
		addToBorrowCart(book_list.at(rndNr));
	}
	notify();
}
std::vector<Book>& BorrowCart::getBorrowCartList() noexcept
{
	return this->borrowList;
}

void BorrowCart::exportToHtml(string path, bool open_after)
{
	if (!path.contains(".html")) path += ".html";

	std::ofstream f(path);

	std::string content;

	content = string("")
		+ "<html>\n"
		+ "<style>\n"
		+ "	.center {\n"
		+ "	margin-left:auto;\n"
		+ "	margin-right:auto;\n"
		+ "}\n"
		+ "	table, th, td{\n"
		+ "  align - self: center;\n"
		+ "  border: 2px solid black;\n"
		+ "	th, td{\n"
		+ "  padding: 5px 20px 5px 20px;\n"
		+ "  font-size:20px;\n"
		+ "}\n"
		+ "</style>\n"
		+ "	<body>\n"
		+ "		<h1 style=text-align:center>Borrows Cart</h1>\n"
		+ "		<br><br>\n"
		+ "		<table class=\"center\">\n"
		+ "			<tr>\n"
		+ "				<th>Nr.Crt.</th>\n"
		+ "				<th>Titlu</th>\n"
		+ "				<th>Autor</th>\n"
		+ "				<th>Gen</th>\n"
		+ "				<th>An aparitie</th>\n"
		+ "			</tr>\n";

	int i = 0;
	for (const Book& b : getBorrowCartList())
	{
		++i;
		content += string("")
		+ "			<tr>\n"
		+ "				<td>" + std::to_string(i) + "</td>\n"
		+ "				<td>" + b.getTitle() + "</td>\n"
		+ "				<td>" + b.getAuthor() + "</td>\n"
		+ "				<td>" + b.getGenre() + "</td>\n"
		+ "				<td>" + std::to_string(b.getYear()) + "</td>\n"
		+ "			</tr>\n";
	}
	content+= string("")
	+"		</table>\n"
	+"	</body>\n"
	+"</html>\n";
	

	f << content;
	f.close();
	
	string cmd = string("start ") + path;
	if(open_after) system(cmd.data()); 
}

void BorrowCart::exportToCsv(string path)
{
	if (!path.contains(".csv")) path += ".csv";
	std::ofstream f(path);
	int i = 0;
	for (const Book& b : getBorrowCartList())
	{
		++i;
		std::println(f, "{},{},{},{},{}", i, b.getTitle(), b.getAuthor(), b.getGenre(), b.getYear());
	}
	f.close();
}
