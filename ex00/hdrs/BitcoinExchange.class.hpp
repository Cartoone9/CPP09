#ifndef BITCOINEXCHANGE_CLASS_HPP
#define BITCOINEXCHANGE_CLASS_HPP

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "colors.hpp"
#include "dictionary.hpp"

class BitcoinExchange
{
	public:
		BitcoinExchange(std::ifstream& infile, std::ifstream& database);
		~BitcoinExchange();

		void	readInfile();
		void	transformLine(const std::string& infile_date, float infile_value);
		void	missingHeader() const;
		void	badLine(const std::string& input) const;
		void	badDate(const std::string& date) const;
		void	badValue(const std::string& value) const;
		void	noDbData(const std::string& date) const;

	private:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& old_obj);
		BitcoinExchange& operator=(const BitcoinExchange& old_obj);

		std::ifstream&					_infile;
		std::map<std::string, float>	_dbData;
};

#endif // #ifndef BITCOINEXCHANGE_CLASS_HPP
