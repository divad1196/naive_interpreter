#include <iostream>
#include <vector>
#include <string>

std::string::size_type NOT_FOUND(std::string::npos);


//std::vector<std::string> split(std::string str, char c,bool keep=false)
//{
	//std::vector<std::string> values;
	//size_t last_c(0);
	//char last_char(str[0]);
	//for(size_t i(0); i < str.size(); i++)
	//{
		//if(str[i] == c and last_char != c)
		//{
			//std::string tmp(str.substr(last_c,i - last_c));
			//values.push_back(tmp);
			//if(keep)
				//values.push_back(std::string(&str[i]));
			//last_c = i + 1;
			//last_char = str[i];
		//}
		//else last_char = str[i];
	//}
	//std::string tmp(str.substr(last_c, str.size() - last_c));
	//if(last_char != c)
	//{
		//values.push_back(tmp);
	//}
	//return values;
//}



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

std::vector<std::string> split(std::string str, std::string list,bool keep=false)
{
	std::cout << "================================================" << std::endl;
	std::cout << "fonction 2" << std::endl;
	std::cout << str << std::endl;

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
				values.push_back(std::string(&star[i],1));
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
	std::cout << "================================================" << std::endl;
	return values;
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

float compute(std::string expression)
{
	std::vector<std::string> tmp(split(expression,"+-*/",true));
	std::cout << "================================================" << std::endl;
	std::cout << "for auto compute" << std::endl;
	for(auto i: tmp)
		std::cout << i << std::endl;
	std::cout << "================================================" << std::endl;

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


		// trouver le premier + ou - et scinder à cet endroit, puis compute
		size_t pos1(expression.find("+")), pos2(expression.find("-"));
		size_t split_pos(0);
		if(pos1 != NOT_FOUND and pos2 != NOT_FOUND)
		{

			std::cout << "TEST1" << std::endl;
			if(pos1 < pos2)
				split_pos = pos1;
			else
				split_pos = pos2;
		}
		else if(pos1 != NOT_FOUND and pos2 == NOT_FOUND)
		{
			std::cout << "TEST2" << std::endl;
			split_pos = pos1;
		}
		else if(pos1 == NOT_FOUND and pos2 != NOT_FOUND)
		{
			std::cout << "TEST3" << std::endl;
			split_pos = pos2;
		}
		else
		{
			// pareil avec les * et /
			std::cout << "TEST4" << std::endl;
			pos1 = expression.find("*");
			pos2 = expression.find("/");
			if(pos1 != NOT_FOUND and pos2 != NOT_FOUND)
			{
				std::cout << "TEST4.1" << std::endl;
				if(pos1 < pos2)
					split_pos = pos1;
				else
					split_pos = pos2;
			}
			else if(pos1 != NOT_FOUND and pos2 == NOT_FOUND)
			{
				std::cout << "TEST4.2" << std::endl;
				split_pos = pos1;
			}
			else if(pos1 == NOT_FOUND and pos2 != NOT_FOUND)
			{
				std::cout << "TEST4.3" << std::endl;
				split_pos = pos2;
			}
			else
			{
				std::cout << "TEST4.4" << std::endl;
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

int main(int argc, char* argv[])
{


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

/*
 * NOTE:
 * il faut d'abord scinder aux + et - car on va décomposer et calculer dans l'order INVERSE
 * Donc en decomposant les +- puis * et / puis ^ on va recalculer ^ puis * et / puis + et -
 *
 * Pour calculer de gauche à droite, pareil, il faut decomposer en partant de la fin de la liste
 *
 * Pas besoin de créer de nouveaux objets, il suffit de garder en mémoire les positions des symboles
 * 6 * 3 + 4 - 5
 * les positions:
 * 		* : 2
 * 		+ : 6
 * 		- : 10
 * /