#ifndef SQUARE
#define SQUARE

#include <vector>
#include <string>


class Square
{
public:

	// Constructors and Destructor
	Square(int row, int col, int value, int boxH, int boxW);
	Square(int row, int col, int boxNum, int value, std::vector<int> domain, std::vector<int> storedDomain);
	~Square();

	// Domains
	void initDomain(int size);
	void removeFromDomain(int _value=-1);
	void addToDomain(int _value);
	std::vector<int> getDomain();
	void restoreDomain();
	void restoreDomains(std::vector<int> domain, std::vector<int> storedDomain);
	void storeDomain();
	std::vector<int> getStoredDomain();
	void preserveDomains(int _value);

	// Value
	void resetValue();
	void setValue(int newValue);
	int getValue();

	// Prints and Strings
	void print();
	void printDomain();
	std::string getDomainString(bool showLastResult = false);
	std::string getPreservedDomainsString();
	std::string getNeighborInfosString();
	std::string getHostString();
	std::string lastAddingAttempt = "";

	// Square Properties
	bool domainLocked = false;
	bool given = false;
	int value;
	int row, col, boxH, boxW;
	int boxNum;

	// Domain Information
	std::vector<int> initialDomain;
	std::vector<int> domain;
	std::vector<int> storedDomain;
	std::vector<std::vector<int>> domainsForPrevHost = std::vector<std::vector<int>>();
	
	// Neighbor Informaion
	std::vector<Square*> neighborReferences;
	std::vector<Square> neighborInfos = std::vector<Square>();	
};

#endif