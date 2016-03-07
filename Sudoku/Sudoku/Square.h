#ifndef SQUARE
#define SQUARE

#include <vector>
#include <string>
class Square
{
public:
	Square(int row, int col, int value, int boxH, int boxW);
	Square(int boxH, int boxW, int boxNum);
	~Square();
	void print();

	void initDomain(int size);
	void removeFromDomain(int _value=-1);
	void addToDomain(int _value);
	void resetValue();
	void setValue(int newValue);
	int getValue();
	
	std::string getDomainString(bool showLastResult=false);
	void printDomain();
	std::vector<int> getDomain();

	std::vector<int> initialDomain;
	void restoreDomain();
	void storeDomain();

	int row, col, boxH, boxW;
	int boxNum;
private:
	std::vector<int> domain;
	std::vector<int> storedDomain;
	int value;

public:
	bool given = false;
	std::string lastAddingAttempt = "";
	std::vector<Square> neighborInfos = std::vector<Square>();
	Square(int row, int col, int boxNum, int value, std::vector<int> domain, std::vector<int> storedDomain);
	std::string getNeighborInfosString();
	std::string getHostString();
	void restoreDomains(std::vector<int> domain, std::vector<int> storedDomain);
	bool domainLocked = false;
	std::vector<int> getStoredDomain();
	std::vector<Square*> neighborReferences;
};

#endif