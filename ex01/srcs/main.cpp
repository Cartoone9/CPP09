#include <iostream>
#include <sstream>

#include "colors.hpp"
#include "dictionary.hpp"
#include "RPN.hpp"

// --- helper functions declaration ---
static int	badUsage();

// --- main function ---
int main(int ac, char** av)
{
	(void)av;

	if (ac < 2)
		return (badUsage(), NOK);

	std::string expression(av[1]);

	if (ac > 2)
	{
		std::ostringstream oss;
		for (int i = 1; i < ac; i++)
		{
			oss << av[i];
			if (i + 1 < ac)
				oss << " ";
		}
		expression = oss.str();
	}

	if (RPN(expression) == ERROR)
		return (NOK);

	return (OK);
}





// --- helper functions definition ---
static int	badUsage()
{
	std::cerr << RED "Error:" RESET << " bad usage => ";
	std::cerr << "./RPN \"<RPN expression>\"" << std::endl;
	std::cerr << YELLOW "Example: " RESET << "./RPN \"3 4 5 * +\"" << std::endl;

	return (NOK);
}
