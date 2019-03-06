#include <stdio.h>

//~ float strtofl(const char str[])
//~ {
	//~ bool valid(true);
	//~ float number(0);
	//~ int div(1);
	//~ bool comma(false);

	//~ int coeff(1);
	//~ size_t start(0);
	//~ if(str[0] == '-')
	//~ {
		//~ coeff = -1;
		//~ start = 1;
	//~ }

	//~ for(size_t i(start); str[i] != '\0' and valid; i++)
	//~ {
		//~ if( 48 <= str[i] and str[i] <= 57 and !comma)
			//~ number = number * 10 + str[i] - 48;
		//~ else if(str[i] == '.' and !comma)
			//~ comma = true;
		//~ else if( 48 <= str[i] and str[i] <= 57)
		//~ {
			//~ number += ((float)(str[i] - 48)) / div;
			//~ div *= 10;
		//~ }
		//~ else
			//~ valid = false;
	//~ }
	//~ return number*coeff;
//~ }



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

struct SubExpr {
	char open;

	size_t start;
	size_t len;

	};
typedef struct SubExpr SubExpr;

struct Expression {
	char* expr;

	SubExpr* liste;
	size_t len;//remove from here

	};
typedef struct Expression Expression;

void add(Expression& tab,SubExpr expr)
{
	tab.liste[tab.len] = expr;
	tab.len += 1;
}

void show(const Expression& tab)
{
	for(size_t n(0); n < tab.len; n++) //add verification if the sub_expression is closed and correct
		printf("%.*s\n",tab.liste[n].len + 1,tab.expr + tab.liste[n].start);
}

float compute(Expression& expr)
{
	//~ Do a struct to handle those 2?

	for(size_t i(0); expr.expr[i] != '\0'; i++)
	{
		if(	expression[i] == '(')
		{
			add(expr,{'(',i,0})
		}
		else if( expression[i] == ')')
		{
			int match_pos(-1);
			size_t index;
			for(size_t j(0); j < expr.len; j++)
			{
				if(expr.liste[j].open == '(' and expr.liste[j].len == 0)
				{
					match_pos = i;
					index = j;
				}
			}
			if(match_pos != -1)
				expr.liste[index].len = match_pos - expr.liste[index].start; // + 1 because there would be 2 characters? Now: no, so we can use the len as index for the array
			// else : raise error -> invalid brackets
		}
	}



	return 0;
}

int main(int argc, char* argv[])
{
	//~ if(1)
		//~ std::cout << "True" << std::endl;

	// remove all space
	//~ std::string str(argv[1]);
	//~ for(size_t j(0); j < str.size(); j++)
	//~ {
		//~ if(str[j] == ' ')
		//~ {
			//~ str.replace(j,1,"");
			//~ j--;
		//~ }
	//~ }

	SubExpr liste[50];
	Expression expr({argv[1],list,0});

	compute(expr);
	return 0;
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
 */