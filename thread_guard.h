#pragma once

class thread_guard;

struct ptask
{
	double& i;

	ptask(double& i_) : i(i_) {}

	/*void operator()()
	{
		auto start = std::chrono::high_resolution_clock::now();
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::microseconds>(stop - start);

		while (percentage < 100)
		{
			if (duration.count() >= 1000000)
			{
				start = std::chrono::high_resolution_clock::now();
			}
			else
				continue;

			double maxPrint = 20;
			double toPrint = i / 5;
			system("CLS");
			std::cout << "[";
			for (int i = 0; i < toPrint; i++)
			{
				std::cout << '\|';
				maxPrint--;
			}
			for (int i = 0; i < maxPrint; i++)
			{
				std::cout << ' ';
			}
			std::cout << "]" << i << "%\n";
		}
		system("CLS");
		std::cout << "Listing...\n";
	}*/

};