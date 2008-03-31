#include <assert.h>
#include "registers.h"
#include "cas.h"
#include "cas-yacc.tab.h"

int expression_overhead (Expression *e)
{
    if (!e)
	return 1;
    switch (e->type) {
    case CONSTANT:
    case LABEL:
	return 2;
    case EXPRESSION:
	return 2 
	    + expression_overhead (e->detail.expression.left) 
	    + expression_overhead (e->detail.expression.right);
    case DUMMY:
	assert (e->type != DUMMY);
    }
    return 0;
}

Expression *newExpression (int operation, Expression *left, Expression *right)
{
    Expression *e = malloc (sizeof (Expression));
    if (!e) {
	perror ("malloc");
	exit (EXIT_FAILURE);
    }
    e->type = EXPRESSION;
    e->detail.expression.operation = operation;
    e->detail.expression.left = left;
    e->detail.expression.right = right;
    return e;
}

Expression *newConstant (int constant)
{
    Expression *e = malloc (sizeof (Expression));
    if (!e) {
	perror ("malloc");
	exit (EXIT_FAILURE);
    }
    e->type = CONSTANT;
    e->detail.constant = constant;
    return e;
}

Expression *newLabel (int label)
{
    Expression *e = malloc (sizeof (Expression));
    if (!e) {
	perror ("malloc");
	exit (EXIT_FAILURE);
    }
    e->type = LABEL;
    e->detail.label = label;
    return e;
}

Expression *do_math (int op, Expression *left, Expression *right) 
{
    if (left->type == CONSTANT && right && right->type == CONSTANT) {
	switch (op) {
	case '+':
	    left->detail.constant += right->detail.constant;
	    break;
	case '-':
	    left->detail.constant -= right->detail.constant;
	    break;
	case '*':
	    left->detail.constant *= right->detail.constant;
	    break;
	case '/':
	    left->detail.constant /= right->detail.constant;
	    break;
	case '%':
	    left->detail.constant %= right->detail.constant;
	    break;
	case '^':
	    left->detail.constant ^= right->detail.constant;
	    break;
	case '&':
	    left->detail.constant &= right->detail.constant;
	    break;
	case '|':
	    left->detail.constant |= right->detail.constant;
	    break;
	case T_LL:
	    left->detail.constant <<= right->detail.constant;
	    break;
	case T_GG:
	    left->detail.constant >>= right->detail.constant;
	    break;
	case UNARY_MIN:
	    left->detail.constant =- left->detail.constant;
	    break;
	case '!':
	    left->detail.constant =! left->detail.constant;
	    break;
	case '~':
	    left->detail.constant =~ left->detail.constant;
	    break;
	}
	free (right);
	return left;
    } else {
	return newExpression (op, left, right);
    }
}

#define min(x,y) ((x)<(y))?(x):(y)
int try_to_evaluate (Expression *e, struct LabelTable *labels, Dword *value, Dword *segment)
{
    Dword label, rvalue;
    int lstatus, rstatus;

    switch (e->type) {
    case CONSTANT:
	*value = e->detail.constant;
	return 1;
    case LABEL:
	label = e->detail.label;
	assert (label < labels->size);
	if (labels->labels[label].defined) {
	    if (*segment != NOT_FOUND && *segment != labels->labels[label].segment) {
		component_error ("invalid cross-segment reference", 
				 labels->labels[label].name);
		return -1;
	    } else {
		*value = labels->labels[label].address;
		*segment = labels->labels[label].segment;
		return 1;
	    }
	} else {
	    if (module_type != CLOF_EXE)
		assert (labels->labels[label].defined || module_type == CLOF_EXE);
	    return 0;
	}
	break;
    case EXPRESSION:
	lstatus = try_to_evaluate (e->detail.expression.left,  labels,  value, segment);
	rstatus = e->detail.expression.right
	    ? try_to_evaluate (e->detail.expression.right, labels, &rvalue, segment)
	    : 1;
	if (min (lstatus, rstatus) <= 0)
	    return min (lstatus, rstatus);
	
	switch (e->detail.expression.operation) {
	case '+':
	    *value += rvalue;
	    return 1;
	case '-':
	    *value -= rvalue;
	    return 1;
	case '*':
	    *value *= rvalue;
	    return 1;
	case '/':
	    *value /= rvalue;
	    return 1;
	case '%':
	    *value += rvalue;
	    return 1;
	case '^':
	    *value ^= rvalue;
	    return 1;
	case '&':
	    *value &= rvalue;
	    return 1;
	case '|':
	    *value |= rvalue;
	    return 1;
	case T_LL:
	    *value <<= rvalue;
	    return 1;
	case T_GG:
	    *value >>= rvalue;
	    return 1;
	case UNARY_MIN:
	    *value =- (*value);
	    return 1;
	case '!':
	    *value =! (*value);
	    return 1;
	case '~':
	    *value =~ (*value);
	    return 1;
	}
	break;
    case DUMMY:
	assert (e->type != DUMMY);
    }
    return 0;
}
