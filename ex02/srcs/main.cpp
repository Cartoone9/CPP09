#include <algorithm>
#include <ctime>
#include <deque>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "colors.hpp"
#include "dictionary.hpp"

#define OK 0
#define NOK 1
#define ERROR -1

// --- helper functions declaration ---
template <typename T>
static void	containerFordJohnson(T& container, void (*sorting_algo)(T&), bool which);
template <typename T>
static void	isSorted(const T& container);
template <typename T>
static bool	isSortedCheck(const T& container);
template <typename T>
static void	printContainer(T& container);

static void	invalidUsage();
static void	invalidArg();
static int	parseArgs(int ac, char** av,
					 std::vector<int>& base_vec, std::deque<int>& base_deq);
static bool	checkDigits(char* str);
static void	vectorFordJohnson(std::vector<int>& temp_vec);
static void	dequeFordJohnson(std::deque<int>& base_deq);
static int	findJacobsthal(int n);

// --- main functions ---
int main(int ac, char** av)
{
	if (ac < 2)
	{
		invalidUsage();
		return (NOK);
	}

	std::vector<int> base_vec;
	std::deque<int> base_deq;

	if (parseArgs(ac, av, base_vec, base_deq) == ERROR)
		return (NOK);

	containerFordJohnson(base_vec, vectorFordJohnson, true);

	containerFordJohnson(base_deq, dequeFordJohnson, false);

	return (OK);
}

template <typename T>
static void	containerFordJohnson(T& container, void (*sorting_algo)(T&), bool which)
{
	std::cout << REVERSED << (which ? TEAL : MAGENTA) << "--- " 
		<< (which ? "VECTOR" : "DEQUE") << " ---\n" RESET << std::endl;

	std::cout << "Elements before: ";
	printContainer(container);

	std::clock_t start_time = std::clock();

	sorting_algo(container);

	std::clock_t end_time = std::clock();

	std::cout << "Elements after: ";
	printContainer(container);

	double	duration_ticks = static_cast<double>(end_time - start_time);
	double	duration_seconds = duration_ticks / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " ORANGE
		<< container.size() << RESET " elements with std::"
		<< (which ? "vector: " : "deque: ")
		<< std::fixed << std::setprecision(6)
		<< UNDERLINE << duration_seconds << " seconds." RESET
		<< (which ? "\n" : "") << std::endl;
}





// --- helper functions definition ---
static void	invalidUsage()
{
	std::cerr << RED "Error: " RESET << "invalid use: ./PmergeMe <values>" << std::endl;
}

static void	invalidArg()
{
	std::cerr << RED "Error: " RESET << "invalid arg in given values" << std::endl;
}

static int	parseArgs(int ac, char** av,
					 std::vector<int>& base_vec, std::deque<int>& base_deq)
{
	for (int i = 1; i < ac; i++)
	{
		std::istringstream	iss(av[i]);
		int					temp_int;

		if (!checkDigits(av[i]))
		{
			invalidArg();
			return (ERROR);
		}

		while (iss >> temp_int)
		{
			base_vec.push_back(temp_int);
			base_deq.push_back(temp_int);
		}
	}
	return (OK);
}

static bool	checkDigits(char* str)
{
	unsigned char	temp_c;

	for (int i = 0; str[i]; i++)
	{
		temp_c = static_cast<unsigned char>(str[i]);
		if (!std::isdigit(temp_c) && !std::isspace(temp_c))
			return (false);
	}
	return (true);
}

