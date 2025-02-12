#include <string>

using std::string;

static bool validateString(string str) noexcept
{
	if (str.size() < 1) return false;
	for (const char c : str)
		if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == ' ' || c == '-' || c == '.' || c == '/' || c == '+' || c == '&'))
			return false;
	return true;
}
bool validateAuthor(string author) noexcept
{
	return validateString(author);
}
bool validateGenre(string genre) noexcept
{
	return validateString(genre);
}
bool validateTitle(string title) noexcept
{
	return validateString(title);
}
int validateYear(int year) noexcept
{
	return year > 0;
}
