#include "BitcoinExchange.class.hpp"

// --- constructors / destructor ---
BitcoinExchange::BitcoinExchange(std::ifstream& infile, std::ifstream& dbfile)
	: _infile(infile)
{
	std::string line;
	while (std::getline(dbfile, line))
	{
		if (!line.compare("date,exchange_rate"))
			continue;

		std::string date = line.substr(0, 10);
		std::string value_str = line.substr(11);

		if (date.empty() || value_str.empty() || line[10] != ',')
			continue;

		std::istringstream iss(value_str);
		float value_float;
		if (iss >> value_float)
			_dbData[date] = value_float;
	}
}

BitcoinExchange::~BitcoinExchange()
{

}





// --- methods ---
static bool	dayIsInvalid(int day, int month, int year)
{
	if (day < 1 || day > 31)
		return (true);

	// february
	if (month == 2)
	{
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		{
			// leap year
			if (day > 29)
				return (true);
		}
		else if (day > 28)
			return (true);
	}
	// april, june, september, november
	else if ((month == 4 || month == 6 || month == 9 || month == 11)
			&& day > 30)
		return (true);

	return (false);
}

static bool	isOnlyDigit(std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!std::isdigit(str.at(i)))
			return (false);
	}
	return (true);
}

static bool	validInfileDate(std::string& token)
{
	if (token.size() != 10)
		return (false);

	if (token.at(4) != '-' || token.at(7) != '-')
		return (false);

	std::string	year_str = token.substr(0, 4);
	std::string	month_str = token.substr(5, 2);
	std::string	day_str = token.substr(8, 2);

	if (!isOnlyDigit(year_str) || !isOnlyDigit(month_str) || !isOnlyDigit(day_str))
		return (false);

	int	year_int = std::atoi(year_str.c_str());
	int	month_int = std::atoi(month_str.c_str());
	int	day_int = std::atoi(day_str.c_str());

	if (year_int <= 2000 || year_int > 2025)
		return (false);
	if (month_int < 1 || month_int > 12)
		return (false);
	if (dayIsInvalid(day_int, month_int, year_int))
		return (false);

	return (true);
}

static bool parseAndValidateValue(const std::string& token, float& infile_value)
{
	std::istringstream  iss(token);
	char                check;
	float               f;

	if (!(iss >> f) || f < 0.0f || f > 1000.0f)
		return (false);

	if (iss >> check)
		return (false);

	infile_value = f;
	return (true);
}

static bool	isValidLine(std::string& line,
		std::string& infile_date,
		float& infile_value,
		const BitcoinExchange& obj)
{
	std::istringstream iss(line);
	std::string date, sep, value;

	if (!(iss >> date >> sep >> value))
	{
		obj.badLine(line);
		return (false);
	}
	if (!validInfileDate(date))
	{
		obj.badDate(date);
		return (false);
	}
	if (sep != "|")
	{
		obj.badLine(line);
		return (false);
	}
	if (!parseAndValidateValue(value, infile_value))
	{
		obj.badValue(value);
		return (false);
	}

	char check;
	if (iss >> check)
	{
		obj.badLine(line);
		return (false);
	}

	infile_date = date;
	return (true);
}

void    BitcoinExchange::transformLine(const std::string& infile_date, float infile_value)
{
	std::map<std::string, float>::iterator it = _dbData.upper_bound(infile_date);

	std::string	db_date;
	float		db_value;

	if (it != _dbData.begin())
	{
		--it;

		db_date = it->first;
		db_value = it->second;
	}
	else
	{
		db_date = "0";
		db_value = 0.0f;
	}

	std::ostringstream oss;
	oss << db_value << " on " << db_date;

	std::cout << GREEN " Valid: " RESET
		<< infile_date << "  =>  " << infile_value
		<< " (" << (db_date.compare("0") ? (oss.str()) : "no data") << ")"
		<< " = " REVERSED " " << db_value * infile_value << " " RESET << std::endl;
}

void	BitcoinExchange::readInfile()
{
	std::string	line;
	bool		first_line = true;

	while (std::getline(_infile, line))
	{
		// check header
		if (first_line)
		{
			first_line = false;
			if (line.compare("date | value") == OK)
				continue;
			else
				missingHeader();
		}

		std::cout << " ------------------------------------------------------------ " << std::endl;

		std::string	infile_date;
		float		infile_value;

		if (!isValidLine(line, infile_date, infile_value, *this))
			continue;

		transformLine(infile_date, infile_value);
	}
}





// --- errors ---
void	BitcoinExchange::missingHeader() const
{
	std::cerr << RED " Error:" RESET << " missing header => date | value" << std::endl;
}

void	BitcoinExchange::badLine(const std::string& line) const
{
	std::cerr << RED " Error:" RESET << " bad line    =>  ";
	std::cerr << ORANGE << (!line.empty() ? line : "(empty)") << RESET << std::endl;
}

void	BitcoinExchange::badDate(const std::string& date) const
{
	std::cerr << RED " Error:" RESET << " bad date    =>  ";
	std::cerr << ORANGE << date << RESET << std::endl;
}

void	BitcoinExchange::badValue(const std::string& value) const
{
	std::cerr << RED " Error:" RESET << " bad value   =>  ";
	std::cerr << ORANGE << value << RESET << std::endl;
}
