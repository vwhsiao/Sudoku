#include "Square.h"
#include <string>

#include<iostream>
Square::Square(int row, int col, int value, int boxH, int boxW)
{
	Square::row = row;
	Square::col = col;
	Square::value = value;
	Square::boxW = boxW;
	Square::boxH = boxH;

	int boxRow = (row) / boxH;
	int boxCol = (col) / boxW;

	Square::boxNum = (int)((boxRow*boxH) + ((float)col / boxW));
	initDomain(boxH * boxW);
}

Square::Square(int boxH, int boxW, int boxNum)
{
	Square::boxH = boxH;
	Square::boxW = boxW;
	Square::boxNum = boxNum;

	// Idk how to math
	Square::col = 0;
	Square::row = 0;

	initDomain(boxH * boxW);
}

Square::~Square()
{
}

void Square::initDomain(int size)
{
	Square::domain.clear();
	for (int i = 0; i < size; i++)
	{
		Square::domain.push_back(i + 1);
	}
}

void Square::removeFromDomain(int _value)
{
	for (int i = 0; i < domain.size(); i++)
	{
		if (domain[i] == _value)
		{
			Square::domain.erase(Square::domain.begin() + i);
			return;
		}
	}
}

void Square::addToDomain(int _value)
{
	for (int i = 0; i< Square::domain.size(); i++)
	{
		if (Square::domain[i] == _value)
		{
			return;
		}
	}
	Square::domain.push_back(_value);
}

void Square::print()
{
	std::cout << "zero based indexing used for following:" << std::endl;
	std::cout << "row: " << row << " col: " << col << std::endl;
	std::cout << "value: " << value<<std::endl;
	std::cout << "box number: " << boxNum<<std::endl;
}

void Square::resetValue()
{
	//addToDomain(value);
	printDomain();
	value = 0;
}

void Square::setValue(int newValue)
{
	removeFromDomain(newValue);
	value = newValue;
}

int Square::getValue()
{
	return value;
}

std::vector<int> Square::getDomain()
{
	return Square::domain;
}

void Square::printDomain()
{
	std::cout << "domain for square (row, col) " << row << " " << col << ": ";
	for (int i = 0; i < domain.size(); i++)
	{
		std::cout << domain[i] << " ";
	}
	std::cout << std::endl;
}

std::string Square::getDomainString()
{
	std::string text = "square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")'s domain: ";
	for (int i = 0; i < domain.size(); i++)
	{
		text += std::to_string(domain[i]) + " ";
	}
	return text + "\n";
}