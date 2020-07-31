#include "pch.h"


//long multithreadedSum = 0;
std::atomic<long> multithreadedSum(0);

void SumNumbers(const std::vector<int>& toBeSummed, int idxStart, int idxEnd)
{
	for (int i = idxStart; i <= idxEnd; ++i)
	{
		multithreadedSum += toBeSummed[i];
	}
}


int main()
{
	std::vector<int> toBeSummed;
	for (int i = 0; i < 30000000; ++i)
	{
		toBeSummed.emplace_back(std::rand());
	}

	long sum = 0;
	for (const int num : toBeSummed)
	{
		sum += num;
	}
	printf("The sum without multithreading is %d\n", sum);

	//-----------------MULTITHREADING-----------------

	std::thread t1(SumNumbers, toBeSummed, 0, 9999999);
	std::thread t2(SumNumbers, toBeSummed, 10000000, 19999999);
	std::thread t3(SumNumbers, toBeSummed, 20000000, 29999999);

	t1.join();
	t2.join();
	t3.join();

	printf("The sum with multithreading is    %d\n", multithreadedSum.load());
}


