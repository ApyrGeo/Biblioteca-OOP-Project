#pragma once
#include <string>
using std::string;


class Exception
{
private:
	string message;

public:
	Exception(const string& message): message{message}
	{
		//this->message = message;
		//setMessage(message);
	}
	void setMessage(const string& msg)
	{
		this->message = msg;
	}
	string what()
	{
		return message;
	}
};

class RepositoryException : public Exception
{
	
};
class ServiceException : public Exception
{
};
class UIException : public Exception
{
};   


