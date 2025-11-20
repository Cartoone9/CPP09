#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "colors.hpp"
#include "dictionary.hpp"

#define OK 0
#define NOK 1
#define ERROR -1

// --- helper functions declaration ---
static void	invalidUsage();
static void	invalidArg();
static int	parseArgs(int ac, char** av, std::vector<int>& temp_vec);
static bool	checkDigits(char* str);
static void	vectorFordJohnson(std::vector<int>& temp_vec);
static int	findJacobsthal(int n);

// --- main function ---
int main(int ac, char** av)
{
	if (ac < 2)
	{
		invalidUsage();
		return (NOK);
	}

	std::vector<int> base_vec;

	if (parseArgs(ac, av, base_vec) == ERROR)
		return (NOK);

	// DEBUG
	std::cout << "Before:" << std::endl;
	for (size_t i = 0; i < base_vec.size(); i++)
		std::cout << "Value [" << i << "]: " << base_vec[i] << std::endl;

	vectorFordJohnson(base_vec);

	// DEBUG
	std::cout << "After:" << std::endl;
	for (size_t i = 0; i < base_vec.size(); i++)
		std::cout << "Value [" << i << "]: " << base_vec[i] << std::endl;

	return (OK);
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

static int	parseArgs(int ac, char** av, std::vector<int>& base_vec)
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
