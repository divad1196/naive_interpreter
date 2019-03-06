#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

//for C, size_t error: expected declaration specifiers or ‘...’ before numeric constant size_t sub_expr_len(0);


float evaluate(float left, float right, char symbol)
{
	//printf("evaluate: %d,%s,%d\n",left,symbol,right);
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

	char* start;

	size_t start_index;
	size_t len;

	};
typedef struct SubExpr SubExpr;

void print_subExpr(const SubExpr* sub_expr)
{
	printf("%.*s\n",sub_expr->len + 1,sub_expr->start);
}

enum OperatorFound {
	OF_NONE,
	OF_PlUS_MINUS,
	OF_MULT_DIV,
	OF_POWER
};
typedef enum OperatorFound OperatorFound;

float compute_expression(char expression[],size_t length)
{
	//We receive an expression without (), for example 5^3 + 3 - 4 * 5
	char* op = NULL;
	OperatorFound level = OF_NONE;
	printf("======================\n");
	printf("%.*s\n",length,expression);
	printf("level: %d\n",level);
	printf("Searching\n");
	for(size_t i = 0; i < length && OF_POWER; i++)
	{
		if(expression[i] == '^')
		{
			op = expression + i;
			level = OF_POWER;
		}
		else if(level < OF_MULT_DIV && (expression[i] == '*' || expression[i] == '/'))
		{
			op = expression + i;
			level = OF_MULT_DIV;
		}
		else if(level < OF_PlUS_MINUS && (expression[i] == '+' || expression[i] == '-'))
		{
			op = expression + i;
			level = OF_PlUS_MINUS;
		}
	}
	printf("level: %d\n",level);

	if(level == OF_NONE)
		return strtod(expression,NULL);

	size_t len = op - expression;

	float result = evaluate(
		compute_expression(expression,len),
		*op,
		compute_expression(op + 1,length - len)
	);

	printf("tmp: %d\n",result);

	return result;
}

float compute_sub_expressions(char expression[],size_t length)
{
	//printf("%.*s\n",length,expression);
	// Do a struct to handle those 2?
	SubExpr sub_expressions[50];
	size_t sub_expr_len = 0;

	int is_open = 0; // we will handle by levels and recursively: ( 5 * (2 + 4) ) - 2 will see only one bracket : ( 5 * (2 + 4) ), this will be considered has a new expression


	for(size_t i = 0; i < length; i++)
	{
		// Currently handle brackets () only
		if(	expression[i] == '(')
		{
			if(!is_open)
			{
				sub_expressions[sub_expr_len] = (SubExpr){'(',&expression[i],i,0};
				sub_expr_len += 1;
			}
			is_open += 1;
		}
		else if( expression[i] == ')')
		{
			if(is_open == 1)
			{
				int match_pos = -1;
				size_t index;
				for(size_t j = 0; j < sub_expr_len; j++)
				{
					if(sub_expressions[j].open == '(' && sub_expressions[j].len == 0)
					{
						match_pos = i;
						index = j;
					}
				}
				if(match_pos != -1)
					sub_expressions[index].len = match_pos - sub_expressions[index].start_index; // + 1 because there would be 2 characters? Now: no, so we can use the len as index for the array
				// else : raise error -> invalid brackets
			}
			is_open -= 1;
		}
	}

	//if(!is_open) // handle the case where open > 0 and open < 0
		// raise error: a bracket was opened but never closed

	if(!sub_expr_len) // means we couldn't find other sub_expression: we have to compute the expression
	{
		compute_expression(expression,length);
	}
	else
	{
		for(size_t n = 0; n < sub_expr_len; n++) //add verification if the sub_expression is closed and correct
		{
			//~ print_subExpr(sub_expressions[n]);
			compute_sub_expressions(sub_expressions[n].start + 1,sub_expressions[n].len - 1); // recursivité
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
	printf("levels: %d, %d, %d, %d\n",OF_NONE,OF_PlUS_MINUS,OF_MULT_DIV,OF_POWER);

	//get the total length without the spaces
	size_t length = 0;
	for(size_t i = 1; i < argc; i++)
	{
		for(size_t j = 0; argv[i][j] != '\0'; j++)
		{
			if(argv[i][j] != ' ')
				length += 1;
		}
	}

	//Malloc the array for the expression without spaces
	char* expression = malloc(sizeof(char)*(length+1));
	if (expression == NULL) // Si l'allocation a échoué
    {
		printf("Allocation failed\n");
        exit(0); // On arrête immédiatement le programme
    }

	//Fill the array
	size_t index = 0;
	for(size_t i = 1; i < argc; i++)
	{
		for(size_t j = 0; argv[i][j] != '\0'; j++)
		{
			if(argv[i][j] != ' ')
			{
				expression[index] = argv[i][j];
				index += 1;
			}
		}
	}
	expression[length] = '\0';

	//Start to compute
	float result = compute_sub_expressions(expression,length);
	printf("result = %f\n",result);

	//Free the array that was malloc
	free(expression);

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

//TODO: scinder et evaluer une expression sans parenthèse -- DONE

//lire un chiffre comme une expression qu'on évalue: les opérateurs n'ont des relations qu'avec des floats