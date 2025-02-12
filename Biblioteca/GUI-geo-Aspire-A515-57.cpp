#include "GUI.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmdiarea.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qlineedit.h>
#include <qinputdialog.h>
#include <qdebug.h>
#include <vector>
#include "Service.h"
#include "Validator.h"
#include "Exception.h"
#include <QtWidgets/qmessagebox.h>
#include "qevent.h"
#include "qstring.h"

void MainWindow::initMainWindow()
{
	setWindowIcon(QIcon("Images/icon.ico"));
	QMenuBar* mbar = new QMenuBar();
	mbar->setStyleSheet("background-color: #2D3631");
	QMenu* m1 = new QMenu("Optiuni");
	generateReport = new QAction("&Genereaza raport\t");
	undo = new QAction("&Undo\t");
	m1->addAction(generateReport);
	m1->addAction(undo);
	mbar->addMenu(m1);
	setMenuBar(mbar);


	QWidget* main = new QWidget();
	main->setMinimumHeight(500);
	main->setAutoFillBackground(true);
	QPalette p = QPalette();
	p.setColor(QPalette::Window, "#4C5A53");
	main->setPalette(p);
	button_add = new QPushButton("&Adauga carte");
	button_modify = new QPushButton("&Modifica carte");
	button_delete = new QPushButton("&Sterge carte");
	button_cart = new QPushButton("&Gestionare cos");
	button_reinit = new QPushButton("&Reinitializare lista carti");
	combobox_filter_sort = new QComboBox();
	listview_books = new QListWidget();

	button_add->setStyleSheet("width: 200px;height:30px;font-size:15px");
	button_modify->setStyleSheet("width: 200px;height:30px;font-size:15px");
	button_delete->setStyleSheet("width: 200px;height:30px;font-size:15px");
	combobox_filter_sort->setStyleSheet("width: 200px;height:30px;font-size:15px");

	button_cart->setStyleSheet("width: 200px;height:30px;font-size:15px");
	listview_books->setMinimumWidth(600);
	listview_books->setAlternatingRowColors(true);
	//listview_books->setStyleSheet("background-color:blue; alternate-background-color:red");

	combobox_filter_sort->addItem("--Selecteaza tipul--");
	combobox_filter_sort->addItems({"Filtrare dupa titlu","Filtrare dupa anul aparitiei","Sortare dupa titlu (↑)","Sortare dupa titlu (↓)", "Sortare dupa autor (↑)", "Sortare dupa autor (↓)","Sortare dupa an+gen (↑)", "Sortare dupa an+gen (↓)" });

	QHBoxLayout* layout_main = new QHBoxLayout();

	QVBoxLayout* layout_left = new QVBoxLayout();
	layout_left->addWidget(button_add);
	layout_left->addWidget(button_modify);
	layout_left->addWidget(button_delete);
	layout_left->addSpacing(30);
	layout_left->addWidget(combobox_filter_sort);

	layout_main->addLayout(layout_left);

	QVBoxLayout* layout_right = new QVBoxLayout();
	layout_right->addWidget(listview_books);
	layout_right->addSpacing(20);
	layout_right->addWidget(button_reinit);
	layout_right->addWidget(button_cart);

	layout_main->addLayout(layout_right);

	main->setLayout(layout_main);

	updateListView(service.getServiceRepo().getRepoList());

	this->setCentralWidget(main);
}
void MainWindow::connectSignalSlots()
{
	QObject::connect(button_add, &QPushButton::clicked, [&]() {
		AddWindow* w = new AddWindow(service);
		QObject::connect(w, &AddWindow::carteAdaugata, [&]() {updateListView(service.getServiceRepo().getRepoList()); });
		w->show();
		});

	QObject::connect(button_modify, &QPushButton::clicked, [&]() {
		if (listview_books->currentRow() == -1)
			QMessageBox::information(this, "Eroare", "Selecteaza mai intai cartea pe care vrei sa o modifici din tabel!");
		else
		{
			ModifyWindow* w = new ModifyWindow(this->service, size_t(listview_books->currentRow()));
			QObject::connect(w, &ModifyWindow::carteModificata, [&]() {updateListView(service.getServiceRepo().getRepoList()); });
			w->show();
		}});

	QObject::connect(button_delete, &QPushButton::clicked, [&]() {
		if (listview_books->currentRow() == -1)
			QMessageBox::information(this, "Eroare", "Selecteaza mai intai cartea pe care vrei sa o stergi din tabel!");
		else
		{
			service.deleteBook(listview_books->currentRow());
			updateListView(service.getServiceRepo().getRepoList());
		}});

	QObject::connect(combobox_filter_sort, &QComboBox::currentIndexChanged, [&]() {
		switch (combobox_filter_sort->currentIndex())
		{
		case 0:
		{
			QMessageBox::warning(this, "Eroare", "Selecteaza un tip de sortare/filtrare.");
			break;
		}
		case 1:
		{
			QString title = QInputDialog::getText(this, "Dialog", "Introdu titlul filtrarii!");
			updateListView(service.filterBooksByTitle(title.toStdString()));
			break;
		}
		case 2:
		{
			int start, final;
			while (1)
			{
				start = QInputDialog::getText(this, "Dialog", "Introdu anul de inceput al filtrarii!").toInt();
				if (validateYear(start)) break;
				QMessageBox::warning(this, "Eroare", "An invalid!");
			}
			while (1)
			{
				final = QInputDialog::getText(this, "Dialog", "Introdu anul de final al filtrarii!").toInt();
				if (validateYear(final)) break;
				QMessageBox::warning(this, "Eroare", "An invalid!");
			}

			updateListView(service.filterBooksByYear(start, final));
			break;
		}
		case 3:
		{
			updateListView(service.getSortedBooksByCriteria("title", 0));
			break;
		}
		case 4:
		{
			updateListView(service.getSortedBooksByCriteria("title", 1));
			break;
		}
		case 5:
		{
			updateListView(service.getSortedBooksByCriteria("author", 0));
			break;
		}
		case 6:
		{
			updateListView(service.getSortedBooksByCriteria("author", 1));
			break;
		}
		case 7:
		{
			updateListView(service.getSortedBooksByCriteria("year", 0));
			break;
		}
		case 8:
		{
			updateListView(service.getSortedBooksByCriteria("year", 1));
			break;
		}
		}
		});

	QObject::connect(button_reinit, &QPushButton::clicked, [&]() {updateListView(service.getServiceRepo().getRepoList()); });

	QObject::connect(undo, &QAction::triggered, [&]() {
		try
		{
			service.undo();
			updateListView(service.getServiceRepo().getRepoList());
		}
		catch (Exception& e)
		{
			QMessageBox::information(this, "Eroare", QString::fromStdString(e.what()));
		}});

	QObject::connect(generateReport, &QAction::triggered, [&]() {
		QString to_show;
		for (const std::pair<string,DTO>& a : service.generateReport())
		{
			to_show += QString::fromStdString(a.first) + ":    \t" + QString::number(a.second.getNr()) + "\t\n";
		}
		QMessageBox::information(this, "Raport", to_show);

		});

	QObject::connect(button_cart, &QPushButton::clicked, [&]() {(new CartWindow(service))->show(); });
}
void MainWindow::updateListView(const std::vector<Book>& v)
{
	listview_books->clear();
	int k = 0;
	for (const Book& a : v)
	{
		QListWidgetItem* q = new QListWidgetItem(QString(
			QString::fromStdString(a.getTitle()) + "\t\t" +
			QString::fromStdString(a.getAuthor()) + "\t\t" +
			QString::fromStdString(a.getGenre()) + "\t\t" +
			QString::number(a.getYear())));
		q->setFont(QFont(this->font().toString(), -5, 100, false));

		listview_books->insertItem(k++, q);
	}
}
MainWindow::MainWindow(Service& service) :QMainWindow::QMainWindow{}, service{ service }
{
	initMainWindow();
	connectSignalSlots();
}

