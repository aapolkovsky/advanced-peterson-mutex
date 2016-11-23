#include <iostream>
#include <queue>

#include "mutex.h"

int main(int argc, char** argv)
{
	std::queue<int> qu;
	homebrew::mutex mx;

	std::thread thread1([&mx, &qu]() {
		for (int i = 0; i < 10; i++)
		{
			mx.lock();
			qu.push(1);
			mx.unlock();
		}
	});

	std::thread thread2([&mx, &qu]() {
		for (int i = 0; i < 10; i++)
		{
			mx.lock();
			qu.push(2);
			mx.unlock();
		}
	});

	std::thread thread3([&mx, &qu]() {
		for (int i = 0; i < 10; i++)
		{
			mx.lock();
			qu.push(3);
			mx.unlock();
		}
	});

	thread1.join();
	thread2.join();
	thread3.join();
	
	while (!qu.empty())
	{
		std::cout << "thread #" << qu.front() << " push" << std::endl;
		qu.pop();
	}

	return 0;
}