static void	vectorFordJohnson(std::vector<int>& base_vec)
{
	size_t	base_vec_len = base_vec.size();

	if (base_vec_len <= 1)
		return;

	bool	is_odd = false;
	int		straggler;

	// odd number of elements -> straggler
	if (base_vec_len % 2 != 0)
	{
		is_odd = true;
		straggler = base_vec[base_vec_len - 1];
		base_vec.pop_back();
		base_vec_len--;
	}

	std::vector<std::pair<int, int> >	orig_pairs;
	std::vector<int>					winners;

	// form the orig_pairs, preorder them so that
	// first is always the biggest of the pair
	// and keep a vector of winners aside for recursion
	for (size_t i = 0; i < base_vec_len; i += 2)
	{
		int	first = base_vec[i];
		int	second = base_vec[i + 1];

		if (first > second)
		{
			orig_pairs.push_back(std::make_pair(first, second));
			winners.push_back(first);
		}
		else
	{
			orig_pairs.push_back(std::make_pair(second, first));
			winners.push_back(second);
		}
	}

	vectorFordJohnson(winners);

	std::vector<int>					main_chain = winners;
	std::vector<std::pair<int, int> >	sorted_pairs;

	for (size_t i = 0; i < main_chain.size(); ++i)
	{
		int current_winner = main_chain[i];
		// Find the loser corresponding to this winner.
		for (size_t j = 0; j < orig_pairs.size(); ++j)
		{
			if (orig_pairs[j].first == current_winner)
			{
				sorted_pairs.push_back(orig_pairs[j]);
				// remove the found pair
				orig_pairs.erase(orig_pairs.begin() + j);
				break;
			}
		}
	}

	std::vector<std::pair<int, size_t> >	pend_chain; // { loser, winner_index }

	for (size_t i = 0; i < main_chain.size(); i++)
	{
		pend_chain.push_back(std::make_pair(sorted_pairs[i].second, i + 1)); // shift index by 1
		// to account for the first
		// pend element insertion
	}

	// insert pend into main here
	// first start with the smallest element of the losers
	// which goes at the beginning
	main_chain.insert(main_chain.begin(), pend_chain[0].first);

	int	index_jacob = 3;
	int	curr_jacob = findJacobsthal(index_jacob++);
	int	prev_jacob = 1;

	while (static_cast<size_t>(prev_jacob) < pend_chain.size())
	{
		if (static_cast<size_t>(curr_jacob) > pend_chain.size())
			curr_jacob = pend_chain.size();

		for (int i = curr_jacob; i > prev_jacob; i--)
		{
			int		value = pend_chain[i - 1].first;
			size_t	limit_index = pend_chain[i - 1].second;

			std::vector<int>::iterator	limit_it = main_chain.begin() + limit_index;
			std::vector<int>::iterator	pos = std::lower_bound(main_chain.begin(), limit_it, value);

			size_t	insertion_index = std::distance(main_chain.begin(), pos);

			main_chain.insert(pos, value);

			std::vector<std::pair<int, size_t> >::iterator it;
			std::vector<std::pair<int, size_t> >::iterator it_end = pend_chain.end();
			for (it = pend_chain.begin(); it != it_end; it++)
			{
				if (it->second >= insertion_index)
					it->second++;
			}
		}
		prev_jacob = curr_jacob;
		curr_jacob = findJacobsthal(index_jacob++);
	}

	// if is_odd insert straggler here
	if (is_odd)
	{
		std::vector<int>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
		main_chain.insert(pos, straggler);
	}

	base_vec = main_chain;
}