void AddWindow::initAddWindow()
{

	lineedit_title = new QLineEdit();
	lineedit_author = new QLineEdit();
	lineedit_genre = new QLineEdit();
	lineedit_year = new QLineEdit();
	button_submit = new QPushButton("&Adauga");

	QWidget* main = new QWidget();
	main->setWindowTitle("Add Book");

	QVBoxLayout* layout_main = new QVBoxLayout();

	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->addWidget(new QLabel("Title:  \t"));
	layout1->addSpacing(5);
	layout1->addWidget(lineedit_title);
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->addWidget(new QLabel("Author: \t"));
	layout2->addSpacing(5);
	layout2->addWidget(lineedit_author);
	QHBoxLayout* layout3 = new QHBoxLayout();
	layout3->addWidget(new QLabel("Genre:  \t"));
	layout3->addSpacing(5);
	layout3->addWidget(lineedit_genre);
	QHBoxLayout* layout4 = new QHBoxLayout();
	layout4->addWidget(new QLabel("Year:   \t"));
	layout4->addSpacing(5);
	layout4->addWidget(lineedit_year);

	layout_main->addLayout(layout1);
	layout_main->addLayout(layout2);
	layout_main->addLayout(layout3);
	layout_main->addLayout(layout4);

	layout_main->addWidget(button_submit);

	main->setLayout(layout_main);

	setCentralWidget(main);
}
void AddWindow::connectSignalSlots()
{
	QObject::connect(button_submit, &QPushButton::clicked, [&]() 
		{
			try {
				service.addBook(lineedit_title->text().toStdString(),
								lineedit_author->text().toStdString(),
								lineedit_genre->text().toStdString(),
								atoi(lineedit_year->text().toStdString().c_str()));
				int r = QMessageBox::information(this, "Info", "Carte adaugata cu succes!", QMessageBox::Ok);
				qDebug() << "->> ADD";

				///reinterpret_cast<MainWindow*>(parent())->update();
				emit carteAdaugata();
				close();
			}
			catch (ServiceException e)
			{
				QMessageBox::information(this,"ServiceException",QString::fromStdString(e.what()));
			}
		});
}

