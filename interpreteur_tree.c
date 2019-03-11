#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct SubExpr;
typedef struct SubExpr SubExpr;

//~ inline
float plus(float left, float right){return left + right;}
//~ inline
float minus(float left, float right){return left - right;}
//~ inline
float mult(float left, float right){return left * right;}
//~ inline
float divf(float left, float right){return left / right;}
//~ inline
float power(float left, float right){return powf(left,right);}

struct SubExpr {
	float value;

	SubExpr* left;
	SubExpr* right;

	float (*operation)(float left,float right);
	};

SubExpr* evaluateSubExpr(SubExpr* sub_expr)
{
	if(sub_expr->left != NULL && sub_expr->right != NULL)
		sub_expr->value = sub_expr->operation(evaluateSubExpr(sub_expr->left)->value,evaluateSubExpr(sub_expr->right)->value);
	return sub_expr;
}

void freeSubExpr(SubExpr* sub_expr)
{
	if(sub_expr->left != NULL)
		freeSubExpr(sub_expr->left);
	if(sub_expr->right != NULL)
		freeSubExpr(sub_expr->right);
	free(sub_expr);
}

const size_t SUBEXPR_SIZE = sizeof(SubExpr);

enum OperatorFound {
	OF_NONE,
	OF_POWER,
	OF_MULT_DIV,
	OF_PlUS_MINUS
};
typedef enum OperatorFound OperatorFound;

SubExpr* parse_expression(char expression[],size_t length)//OperatorFound level_max ? -> to force the stop sooner
{
	//~ printf("%.*s\n",length,expression);
	//We receive an expression without (), for example 5^3 + 3 - 4 * 5
	char* op = NULL;
	OperatorFound level = OF_NONE;

	int is_open = 0; // we will handle by levels and recursively: ( 5 * (2 + 4) ) - 2 will see only one bracket : ( 5 * (2 + 4) ), this will be considered has a new expression
	int brackets_found = 0;

	for(size_t i = length-1; i > 0 && level != OF_PlUS_MINUS; i--)//from end to the begining
	{
		if(expression[i] == '(')
			is_open -= 1;
		else if(!is_open)
		{
			if(expression[i] == ')')// because we read backward
			{
				is_open += 1;
				brackets_found = 1;
			}
			else if(expression[i] == '+' || expression[i] == '-')
			{
				op = expression + i;
				level = OF_PlUS_MINUS;
			}
			else if(level < OF_MULT_DIV && (expression[i] == '*' || expression[i] == '/'))
			{
				op = expression + i;
				level = OF_MULT_DIV;
			}
			else if(level < OF_POWER && expression[i] == '^')
			{
				op = expression + i;
				level = OF_POWER;
			}
		}
		//if(is_open < 0) -> ERROR : trop de (
	}
	//if(is_open > 0) -> ERROR : trop de )


	if(level == OF_NONE && brackets_found)//It's an expression in brackets
		return parse_expression(expression+1,length - 2); // works because we took back the spaces -> it removes the externals brackets /!\ -2 because we remove 2 characters

	SubExpr* sub_expr = malloc(SUBEXPR_SIZE);
	if(level == OF_NONE && !brackets_found)//It's only a float
	{
		sub_expr->left = NULL;
		sub_expr->right = NULL;
		sub_expr->value = atof(expression);
		return sub_expr;
	}

	size_t len = op - expression;

	sub_expr->left = parse_expression(expression,len);
	sub_expr->right = parse_expression(op + 1,length - len - 1);

	if(*op == '+')
		sub_expr->operation = plus;
	else if(*op == '-')
		sub_expr->operation = minus;
	else if(*op == '*')
		sub_expr->operation = mult;
	else if(*op == '/')
		sub_expr->operation = divf;
	else if(*op == '^')
		sub_expr->operation = power;

	//~ printf("======================\n");
	//~ printf("%.*s = %f\n",length,expression,sub_expr->value);

	return sub_expr;
}

// /!\ this function only generate the data structures and evaluate it at the end by calling evaluateSubExpr
SubExpr* parse(char expression[],size_t length)
{
	//~ printf("%.*s\n",length,expression);

	//get the total length without the spaces -> /!\ Necessary to avoid problems and verifications
	size_t len = 0;
	for(size_t i = 0; i < length; i++)
	{
		if(expression[i] != ' ')
			len += 1;
	}

	//Malloc the array for the expression without spaces
	char* new_array = malloc(sizeof(char)*(len+1));
	if (new_array == NULL) // Si l'allocation a échoué
    {
		printf("Allocation failed\n");
        exit(0); // On arrête immédiatement le programme
    }

	//Fill the array
	size_t index = 0;
	for(size_t i = 0; i < length; i++)
	{
		if(expression[i] != ' ')
		{
			new_array[index] = expression[i];
			index += 1;
		}
	}
	new_array[len] = '\0';
	//~ printf("%.*s\n",len,new_array);
	//Start to compute
	SubExpr* result = parse_expression(new_array,len);

	//Free the array that was malloc
	free(new_array);
	return evaluateSubExpr(result);
}

int main(int argc, char* argv[])
{
	SubExpr* result = parse(argv[1],strlen(argv[1]));
	printf("%f\n",result->value);
	freeSubExpr(result);
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