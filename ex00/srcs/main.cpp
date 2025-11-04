#include <fstream>
#include <iostream>
#include <string>

#include "colors.hpp"
#include "dictionary.hpp"
#include "BitcoinExchange.class.hpp"

// --- helper functions declaration ---
static int	badInput();
static int	badInfile();
static int	badDatabase();

// --- main function ---
int main(int ac, char** av)
{
	if (ac != 2)
		return (badInput());

	std::ifstream infile(av[1]);
	if (!infile)
		return (badInfile());

	std::ifstream dbfile("data.csv");
	if (!dbfile)
		return (badDatabase());

	BitcoinExchange	btc_obj(infile, dbfile);

	btc_obj.readInfile();

	return (OK);
}





// --- helper functions definition ---
static int	badInput()
{
	std::cerr << RED "Error:" RESET << " bad usage => ";
	std::cerr << "./btc <infile>" << std::endl;

	return (NOK);
}

static int	badInfile()
{
	std::cerr << RED "Error:" RESET << " could not open infile." << std::endl;

	return (NOK);
}

static int	badDatabase()
{
	std::cerr << RED "Error:" RESET << " could not open \"data.csv\"." << std::endl;

	return (NOK);
}