AddWindow::AddWindow(/*MainWindow& main,*/ Service& service) : /*QMainWindow::QMainWindow{&main, Qt::Window},*/ service{service}
{
	initAddWindow();
	connectSignalSlots();
}
void ModifyWindow::initModifyWindow()
{
	lineedit_title = new QLineEdit();
	lineedit_author = new QLineEdit();
	lineedit_genre = new QLineEdit();
	lineedit_year = new QLineEdit();
	button_submit = new QPushButton("&Modifica");

	QWidget* main = new QWidget();
	main->setWindowTitle("Modify Book");

	QVBoxLayout* layout_main = new QVBoxLayout();

	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->addWidget(new QLabel("New Title:  \t"));
	layout1->addSpacing(5);
	layout1->addWidget(lineedit_title);
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->addWidget(new QLabel("New Author: \t"));
	layout2->addSpacing(5);
	layout2->addWidget(lineedit_author);
	QHBoxLayout* layout3 = new QHBoxLayout();
	layout3->addWidget(new QLabel("New Genre:  \t"));
	layout3->addSpacing(5);
	layout3->addWidget(lineedit_genre);
	QHBoxLayout* layout4 = new QHBoxLayout();
	layout4->addWidget(new QLabel("New Year:   \t"));
	layout4->addSpacing(5);
	layout4->addWidget(lineedit_year);

	layout_main->addLayout(layout1);
	layout_main->addLayout(layout2);
	layout_main->addLayout(layout3);
	layout_main->addLayout(layout4);

	Book b = service.getServiceRepo().getElementList(this->index);

	lineedit_title->setText(QString::fromStdString(b.getTitle()));
	lineedit_author->setText(QString::fromStdString(b.getAuthor()));
	lineedit_genre->setText(QString::fromStdString(b.getGenre()));
	lineedit_year->setText(QString::number(b.getYear()));

	layout_main->addWidget(button_submit);

	main->setLayout(layout_main);

	setCentralWidget(main);
}
void ModifyWindow::connectSignalSlots()
{
	QObject::connect(button_submit, &QPushButton::clicked, [&]()
		{
			try {
				service.modifyBook(index,lineedit_title->text().toStdString(),
					lineedit_author->text().toStdString(),
					lineedit_genre->text().toStdString(),
					atoi(lineedit_year->text().toStdString().c_str()));
				int r = QMessageBox::information(this, "Info", "Carte modificata cu succes!", QMessageBox::Ok);
				qDebug() << "->> MODIFY";
				//reinterpret_cast<MainWindow*>(parent())->update();

				emit carteModificata();
				close();
			}
			catch (ServiceException e)
			{
				QMessageBox::information(this, "ServiceException", QString::fromStdString(e.what()));
			}
		});
}

ModifyWindow::ModifyWindow(Service& service, size_t index):service{service}, index{index}
{
	initModifyWindow();
	connectSignalSlots();
}

