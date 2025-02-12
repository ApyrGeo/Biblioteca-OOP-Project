#pragma once
#include <string>
using std::string;

/// <summary>
/// Validates an author name.
/// </summary>
/// <param name="author">- the given author name</param>
/// <returns>Bool representing the result of validation (1 - it is a good author name, 0 - it is not)</returns>
bool validateAuthor(string author) noexcept;
/// <summary>
/// Validates an genre.
/// </summary>
/// <param name="genre">- the given genre</param>
/// <returns>Bool representing the result of validation (1 - it is a good genre, 0 - it is not)</returns>
bool validateGenre(string genre) noexcept;
/// <summary>
/// Validates an title.
/// </summary>
/// <param name="title">- the given title</param>
/// <returns>Bool representing the result of validation (1 - it is a good title, 0 - it is not)</returns>
bool validateTitle(string title) noexcept;
/// <summary>
/// Validates a year.
/// </summary>
/// <param name="year">- the given year</param>
/// <returns>Bool representing the result of validation (1 - it is a good year, 0 - it is not)</returns>
int validateYear(int year) noexcept;
