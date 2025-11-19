#include <iostream>
#include <vector>

#include "colors.hpp"
#include "dictionary.hpp"

#define OK 0
#define NOK 1
#define ERROR -1

// --- helper functions declaration ---
static void	invalidUsage();
static void	invalidArg();
static void	vectorFordJohnson();
static int	parseArgs(int ac, char** av, std::vector<int> temp_vec);

// --- main function ---
int main(int ac, char** av)
{
	if (ac < 2)
	{
		invalidUsage();
		return (NOK);
	}

	std::vector<int> temp_vec;

	if (parseArgs(ac, av, temp_vec) == ERROR)
		return (NOK);

	vectorFordJohnson(temp_array);

	return (OK);
}





// --- helper functions definition ---
static void	invalidUsage()
{
	std::cerr << RED "Error: " RESET << "invalid use: ./PmergeMe <values>" << std::endl;
}

static int	parseArgs(int ac, char** av, std::vector<int> temp_vec)
{
	// check each char* with std::isdigit after passing leading whitespace
	for (int i = 0; i < ac; i++)
	{
		// pass the whitespaces
		while (std::isspace(*av[i]))
			av[i]++;

		int j = 0;
		while (av[i][j])
		{
			if (std::isspace(av[i][j]))
			{
				// take up to 'j' and insert in temp_vec
				// while isspace j++
				// av[i] += j;
				// j = 0;
			}
			if (!std::isdigit(av[i][j]))
			{
				invalidArg();
				return (ERROR);
			}
			j++;
		}
	}
}

static void	invalidArg()
{
	std::cerr << RED "Error: " RESET << "invalid arg in given values" << std::endl;
}

static void	vectorFordJohnson()
{
	// using a std::vector
	//
	// first make the pairs of blocks and pre-sort while keeping the order stored
	// recursively call the function with bigger blocks untils max blocks < 2
	//
	// now from the biggest blocks, make a main and a pend
	// sort the pend into the main using the Jacobsthal seq
	// if you can't use the Jacobsthal seq then use the pend reverse order

}
