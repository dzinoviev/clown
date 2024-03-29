#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "registers.h"
#include "clowndev.h"

int expression_overhead(Expression *e)
{
  assert(e && e->type != DUMMY); // ???
  switch (e->type) {
  case CONSTANT:
  case LABEL:
  case SELECTOR:
    return 2;
  case EXPRESSION:
    return 2 
      + expression_overhead(e->detail.expression.left) 
      + expression_overhead(e->detail.expression.right);
  case DUMMY:
    break;
  }
  return 0;
}

Expression *newExpression(int operation, Expression *left, Expression *right)
{
  Expression *e = safe_malloc(sizeof(Expression));

  e->type = EXPRESSION;
  e->detail.expression.operation = operation;
  e->detail.expression.left = left;
  e->detail.expression.right = right;
  return e;
}

Expression *newConstant(Dword constant)
{
  Expression *e = safe_malloc(sizeof(Expression));

  e->type = CONSTANT;
  e->detail.constant = constant;
  return e;
}

Expression *newSelector(Selector s)
{
  Expression *e = safe_malloc(sizeof(Expression));

  e->type = SELECTOR;
  e->detail.constant = s;
  return e;
}

Expression *newLabel(int label)
{
  Expression *e = safe_malloc(sizeof(Expression));

  e->type = LABEL;
  e->detail.label = label;
  return e;
}

Expression *do_math(int op, Expression *left, Expression *right) 
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
    case C_LL:
      left->detail.constant <<= right->detail.constant;
      break;
    case C_GG:
      left->detail.constant >>= right->detail.constant;
      break;
    case C_UNARY_MIN:
      left->detail.constant = -left->detail.constant;
      break;
    case '!':
      left->detail.constant = !left->detail.constant;
      break;
    case '~':
      left->detail.constant = ~left->detail.constant;
      break;
    }
    free(right);
    return left;
  } else {
    return newExpression(op, left, right);
  }
}

#define min(x,y) ((x)<(y))?(x):(y)
int try_to_evaluate(Expression *e, struct LabelTable *labels,
		    struct SegmentTable *st, Dword *value, Dword *segment)
{
  Dword label, rvalue;
  Selector s;
  int lstatus, rstatus;

  switch (e->type) {
  case CONSTANT:
    *value = e->detail.constant;
    return 1;
  case SELECTOR:
    s = (Selector)(e->detail.constant);
    label = st->segments[SEL_ID(s)].new_loc;
    *value = MK_SELECTOR(label, SEL_RPL(s), SEL_TABL(s));
    return 1;
  case LABEL:
    label = e->detail.label;
    assert(label < labels->size);
    if (labels->labels[label].defined) {
      if (*segment != NOT_FOUND && *segment != labels->labels[label].segment) {
	component_error("expression",
			 "invalid cross-segment reference", 
			 labels->labels[label].name);
	return -1;
      } else {
	*value = labels->labels[label].address;
	*segment = labels->labels[label].segment;
	return 1;
      }
    } else {
      if (module_type != CLOF_EXE)
	assert(labels->labels[label].defined || module_type == CLOF_EXE);
      return 0;
    }
    break;
  case EXPRESSION:
    lstatus = try_to_evaluate(e->detail.expression.left,  labels, st, value, segment);
    rstatus = e->detail.expression.right
      ? try_to_evaluate(e->detail.expression.right, labels, st, &rvalue, segment)
      : 1;
    if (min(lstatus, rstatus) <= 0)
      return min(lstatus, rstatus);
	
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
    case C_LL:
      *value <<= rvalue;
      return 1;
    case C_GG:
      *value >>= rvalue;
      return 1;
    case C_UNARY_MIN:
      *value = -(*value);
      return 1;
    case '!':
      *value = !(*value);
      return 1;
    case '~':
      *value = ~(*value);
      return 1;
    }
    break;
  case DUMMY:
    assert(e->type != DUMMY);
  }
  return 0;
}

static Expression *restore_expression(Dword* code, int *pointer)
{
    Dword type = code[(*pointer)++];

    switch (type) {
    case CONSTANT:
	return newConstant(code[(*pointer)++]);
    case SELECTOR:
 	return newSelector(code[(*pointer)++]);
    case LABEL:
 	return newLabel(code[(*pointer)++]);
    case EXPRESSION:
      {
	Dword op = code[(*pointer)++];
	Expression *left = restore_expression(code, pointer);
	Expression *right = restore_expression(code, pointer);
	return newExpression(op, left, right);
      }
    case DUMMY:
	return NULL;
    }
    return NULL;		/* this never happens */
}

Dword *build_expressions(Dword* code, int wordcodesize,
			 int *truesize/*, int *escapes*/)
{
  Dword *truecode = safe_malloc(2 * sizeof(Dword) * wordcodesize);

  for (int i = 0; i < wordcodesize; ) {
    /* escape sequence or actual instruction */
    switch (truecode[(*truesize)++] = code[i++]) {
    case FIX_EXPRESSION:
    case FIX_ADISPLACEMENT:
      truecode[(*truesize)++] = code[i++]; /* "instruction" */
      Expression *e = restore_expression(code, &i);
	    
      truecode[(*truesize)++] = (Dword)((int64_t)e << 32 >> 32);
      truecode[(*truesize)++] = (Dword)((int64_t)e >> 32);
      break;
    case FIX_SEGMENT:
      break;
    case FIX_RDISPLACEMENT:
      break;
    default:
      break;
    }
  }

  truecode = safe_realloc(truecode, sizeof(Dword) * (*truesize));

  return truecode;
}
