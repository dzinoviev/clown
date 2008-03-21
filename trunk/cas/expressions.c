#include <assert.h>
#include "registers.h"
#include "cas.h"
#include "cas-yacc.tab.h"

static Expression *newExpression (int operation, Expression *left, Expression *right)
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
    case EXPRESSION:
	switch (e->detail.expression.operation) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '^':
	case '&':
	case '|':
	case T_LL:
	case T_GG:
	    lstatus = try_to_evaluate (e->detail.expression.left,  labels,  value, segment);
	    rstatus = try_to_evaluate (e->detail.expression.right, labels, &rvalue, segment);
	    if (min (lstatus, rstatus) <= 0)
		return min (lstatus, rstatus);
	    break;
	case UNARY_MIN:
	case '!':
	case '~':
	    lstatus = try_to_evaluate (e->detail.expression.left, labels, value, segment);
	    if (lstatus <= 0)
		return lstatus;
	    break;
	default:
	    assert (0);
	}

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
    }
    return 0;
}
