#include "RPN.hpp"

// --- helper functions declaration ---
static bool	isValidValue(const std::string& token);
static bool	isValidOperator(char oper);
static int	doOperation(int value_1, int value_2, char oper);
static void	generalError();
static void	rangeError();
static void	operatorError();
static void	remainderError();
static void	missingOperatorError();

// --- main function ---
int	RPN(const std::string& expression)
{
	bool				check_operator = false;
	std::stack<int>		stack;
	std::istringstream	parsing_iss(expression);
	std::string			token;

	while (parsing_iss >> token)
	{
		if (isValidValue(token))
		{
			std::istringstream iss_digits(token);
			int value;
			if (!(iss_digits >> value))
			{
				generalError();
				return (ERROR);
			}

			stack.push(value);
		}
		else if (token.size() == 1)
		{
			if (!isValidOperator(token[0]) || stack.size() < 2)
			{
				operatorError();
				return (ERROR);
			}
			if (!check_operator)
				check_operator = true;

			int	value_2 = stack.top();
			stack.pop();
			int	value_1 = stack.top();
			stack.pop();

			int total = doOperation(value_1, value_2, token[0]);
			if (total == ERROR)
			{
				rangeError();
				return (ERROR);
			}

			stack.push(total);
		}
		else
		{
			operatorError();
			return (ERROR);
		}
	}
	if (!check_operator)
	{
		missingOperatorError();
		return (ERROR);
	}
	if (stack.size() > 1)
	{
		remainderError();
		return (ERROR);
	}

	std::cout << "Result: " << stack.top() << std::endl;
	return (OK);
}





// --- helper functions definition ---
static bool	isValidValue(const std::string& token)
{
	if (token.empty())
		return (false);

	std::istringstream iss_digits(token);
	int value;
	if (!(iss_digits >> value))
		return (false);

	char check;
	return (!(iss_digits >> check));
}

static bool	isValidOperator(char oper)
{
	if (oper != '+' && oper != '-'
			&& oper != '*' && oper != '/')
		return (false);
	else
		return (true);
}

static int	doOperation(int value_1, int value_2, char oper)
{
	if (oper == '+')
		return (static_cast<long long>(value_1) + static_cast<long long>(value_2) <= INT_MAX ? value_1 + value_2 : ERROR);

	if (oper == '-')
		return (static_cast<long long>(value_1) - static_cast<long long>(value_2) >= INT_MIN ? value_1 - value_2 : ERROR);

	if (oper == '*')
		return (static_cast<long long>(value_1) * static_cast<long long>(value_2) <= INT_MAX ? value_1 * value_2 : ERROR);

	if (oper == '/' && value_2 != 0)
		return (static_cast<long long>(value_1) / static_cast<long long>(value_2) >= INT_MIN ? value_1 / value_2 : ERROR);

	return (ERROR);
}

static void	generalError()
{
	std::cerr << RED "Error:" RESET << " bad expression." << std::endl;
}

static void rangeError()
{
	std::cerr << RED "Error:" RESET << " invalid operation and/or expression result outside int range." << std::endl;
}

static void	operatorError()
{
	std::cerr << RED "Error:" RESET << " invalid character and/or too many operators." << std::endl;
}

static void	remainderError()
{
	std::cerr << RED "Error:" RESET << " remainder at the end." << std::endl;
}

static void	missingOperatorError()
{
	std::cerr << RED "Error:" RESET << " expression has no operator." << std::endl;
}
