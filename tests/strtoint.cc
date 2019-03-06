#include <iostream>
#include <vector>
#include <string>

std::string::size_type NOT_FOUND(std::string::npos);

int strtoint(char str[])
{
	int number(0);
	int coeff(1);
	size_t start(0);
	if(str[0] == '-')
	{
		coeff = -1;
		start = 1;
	}
		
	for(size_t i(start); str[i] != '\0'; i++)
	{
		if( 48 <= str[i] and str[i] <= 57)
			number = number * 10 + (int)str[i] - 48;
	}
	return number*coeff;
}

float strtofl(const char str[])
{
	bool valid(true);
	float number(0);
	int div(1);
	bool comma(false);
	
	int coeff(1);
	size_t start(0);
	if(str[0] == '-')
	{
		coeff = -1;
		start = 1;
	}
		
	for(size_t i(start); str[i] != '\0' and valid; i++)
	{
		if( 48 <= str[i] and str[i] <= 57 and !comma)
			number = number * 10 + str[i] - 48;
		else if(str[i] == '.' and !comma)
			comma = true;
		else if( 48 <= str[i] and str[i] <= 57)
		{
			number += ((float)(str[i] - 48)) / div;
			div *= 10;
		}
		else
			valid = false;
	}
	return number*coeff;
}
float strtofl(const std::string str)
{
	return strtofl(str.c_str());
}

float evaluate(float left, float right, char symbol)
{
	if(symbol == '+')
		return left + right;
	else if(symbol == '-')
		return left - right;
	else if(symbol == '*')
		return left * right;
	else if(symbol == '/')
		return left / right;
	//else f(symbol == '^')
	//	return left / right;
}

std::vector<std::string> split(std::string str, char c,bool keep=false)
{
	std::vector<std::string> values;
	size_t last_c(0);
	char last_char(str[0]);
	for(size_t i(0); i < str.size(); i++)
	{
		if(str[i] == c and last_char != c)
		{
			std::string tmp(str.substr(last_c,i - last_c));
			values.push_back(tmp);
			if(keep)
				values.push_back(std::string(&str[i]));
			last_c = i + 1;
			last_char = str[i];
		}
		else last_char = str[i];
	}
	std::string tmp(str.substr(last_c, str.size() - last_c));
	if(last_char != c)
	{
		values.push_back(tmp);
	}
	return values;
}

std::vector<std::string> split(std::string str, std::string list,bool keep=false)
{
	std::cout << "fonction 2" << std::endl;
	std::vector<std::string> values;
	size_t last_c(0);
	char last_char(str[0]);
	for(size_t i(0); i < str.size(); i++)
	{
		if(list.find(str[i]) != NOT_FOUND and list.find(last_char) == NOT_FOUND)// and list.find(last_char) == NOT_FOUND
		{
			std::string tmp(str.substr(last_c,i - last_c));
			values.push_back(tmp);
			if(keep)
				values.push_back(std::string(&str[i],1));
			last_c = i + 1;
			last_char = str[i];
		}
		else
		{
			last_char = str[i];
			last_c = i;
		}
	}
	std::string tmp(str.substr(last_c, str.size() - last_c));
	if(list.find(last_char) == NOT_FOUND)
	{
		values.push_back(tmp);
	}
	std::cout << "splited" << std::endl;
	return values;
}

void test(int argc, char* argv[])
{
	std::cout << strtofl(argv[1]) + 4 << std::endl;
	int a(60);
	std::cout << (char)a << std::endl;
	std::cout << ('1'  + '9') << std::endl;

}

//works only for template "x operator y"
void test1(int argc, char* argv[])
{
	std::vector<std::string> values;
	
	for(size_t i(1); i < argc; i++)
	{
		std::string str(argv[i]);
		std::vector<std::string> tmp(split(str,' '));
		values.insert( values.end(), tmp.begin(), tmp.end());
	}
	
	float left(strtofl(values[0]));
	float right(strtofl(values[2]));
	std::cout << values[0] << values[1] << values[2] << std::endl;
	std::cout << evaluate(left,right,values[1][0]) << std::endl;
}