void CartWindow::initCartWindow()
{
	setWindowIcon(QIcon("Images/icon.ico"));

	setMinimumSize(1500, 500);
	QMenuBar* mbar = new QMenuBar();
	QMenu* m1 = new QMenu("Salvare");
	saveToCsv = new QAction("Save to CSV");
	saveToHtml = new QAction("Save to HTML");

	mbar->setStyleSheet("background-color: #5A6267");

	m1->addAction(saveToCsv);
	m1->addAction(saveToHtml);
	mbar->addMenu(m1);
	this->setMenuBar(mbar);

	button_add = new QPushButton("&Adauga carte");
	button_generate = new QPushButton("&Genereaza carti");
	button_delete = new QPushButton("&Sterge intregul cos");
	list_books = new QListWidget();
	list_cart = new QListWidget();
	label_count = new QLabel();

	button_add->setStyleSheet("width: 200px;height:30px;font-size:15px");
	button_generate->setStyleSheet("width: 200px;height:30px;font-size:15px");
	button_delete->setStyleSheet("width: 200px;height:30px;font-size:15px");


	QWidget* main = new QWidget();

	main->setAutoFillBackground(true);
	QPalette p = QPalette();
	p.setColor(QPalette::Window, "#6C767C");
	main->setPalette(p);

	QHBoxLayout* layout_main = new QHBoxLayout();
	QVBoxLayout* layout_buttons = new QVBoxLayout();
	layout_buttons->addWidget(button_add);
	layout_buttons->addWidget(button_generate);
	layout_buttons->addWidget(button_delete);
	
	layout_main->addLayout(layout_buttons);

	QVBoxLayout* layout_table1 = new QVBoxLayout();
	layout_table1->addWidget(new QLabel("Carti disponibile: "));
	layout_table1->addWidget(list_books);
	layout_table1->addSpacing(22);
	layout_main->addLayout(layout_table1);


	QVBoxLayout* layout_table2 = new QVBoxLayout();
	layout_table2->addWidget(new QLabel("Cos:"));
	layout_table2->addWidget(list_cart);
	layout_table2->addWidget(label_count);
	layout_main->addLayout(layout_table2);

	main->setLayout(layout_main);
	setCentralWidget(main);

	updateListBooks();
	updateListCart();
}

void CartWindow::connectSignalSlots()
{
	QObject::connect(button_add, &QPushButton::clicked, [&]() {
		if (list_books->currentRow() == -1)
		{
			QMessageBox::warning(this, "Eroare", "Selecteaza mai intai cartea din lista care sa fie adaugata in cos!");
		}
		else
		{
			service.getBorrowCart().addToBorrowCart(service.getServiceRepo().getElementList(list_books->currentRow()));
			updateListCart();
		}
		});

	QObject::connect(button_delete, &QPushButton::clicked, [&]() {
		service.getBorrowCart().deleteBorrowCart();
		updateListCart(); });

	QObject::connect(button_generate, &QPushButton::clicked, [&]() {
		int nr = QInputDialog::getText(this, "Dialog", "Introdu numarul de carti de generat!").toInt();
		service.getBorrowCart().generateBorrowCart(service.getServiceRepo().getRepoList(), nr);
		updateListCart();
		});

	QObject::connect(saveToCsv, &QAction::triggered, [&]() {
		QString a = QFileDialog::getSaveFileName(this, "Save", "", "CSV files (*.csv)");
		if (a == "") return;

		service.getBorrowCart().exportToCsv(a.toStdString());
		QMessageBox::information(this, "Info", "Carti salvate cu succes!");
		});

	QObject::connect(saveToHtml, &QAction::triggered, [&]() {
		QString a = QFileDialog::getSaveFileName(this, "Save", "", "HTML files (*.html)");
		if (a == "") return;

		if (QMessageBox::information(this, "Info", "Carti salvate cu succes! Doresti vizualizarea site-ului?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
			service.getBorrowCart().exportToHtml(a.toStdString(), 1);
		else
			service.getBorrowCart().exportToHtml(a.toStdString(), 0);
		});

}

void CartWindow::updateListBooks()
{
	list_books->clear();
	int k = 0;
	for (const Book& b : service.getServiceRepo().getRepoList())
	{
		QListWidgetItem* q = new QListWidgetItem(QString(
			QString::fromStdString(b.getTitle()) + "\t\t" +
			QString::fromStdString(b.getAuthor()) + "\t\t" +
			QString::fromStdString(b.getGenre()) + "\t\t" +
			QString::number(b.getYear())));
		q->setFont(QFont(this->font().toString(), -5, 100, false));

		list_books->insertItem(k++, q);
	}
	label_count->setText("Carti in cos: " + QString::number(service.getBorrowCart().getSize()));

}

void CartWindow::updateListCart()
{
	list_cart->clear();
	int k = 0;
	for (const Book& b : service.getBorrowCart().getBorrowCartList())
	{
		QListWidgetItem* q = new QListWidgetItem(QString(
			QString::fromStdString(b.getTitle()) + "\t\t" +
			QString::fromStdString(b.getAuthor()) + "\t\t" +
			QString::fromStdString(b.getGenre()) + "\t\t" +
			QString::number(b.getYear())));
		q->setFont(QFont(this->font().toString(), -5, 100, false));

		list_cart->insertItem(k++, q);
	}
	label_count->setText("Carti in cos: " + QString::number(service.getBorrowCart().getSize()));

}

CartWindow::CartWindow(Service& service):service{service}
{
	initCartWindow();
	connectSignalSlots();
}
