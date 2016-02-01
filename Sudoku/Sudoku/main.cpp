#include <iostream>
#include "Sudoku.h"
#include "FileManager.h"
#include <string>
#include <time.h>
std::vector<int> generateFromFile(std::string filename)
{
	const std::string inputFilesPath = "..\\_InputFiles\\";
	FileManager file = FileManager();
	return file.readFile(inputFilesPath + filename);
}

void altMain()
{
	bool problem = true;
	bool initTest = false;
	bool rngSize4 = false;
	bool rngSize6 = false;
	bool rngSize9 = false;
	bool rngSize12 = false;

	if (problem)
	{
		Sudoku p = Sudoku(6);
		p.buildByRng();
		p.generateProblem(5);
		p.print();
	}

	if (initTest)
	{
		Sudoku test = Sudoku(generateFromFile("test.txt"));
		test.print();
		test.printByColumns();
		test.printByBoxes();
	}

	if (rngSize4)
	{
		Sudoku s4 = Sudoku(4);
		s4.buildByRng();
		s4.print();
	}
	
	if (rngSize6)
	{
		Sudoku s6 = Sudoku(6);
		s6.buildByRng();
		s6.print();
	}

	if (rngSize9)
	{
		Sudoku s9 = Sudoku(9);
		s9.buildByRng();
		s9.print();
	}

	if (rngSize12)
	{
		Sudoku s12 = Sudoku(12);
		s12.buildByRng();
		s12.print();
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	if (argv[1] == nullptr)
	{
		altMain();
		return 0;
	}

	//yes, this is hard coded cause supposedly the arguments are always in the same order. 
	FileManager file = FileManager();
	std::vector<std::string> options = std::vector<std::string>();
	if (argc > 4)
	{
		for (int i = 4; i < argc; i++)
		{
			options.push_back(argv[i]);
			std::cout << argv[i] << std::endl;
		}
		options.push_back(" ");
		
		
		
		Sudoku* s = new Sudoku(file.readFile(argv[1]), std::stof(argv[3]), options);
		file.writeTo(argv[2], s->returnOutput());
		s->print();
		delete s;
	}
	
	else
	{
		options.push_back(" ");
		
		Sudoku* s = new Sudoku(file.readFile(argv[1]), std::stof(argv[3]), options);
		file.writeTo(argv[2], s->returnOutput());
		s->print();
		delete s;
	}
	
	/*clock_t t1, t2;
	t1 = clock();
	for (int i = 1; i < 51; i++)
	{
		std::string unsolved =  "PE" + std::to_string(i)+".txt";

		std::cout << unsolved << std::endl;
		Sudoku s = Sudoku(generateFromFile(unsolved));
		std::cout << "before: " << std::endl;
		s.print();
		s.solveStart();
		std::cout << "after: " << std::endl;
		s.print();
		std::cout << std::endl;
	}
	t2 = clock();
	float diff((float)t2 - (float)t1);
	std::cout << "time spent: " << diff/CLOCKS_PER_SEC << std::endl;

*/

	std::cin.get();
	return 0;
}
