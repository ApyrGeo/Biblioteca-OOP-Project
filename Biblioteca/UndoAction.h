#pragma once
#include "Domain.h"
#include "Repository.h"

class UndoAction
{
public:
	virtual void doUndo() = 0;
	virtual ~UndoAction() {};
};

class UndoAdd:public UndoAction
{
private:
	size_t id_book;
	AbstractRepository& rep;
public:
	UndoAdd(AbstractRepository& rep, size_t last_id) noexcept : rep{ rep }, id_book{ last_id }{} 
	void doUndo() noexcept override
	{
		rep.deleteFromList(id_book);
	}
};

class UndoModify :public UndoAction
{
private:
	size_t index;
	Book previous;
	AbstractRepository& rep;
public:
	UndoModify(AbstractRepository& rep, size_t index, const Book& previous) : rep{ rep }, index{ index }, previous { previous } {}
	void doUndo() override
	{
		rep.modifyInList(index, previous);
	}
};

class UndoDelete :public UndoAction
{
private:
	size_t position;
	Book to_add;
	AbstractRepository& rep;
public:
	UndoDelete(AbstractRepository& rep, size_t position, const Book& to_add) : rep{ rep }, to_add{ to_add }, position{position} {}
	void doUndo() override
	{
		rep.addToListOnPosition(position, to_add);
	}
};

