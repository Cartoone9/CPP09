#include "RPN.hpp"

// --- helper functions declaration ---
static bool			isValidValue(const std::string& token, int& value);
static int			handleOperator(const char oper, std::stack<int>& stack,
									bool& check_operator);
static bool			isValidOperator(char oper);
static long long	doOperation(int value_1, int value_2, char oper);
static void			rangeError();
static void			operatorError();
static void			remainderError();
static void			divisionByZeroError();
static void			missingOperatorError();

// --- main function ---
int	RPN(const std::string& expression)
{
	bool				check_operator = false;
	std::stack<int>		stack;
	std::istringstream	parsing_iss(expression);
	std::string			token;

	while (parsing_iss >> token)
	{
		int	value = 0;

		if (isValidValue(token, value))
			stack.push(value);
		else if (token.size() == 1)
		{
			if (handleOperator(token[0], stack, check_operator) == ERROR)
				return (ERROR);
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

	std::cout << UNDERLINE "Result:" RESET
			<< " " REVERSED " " << stack.top() 
			<< " " RESET << std::endl;

	return (OK);
}





// --- helper functions definition ---
static bool	isValidValue(const std::string& token, int& value)
{
	if (token.empty())
		return (false);

	std::istringstream iss_digits(token);
	if (!(iss_digits >> value))
		return (false);

	char check;
	return (!(iss_digits >> check));
}

static int	handleOperator(const char oper, std::stack<int>& stack,
							bool& check_operator)
{
	if (!isValidOperator(oper) || stack.size() < 2)
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
	if (oper == '/' && value_2 == 0)
	{
		divisionByZeroError();
		return (ERROR);
	}

	long long total = doOperation(value_1, value_2, oper);
	if (total == ERROR_LL)
	{
		rangeError();
		return (ERROR);
	}

	stack.push(static_cast<int>(total));

	return (OK);
}

static bool	isValidOperator(char oper)
{
	return (oper == '+' || oper == '-' || oper == '*' || oper == '/');
}

static long long doOperation(int value_1, int value_2, char oper)
{
	long long result = 0;

	if (oper == '+')
		result = static_cast<long long>(value_1) + value_2;
	else if (oper == '-')
		result = static_cast<long long>(value_1) - value_2;
	else if (oper == '*')
		result = static_cast<long long>(value_1) * value_2;
	else if (oper == '/')
		result = static_cast<long long>(value_1) / value_2;
	else
		return (ERROR_LL);

	if (result > INT_MAX || result < INT_MIN)
		return (ERROR_LL);

	return (result);
}

static void rangeError()
{
	std::cerr << RED "Error:" RESET
		<< " invalid operation and/or expression result outside int range." << std::endl;
}

static void	operatorError()
{
	std::cerr << RED "Error:" RESET
		<< " invalid character and/or too many operators." << std::endl;
}

static void	remainderError()
{
	std::cerr << RED "Error:" RESET
		<< " remainder at the end." << std::endl;
}

static void divisionByZeroError()
{
    std::cerr << RED "Error:" RESET
		<< " division by zero." << std::endl;
}

static void	missingOperatorError()
{
	std::cerr << RED "Error:" RESET
		<< " expression has no operator." << std::endl;
}
