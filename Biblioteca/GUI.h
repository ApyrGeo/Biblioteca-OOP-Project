#pragma once

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/QMainWindow>
#include <qpainter.h>
#include <QWindow>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qtablewidget.h>
#include "Service.h"
#include <random>
#include "Observer.h"


class TableModel : public QAbstractTableModel
{
	vector<Book> lst;
public:
	TableModel(const vector<Book>& lst) : lst{ lst } {}
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	void setData(const vector<Book> books);
};
class CartWindow : public QMainWindow
{
	Q_OBJECT

private:
	Service& service;
	QAction* saveToHtml;
	QAction* saveToCsv;


	//QFileDialog::getOpenFileName();
	QListWidget* list_books;
	//QListWidget* list_cart;
	//QTableWidget* list_cart;

	QPushButton* button_add;
	QPushButton* button_delete;
	QPushButton* button_generate;
	//QLabel* label_count;


	QPushButton* button_open_crud = new QPushButton("&Deschide CosCRUDGUI");
	QPushButton* button_open_readonly = new QPushButton("&Deschide CosReadOnly");


	void initCartWindow();
	void connectSignalSlots();
	void updateListBooks();
	//void updateListCart();
public:
	CartWindow(Service& service);
};

class AddWindow : public QMainWindow
{
	Q_OBJECT

private:
	Service& service;

	QLineEdit* lineedit_title;
	QLineEdit* lineedit_author;
	QLineEdit* lineedit_genre;
	QLineEdit* lineedit_year;
	QPushButton* button_submit;


	void initAddWindow();
	void connectSignalSlots();

signals:
	void carteAdaugata();

public:
	AddWindow(/*MainWindow& main,*/ Service& service);
};
class ModifyWindow : public QMainWindow
{
	Q_OBJECT
private:
	Service& service;
	size_t index;

	QLineEdit* lineedit_title;
	QLineEdit* lineedit_author;
	QLineEdit* lineedit_genre;
	QLineEdit* lineedit_year;
	QPushButton* button_submit;


	void initModifyWindow();
	void connectSignalSlots();

signals:
	void carteModificata();

public:
	ModifyWindow(Service& service, size_t index);
};
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Service& service;

	QAction* generateReport;

	QAction* undo;
	QPushButton* button_add;
	QPushButton* button_modify;
	QPushButton* button_delete;

	QComboBox* combobox_filter_sort;

	QTableView* tbl = new QTableView;
	TableModel* model;

	QPushButton* button_reinit;
	QPushButton* button_cart;
	//QListWidget* listview_books;

	AddWindow* add_window;

	void initMainWindow();
	void connectSignalSlots();
	//void updateListView(const std::vector<Book>& v);


public:
	MainWindow(Service& service);
};

class cosCRUDGUI : public QMainWindow, public Observer
{
	Service& service;


	QTableWidget* list_cart = new QTableWidget{1,4};
	QPushButton* button_empty = new QPushButton("&Stergere");
	QPushButton* button_gen = new QPushButton("&Generare");

	void update() override;
	void updateTable();
	void initWindow();
	void connectSignalSlots();
public:
	cosCRUDGUI(Service& service);
	~cosCRUDGUI();
};

class cosReadOnly : public QMainWindow, public Observer
{
	Service& service;
	void update() override;
	void paintEvent(QPaintEvent* ev) override;
public:
	cosReadOnly(Service& service);
	~cosReadOnly();
};
