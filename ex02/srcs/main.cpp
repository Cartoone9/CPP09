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
	// for (size_t i = 0; i < temp_vec.size(); i++)
	// 	std::cout << "Value [" << i << "]: " << temp_vec.at(i) << std::endl;

	vectorFordJohnson(base_vec);

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
		straggler = base_vec.at(base_vec_len - 1);
		base_vec.pop_back();
		base_vec_len--;
	}

	std::vector<std::pair<int, int> >	pairs;
	std::vector<int>					winners;

	// form the pairs, preorder them so that
	// first is always the biggest of the pair
	// and keep a vector of winners aside for recursion
	for (size_t i = 0; i < base_vec_len; i += 2)
	{
		int	first = base_vec.at(i);
		int	second = base_vec.at(i + 1);

		if (first > second)
		{
			pairs.push_back(std::make_pair(first, second));
			winners.push_back(first);
		}
		else
		{
			pairs.push_back(std::make_pair(second, first));
			winners.push_back(second);
		}
	}

	vectorFordJohnson(winners);

	std::vector<int> main_stack = winners;
	std::vector<int> pend_stack;

	for (size_t i = 0; i < main_stack.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (main_stack.at(i) == pairs.at(j).first)
			{
				// add second to the pend (looooser)
				pend_stack.push_back(pairs.at(j).second);
				// remove the pair to avoid duplicates
				pairs.erase(pairs.begin() + j);
				// break and go to the next main element (winner)
				break;
			}
		}
	}

	// insert pend into main here
	
	// if is_odd insert straggler here
	
	base_vec = main_stack;
}
