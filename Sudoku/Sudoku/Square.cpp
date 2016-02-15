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
	if (_value == 0)
		return;

	if (_value == -1)
		_value = value;

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
	if (_value == 0)
	{
		lastAddingAttempt = "-Tried to add in 0 but FAILED.";
		return;
	}

	bool found = false;
	for (int i = 0; i < storedDomain.size(); i++)
	{
		if (storedDomain[i] == _value)
		{
			lastAddingAttempt = "-Value " + std::to_string(_value) + " is ADDED back to domain.";
			found = true;
			break;
		}
	}

	if (!found)
	{
		lastAddingAttempt = "-Value " + std::to_string(_value) + " is NOT FOUND in storedDomain.";
		return;
	}

	for (int i = 0; i< Square::domain.size(); i++)
	{
		if (Square::domain[i] == _value)
		{
			lastAddingAttempt = "-Value " + std::to_string(_value) + " is ALREADY IN the domain.";
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
	addToDomain(value);
	//printDomain();
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

void Square::restoreDomain(std::vector<int> newDomain)
{
	storedDomain = newDomain;
	domain = newDomain;
}

void Square::storeDomain()
{
	storedDomain = domain;
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

std::string Square::getDomainString(bool showLastResult)
{
	std::string text = "square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")'s domain: ";
	if (given)
		return text + "GIVEN\n";
	if (domain.size() == 0)
		text += "EMPTY";
	for (int i = 0; i < domain.size(); i++)
	{
		text += std::to_string(domain[i]) + " ";
	}
	if (showLastResult && lastAddingAttempt != "")
		text += "  " + lastAddingAttempt;
	lastAddingAttempt = "";

	text += "\n--- storedDomain: ";
	for (int i = 0; i < storedDomain.size(); i++)
	{
		text += std::to_string(storedDomain[i]) + " ";
	}
	if (storedDomain.size() == 0)
		text += "EMPTY";

	return text + "\n";
}

Square::Square(int row, int col, int boxNum, int value, std::vector<int> domain)
{
	Square::row = row;
	Square::col = col;
	Square::boxNum = boxNum;
	setValue(value);
	Square::domain = domain;
}

std::string Square::getNeighborDomainsString()
{
	std::string text = "\nNeighbor Domains:\n";
	for (int i = 0; i < neighborDomains.size(); i++)
	{
		int row = neighborDomains[i].row;
		int col = neighborDomains[i].col;
		std::vector<int> domain = neighborDomains[i].domain;

		text += "square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")'s domain: ";
		if (given)
			return text + "GIVEN\n";
		if (domain.size() == 0)
			text += "EMPTY";
		for (int i = 0; i < domain.size(); i++)
		{
			text += std::to_string(domain[i]) + " ";
		}
		text += "\n";
	}
	return text + "\n";
}