static void	dequeFordJohnson(std::deque<int>& base_deq)
{
	size_t	base_deq_len = base_deq.size();

	if (base_deq_len <= 1)
		return;

	bool	is_odd = false;
	int		straggler;

	// odd number of elements -> straggler
	if (base_deq_len % 2 != 0)
	{
		is_odd = true;
		straggler = base_deq[base_deq_len - 1];
		base_deq.pop_back();
		base_deq_len--;
	}

	std::deque<std::pair<int, int> >	orig_pairs;
	std::deque<int>						winners;

	// form the orig_pairs, preorder them so that
	// first is always the biggest of the pair
	// and keep a deque of winners aside for recursion
	for (size_t i = 0; i < base_deq_len; i += 2)
	{
		int	first = base_deq[i];
		int	second = base_deq[i + 1];

		if (first > second)
		{
			orig_pairs.push_back(std::make_pair(first, second));
			winners.push_back(first);
		}
		else
	{
			orig_pairs.push_back(std::make_pair(second, first));
			winners.push_back(second);
		}
	}

	dequeFordJohnson(winners);

	std::deque<int>					main_chain = winners;
	std::deque<std::pair<int, int> >	sorted_pairs;

	for (size_t i = 0; i < main_chain.size(); ++i)
	{
		int current_winner = main_chain[i];
		// Find the loser corresponding to this winner.
		for (size_t j = 0; j < orig_pairs.size(); ++j)
		{
			if (orig_pairs[j].first == current_winner)
			{
				sorted_pairs.push_back(orig_pairs[j]);
				// remove the found pair
				orig_pairs.erase(orig_pairs.begin() + j);
				break;
			}
		}
	}

	std::deque<std::pair<int, size_t> >	pend_chain; // { loser, winner_index }

	for (size_t i = 0; i < main_chain.size(); i++)
	{
		pend_chain.push_back(std::make_pair(sorted_pairs[i].second, i + 1)); // shift index by 1
		// to account for the first
		// pend element insertion
	}

	// insert pend into main here
	// first start with the smallest element of the losers
	// which goes at the beginning
	main_chain.insert(main_chain.begin(), pend_chain[0].first);

	int	index_jacob = 3;
	int	curr_jacob = findJacobsthal(index_jacob++);
	int	prev_jacob = 1;

	while (static_cast<size_t>(prev_jacob) < pend_chain.size())
	{
		if (static_cast<size_t>(curr_jacob) > pend_chain.size())
			curr_jacob = pend_chain.size();

		for (int i = curr_jacob; i > prev_jacob; i--)
		{
			int		value = pend_chain[i - 1].first;
			size_t	limit_index = pend_chain[i - 1].second;

			std::deque<int>::iterator	limit_it = main_chain.begin() + limit_index;
			std::deque<int>::iterator	pos = std::lower_bound(main_chain.begin(), limit_it, value);

			size_t	insertion_index = std::distance(main_chain.begin(), pos);

			main_chain.insert(pos, value);

			std::deque<std::pair<int, size_t> >::iterator it;
			std::deque<std::pair<int, size_t> >::iterator it_end = pend_chain.end();
			for (it = pend_chain.begin(); it != it_end; it++)
			{
				if (it->second >= insertion_index)
					it->second++;
			}
		}
		prev_jacob = curr_jacob;
		curr_jacob = findJacobsthal(index_jacob++);
	}

	// if is_odd insert straggler here
	if (is_odd)
	{
		std::deque<int>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
		main_chain.insert(pos, straggler);
	}

	base_deq = main_chain;
}

static int	findJacobsthal(int n)
{
	if (n <= 1)
		return n;

	int prev = 0; // J(0)
	int curr = 1; // J(1)

	for (int i = 2; i <= n; i++)
	{
		int next = curr + 2 * prev;
		prev = curr;
		curr = next;
	}

	return (curr);
}



template <typename T>
static void	isSorted(const T& container)
{
	std::cout << "Sorted? " 
		<< (isSortedCheck(container) ? GREEN "[OK]" RESET : RED "[NO]" RESET)
		<< "\n" << std::endl;
}

template <typename T>
static bool	isSortedCheck(const T& container)
{

	typename T::const_iterator it = container.begin();
	typename T::const_iterator it_end = container.end();

	if (it == it_end || it + 1 == it_end)
		return (true);

	while (it + 1 != it_end)
	{
		if (!(*it <= *(it + 1)))
			return (false);

		it++;
	}
	return (true);
}

template <typename T>
static void	printContainer(T& container)
{
	size_t	max_elem = 10;
	size_t	max = (container.size() <= max_elem ? container.size() : max_elem);

	for (size_t i = 0; i < max; i++)
	{
		std::cout << container[i];
		if (i + 1 < max)
			std::cout << " ";
		else
		{
			if (container.size() > max_elem)
				std::cout << " [...]";
			std::cout << std::endl;
		}
	}

	isSorted(container);
}