float compute(std::string expression)
{
	std::vector<std::string> tmp(split(expression,"+-*/",true));
	for(auto i: tmp)
		std::cout << i << std::endl;
	
	if(tmp.size() == 1)
		return strtofl(expression);
	else if(tmp.size() == 3)
	{
		float left(strtofl(tmp[0]));
		float right(strtofl(tmp[2]));
		return evaluate(left,right,tmp[1][0]);
	}
	else
	{
		// trouver les parenthèse et les évaluer, sinon
		
		
		// trouver le premier * ou / et scinder à cet endroit, puis compute
		size_t pos1(expression.find("*")), pos2(expression.find("/"));
		size_t split_pos(0);
		if(pos1 != NOT_FOUND and pos2 != NOT_FOUND)
		{
			
			std::cout << "* and /" << std::endl;
			if(pos1 < pos2)
				split_pos = pos1;
			else
				split_pos = pos2;
		}
		else if(pos1 != NOT_FOUND and pos2 == NOT_FOUND)
		{
			std::cout << "* " << std::endl;
			split_pos = pos1;
		}
		else if(pos1 == NOT_FOUND and pos2 != NOT_FOUND)
		{
			std::cout << "/" << std::endl;
			split_pos = pos2;		
		}
		else
		{
			// pareil avec les + et -
			
			pos1 = expression.find("+");
			pos2 = expression.find("-");
			if(pos1 != NOT_FOUND and pos2 != NOT_FOUND)
			{
				if(pos1 < pos2)
					split_pos = pos1;
				else
					split_pos = pos2;
			}
			else if(pos1 != NOT_FOUND and pos2 == NOT_FOUND)
			{
					split_pos = pos1;
			}
			else if(pos1 == NOT_FOUND and pos2 != NOT_FOUND)
			{
					split_pos = pos2;		
			}
			else
			{
				std::cout << "+ and -" << std::endl;
				return strtofl(expression);
			}
		}
		
		std::string left(expression.substr(0,split_pos));
		std::string right(expression.substr(split_pos+1,expression.size()-1));
		std::string symbol(expression.substr(split_pos,1));
		std::string new_expression (std::to_string(compute(left)) + symbol + std::to_string(compute(right)));
		std::cout << "New expression: " << new_expression << std::endl;
		return compute(new_expression);
	}
}

void test2(int argc, char* argv[])
{
	//stock les sous-string à évaluer
	std::vector<std::string> values;
	
	for(size_t i(1); i < argc; i++)
	{
		//std::string list("+-*/")
		std::string str(argv[i]);
		
		// remove all space
		//std::cout << "before:" << str << std::endl;
		for(size_t j(0); j < str.size(); j++)
		{
			if(str[j] == ' ')
			{
				str.replace(j,1,"");
				j--;
			}
		}
		//std::cout << "after:" << str << std::endl;
		
		std::vector<std::string> tmp(split(str,"+-*/",true));
		values.insert( values.end(), tmp.begin(), tmp.end());
	}
	for(auto i: values)
		std::cout << i << std::endl;
}

void test3(int argc, char* argv[])
{
	//stock les sous-string à évaluer
	std::vector<std::string> values;
	
	for(size_t i(1); i < argc; i++)
	{
		//std::string list("+-*/")
		std::string str(argv[i]);
		
		// remove all space
		//std::cout << "before:" << str << std::endl;
		for(size_t j(0); j < str.size(); j++)
		{
			if(str[j] == ' ')
			{
				str.replace(j,1,"");
				j--;
			}
		}
		//std::cout << "after:" << str << std::endl;
	}
	for(auto i: values)
		std::cout << i << std::endl;
	
	//float left(strtofl(values[0]));
	//float right(strtofl(values[2]));
	//std::cout << values[0] << values[1] << values[2] << std::endl;
	//std::cout << evaluate(left,right,values[1][0]) << std::endl;
}

int main(int argc, char* argv[])
{
	//test(argc,argv);
	//test1(argc,argv);
	//test2(argc,argv);
	
	
	// remove all space
	std::string str(argv[1]);
	for(size_t j(0); j < str.size(); j++)
	{
		if(str[j] == ' ')
		{
			str.replace(j,1,"");
			j--;
		}
	}
	std::cout << str << std::endl;
	std::cout << compute(str) << std::endl;
	
}
