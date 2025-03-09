#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <clocale>
#include <cctype>

class Point
{

public:
	int x;
	int y;

	Point(int x, int y) { this->x = x; this->y = y; }

	void setX(int x) { this->x = x; };
	void setY(int y) { this->y = y; };
};

class Engine
{
	private:
		std::vector<std::vector<char>> table;

	public:
		std::vector<std::vector<char>> getTable() { return this->table; }


		std::string alterString(std::string str);


		std::vector<std::vector<char>> createTable(std::string keyword);

		std::string encipher(std::string text, char separator);

		std::vector<std::string> encodeDigraph(std::vector<std::string> digraphs);

		Point getPoint(char c);

		std::string encode(std::string text, std::string keyword, char separator);

		std::string decode(std::string text, std::string keyword, char separator);

};

