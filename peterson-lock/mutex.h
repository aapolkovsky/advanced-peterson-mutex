#ifndef HOMEBREW_MUTEX_HPP
#define HOMEBREW_MUTEX_HPP

#include <atomic>
#include <vector>
#include <thread>
#include <map>
#include <algorithm>

namespace homebrew
{
	class advanced_peterson_mutex;

	using mutex = advanced_peterson_mutex;

	class advanced_peterson_mutex
	{
	public:
		enum { MAX_THREADS_NUMBER = 128 };

		advanced_peterson_mutex() : threads_(MAX_THREADS_NUMBER), ticket_(MAX_THREADS_NUMBER)
		{
		}

		void lock()
		{
			auto key = std::this_thread::get_id();
			int thread_id;
			if (map_.find(key) == map_.end())
			{
				map_[key] = ++thread_count_;
				thread_id = map_[key];
				threads_[thread_id] = false;
				ticket_[thread_id] = 0;
			}
			else
			{
				thread_id = map_[key];
			}
			get_ticket(thread_id);
			wait(thread_id);
		}

		void unlock()
		{
			auto key = std::this_thread::get_id();
			int thread_id = map_[key];
			threads_[thread_id] = false;
		}

	private:
		void get_ticket(int thread_id)
		{
			threads_[thread_id] = true;
			int max_ticket = 0;
			for (int i = 0; i < ticket_.size(); i++)
			{
				max_ticket = std::max(max_ticket, ticket_[i].load());
			}
			ticket_[thread_id] = max_ticket + 1;
		}

		void wait(int thread_id)
		{
			for (int i = 0; i < ticket_.size(); ++i)
			{
				while (threads_[i] && std::make_pair(ticket_[i].load(), i) < std::make_pair(ticket_[thread_id].load(), thread_id));
			}
		}

	private:
		std::map<std::thread::id, int> map_;
		std::vector<std::atomic_bool> threads_;
		std::vector<std::atomic_int> ticket_;
		std::atomic<int> thread_count_;
	};
}

#endif
