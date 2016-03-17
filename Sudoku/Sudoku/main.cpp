#include <iostream>
#include "Sudoku.h"
#include "FileManager.h"
#include <string>
#include <time.h>

float totalTime=0;
float totalNodes = 0;
int succeeded = 0;

std::vector<float> listOfTimes = std::vector<float>();

std::vector<int> generateFromFile(std::string filename)
{
	const std::string inputFilesPath = "..\\_InputFiles\\";
	FileManager file = FileManager();
	std::vector<int> contents = file.readFile(inputFilesPath + filename);
	return contents;
}

void solveSudoku(std::string filename, int i, std::string output, int timelimit, std::vector<std::string> options)
{
	std::string fullFilename = filename + /*std::to_string(i) +*/ ".txt";
	std::string fullOutput = output + std::to_string(i) + ".txt";

	std::cout << "\n==========================" << std::endl;
	std::cout << "Solving " + fullFilename;

	FileManager file = FileManager();
	std::vector<int> reqs = file.readFile(fullFilename);
	if (reqs.size() == 0)
	{
		std::cout << "There is an error with this file." << std::endl;
		return;
	}

	Sudoku* s = new Sudoku(reqs, timelimit, options);
	file.writeTo(fullOutput, s->generateLog());

	std::cout << "result: " << std::endl;
	
	s->debugLog(s->getSudokuPrint("Solution"));

	totalNodes += s->countNodes;
	listOfTimes.push_back(s->timeToCount);
	totalTime += s->timeToCount;
	std::cout << s->solution << std::endl;
	if (s->solution)
	{
		succeeded++;
	}
	
	delete s;
}

void printStats(int num)
{
	std::cout << "average nodes: " << totalNodes / num << std::endl;
	std::cout << "average time: " << totalTime / num << std::endl;


	float averageTime = totalTime / num;
	std::vector<float> squaredTimes = std::vector<float>();
	for (int i = 0; i < listOfTimes.size(); i++)
	{
		float squareThis = listOfTimes[i] - averageTime;
		squareThis *= squareThis;
		squaredTimes.push_back(squareThis);
	}
	float meanSTDTime = 0;
	for (int i = 0; i < squaredTimes.size(); i++)
	{
		meanSTDTime += squaredTimes[i];
	}
	meanSTDTime /= squaredTimes.size();
	std::cout << "standard deviation: " << sqrt(meanSTDTime) << std::endl;;
	std::cout << "succeeded: " << succeeded << std::endl;

}

void lazyMain()
{
	const std::string inputFilesPath = "..\\_InputFiles\\";
	const std::string outputFilesPath = "..\\_OutputFiles\\";
	
	std::string filename;
	int timelimit = -1;
	std::vector<std::string> options = std::vector<std::string>();
	options.push_back("GEN");
	
	options.push_back("FC");
	options.push_back("MRV");
	//options.push_back("DH");
	
	std::string output = outputFilesPath + "output-";
	for (int i = 0; i < options.size(); i++)
	{
		output += options[i] + "-";
	}
	int totalNum = 0;
	filename = "GEN";
	for (int i = 1; i <= 10; i++)
	{

		solveSudoku(inputFilesPath + filename, i, output + filename, 300, options);
		totalNum = i;
	}


	filename = "PE";
	for (int i = 1; i <= 50; i++)
	{
		break;
		solveSudoku(inputFilesPath + filename, i, output + filename, timelimit, options);
		totalNum = i;
	}

	filename = "PM";
	for (int i = 1; i <= 5; i++)
	{
		break;
		solveSudoku(inputFilesPath + filename, i, output + filename, timelimit, options);
	}

	filename = "PH";
	for (int i = 1; i <= 5; i++)
	{
		break;
		solveSudoku(inputFilesPath + filename, i, output + filename, timelimit, options);
	}

	std::cout << "Calculations are complete!" << std::endl;
	printStats(totalNum);
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
		Sudoku* p = new Sudoku(6);
		p->buildByRng();
		p->generateProblem(5);
		p->print();
		delete p;
	}

	if (initTest)
	{
		Sudoku* test = new Sudoku(generateFromFile("test.txt"));
		test->print();
		test->printByColumns();
		test->printByBoxes();
		delete test;
	}

	if (rngSize4)
	{
		Sudoku* s = new Sudoku(4);
		s->buildByRng();
		s->print();
		delete s;
	}

	if (rngSize6)
	{
		Sudoku* s = new Sudoku(4);
		s->buildByRng();
		s->print();
		delete s;
	}

	if (rngSize9)
	{
		Sudoku* s = new Sudoku(4);
		s->buildByRng();
		s->print();
		delete s;
	}

	if (rngSize12)
	{
		Sudoku* s = new Sudoku(4);
		s->buildByRng();
		s->print();
		delete s;
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	if (argv[1] == nullptr)
	{
		lazyMain();
		//altMain();
		//getchar();
		return 0;
	}

	//yes, this is hard coded cause supposedly the arguments are always in the same order. 
	FileManager file = FileManager();
	std::vector<std::string> options = std::vector<std::string>();
	//std::cout << argc << std::endl;
	int totalNum = 0;
	
	if (argc > 4)
	{
		for (int i = 4; i < argc; i++)
		{
			options.push_back(argv[i]);
		}

		
		
			
		Sudoku* s = new Sudoku(file.readFile(argv[1]), std::stof(argv[3]), options);

		file.writeTo(argv[2], s->generateLog());

		s->debugLog(s->getSudokuPrint("Solution"));


		//std::cout <<"nodes: "<< s->countNodes << std::endl;
		//std::cout <<"time spent: "<< s->timeToCount << std::endl;
		//	

		//if (!s->isTimeUp())
		//{
		//	totalNodes += s->countNodes;
		//	listOfTimes.push_back(s->timeToCount);
		//	totalTime += s->timeToCount;
		//	succeeded++;
		//	totalNum++;
		//}

		delete s;
			

		
	}
	
	else
	{
		options.push_back(" ");
		
		Sudoku* s = new Sudoku(file.readFile(argv[1]), std::stof(argv[3]), options);
		/*file.writeTo(argv[2], s->returnSudoku());*/
		s->print();
		file.writeTo(argv[2], s->generateLog());
		delete s;
	}
	//printStats(totalNum);
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

	/*std::cin.get();*/
	return 0;
}
