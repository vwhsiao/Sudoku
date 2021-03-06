#include <string>
#include<iostream>

#include "Square.h"


#pragma region CONSTRUCTORS AND DESTRUCTOR

// Constructor with full information for a square
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
	neighborReferences = std::vector<Square*>();
}

// Square constructor used for neighbor information
Square::Square(int row, int col, int boxNum, int value, std::vector<int> domain, std::vector<int> storedDomain)
{
	Square::row = row;
	Square::col = col;
	Square::boxNum = boxNum;
	setValue(value);
	Square::domain = domain;
	Square::storedDomain = storedDomain;
}

Square::~Square()
{
}

#pragma endregion


#pragma region VALUE

// Add the value back to domain and set value to zero
void Square::resetValue()
{
	addToDomain(value);
	value = 0;
}

// Setter for the value
void Square::setValue(int newValue)
{
	removeFromDomain(newValue);
	value = newValue;
}

// Getter for the value
int Square::getValue()
{
	return value;
}

#pragma endregion


#pragma region DOMAINS

// Initialize the domain from size
void Square::initDomain(int size)
{
	Square::domain.clear();
	for (int i = 0; i < size; i++)
	{
		Square::domain.push_back(i + 1);
	}
}

// Remove desired value from domain
void Square::removeFromDomain(int _value)
{
	if (domainLocked || given)
		return;

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

// Add value to domain (only if it exists in storedDomain)
void Square::addToDomain(int _value)
{
	if (domainLocked || given)
		return;

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

// Getter for the domain
std::vector<int> Square::getDomain()
{
	return Square::domain;
}

// Restore domain to storedDomain
void Square::restoreDomain()
{
	if (domainLocked || given)
		return;

	domain = storedDomain;
}

// Store domain into storedDomain
void Square::storeDomain()
{
	if (domainLocked || given)
		return;

	storedDomain = domain;
}

// Getter for storedDomain
std::vector<int> Square::getStoredDomain()
{
	return storedDomain;
}

// Given two domains, replace domain and storedDomain respectively
void Square::restoreDomains(std::vector<int> domain, std::vector<int> storedDomain)
{
	if (domainLocked || given)
		return;

	Square::domain = domain;
	Square::storedDomain = storedDomain;
}

// "Timetravel" to before value was assigned and preserve the domain and storedDomain at that state
void Square::preserveDomains(int _value)
{
	std::vector<int> oldDomain = domain;
	std::vector<int> oldStoredDomain = storedDomain;
	if (domainsForPrevHost.size() > 0)
	{
		storedDomain = domainsForPrevHost[domainsForPrevHost.size() - 1];
		domainsForPrevHost.clear();
	}
	addToDomain(_value);
	domainsForPrevHost.push_back(domain);
	domainsForPrevHost.push_back(storedDomain);
	domain = oldDomain;
	storedDomain = oldStoredDomain;
}

#pragma endregion


#pragma region STRINGS AND PRINTS

// Print out information of the square into the console
void Square::print()
{
	std::cout << "zero based indexing used for following:" << std::endl;
	std::cout << "row: " << row << " col: " << col << std::endl;
	std::cout << "value: " << value << std::endl;
	std::cout << "box number: " << boxNum << std::endl;
}

// Print out domain information into console
void Square::printDomain()
{
	std::cout << "domain for square (row, col) " << row << " " << col << ": ";
	for (int i = 0; i < domain.size(); i++)
	{
		std::cout << domain[i] << " ";
	}
	std::cout << std::endl;
}

// Return a string of domain information
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

// Return a string of neighbor domain information
std::string Square::getNeighborInfosString()
{
	std::string text = "Neighbor Domains:\n";
	if (neighborInfos.size() == 0)
		return text + "EMPTY";

	for (int i = 0; i < neighborInfos.size(); i++)
	{
		int row = neighborInfos[i].row;
		int col = neighborInfos[i].col;
		std::vector<int> domain = neighborInfos[i].getDomain();
		std::vector<int> storedDomain = neighborInfos[i].storedDomain;

		text += "square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")'s domain: ";
		if (given)
			return text + "GIVEN\n";
		if (domain.size() == 0)
			text += "EMPTY";
		for (int i = 0; i < domain.size(); i++)
		{
			text += std::to_string(domain[i]) + " ";
		}

		text += "\n--- storedDomain: ";
		for (int i = 0; i < storedDomain.size(); i++)
		{
			text += std::to_string(storedDomain[i]) + " ";
		}
		if (storedDomain.size() == 0)
			text += "EMPTY";
		text += "\n";
	}
	return text + "\n";
}

// Return a string of the current Square's row and column
std::string Square::getHostString()
{
	return "Host Info: square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")";
}

// Return a string of the preserved domains
std::string Square::getPreservedDomainsString()
{
	std::string text = "square (row: " + std::to_string(row) + ", col: " + std::to_string(col) + ")'s PRESERVED domains: ";
	if (given)
		return text + "GIVEN\n";
	if (domainsForPrevHost.size() == 0)
		return text + "NO DOMAINS PRESERVED";
	std::vector<int> domain = domainsForPrevHost[0];
	std::vector<int> storedDomain = domainsForPrevHost[1];
	if (domain.size() == 0)
		text += "EMPTY";
	for (int i = 0; i < domain.size(); i++)
	{
		text += std::to_string(domain[i]) + " ";
	}

	text += "\n--- storedDomain: ";
	for (int i = 0; i < storedDomain.size(); i++)
	{
		text += std::to_string(storedDomain[i]) + " ";
	}
	if (storedDomain.size() == 0)
		text += "EMPTY";

	return text + "\n";
}

#pragma endregion
