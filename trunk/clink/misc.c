#include <stdio.h>
#include <assert.h>
#include "clowndev.h"
#include "clink.h"

int newSegment (struct Segment *s)
{
    if (modules[current_module].st.size <= s->id) {
	modules[current_module].st.segments = realloc (modules[current_module].st.segments, 
						       (s->id + 1) * sizeof (struct Segment));
	if (!modules[current_module].st.segments) {
	    perror ("realloc");
	    return 0;
	}
	modules[current_module].st.size = s->id + 1;
    }
    s->in_use = 1;
    modules[current_module].st.segments[s->id] = *s;
    modules[current_module].st.segments[s->id].module = current_module;

    return 1;
}

int newSymbol (struct Label *s)
{
    if (modules[current_module].lt.size <= s->id) {
	modules[current_module].lt.labels = realloc (modules[current_module].lt.labels, 
						     (s->id + 1) * sizeof (struct Label));
	if (!modules[current_module].lt.labels) {
	    perror ("realloc");
	    return 0;
	}
	modules[current_module].lt.size = s->id + 1;
    }
    s->in_use = 1;
    modules[current_module].lt.labels[s->id] = *s;

    return 1;
}

static Expression *restore_expression (Dword* code, int *pointer)
{
    Expression *left, *right;
    Dword op;
    Dword type = code[(*pointer)++];

    /* DEBUG */ /* fprintf (stderr, "TYPE: 0x%08X\n", type);*/
    switch (type) {
    case CONSTANT:
	/* DEBUG */ /* fprintf (stderr, "CONST: 0x%08X\n", code[*pointer]); */
	return newConstant (code[(*pointer)++]);
    case LABEL:
	/* DEBUG */ /* fprintf (stderr, "LABEL: 0x%08X\n", code[*pointer]); */
	return newLabel (code[(*pointer)++]);
    case EXPRESSION:
	/* DEBUG */ /* fprintf (stderr, "OP: 0x%08X\n", code[*pointer]); */
	op = code[(*pointer)++];
	left = restore_expression (code, pointer);
	right = restore_expression (code, pointer);
	return newExpression (op, left, right);
    case DUMMY:
	return NULL;
    default:
	assert (0);
    }
    return NULL;		/* this never happens */
}

Dword *build_expressions (Dword* code, int wordcodesize, int *truesize, int *escapes)
{
    Dword *truecode;    
    int i;

    *truesize = 0;    
    truecode = malloc (sizeof (Dword) * wordcodesize);
    if (!truecode) {
	perror ("malloc");
	return NULL;
    }

    /* DEBUG */ /* fprintf (stderr, "wordcodesize=%d\n", wordcodesize);	*/
    for (i = 0; i < wordcodesize; ) {
	/* DEBUG */ /* fprintf (stderr, "READ: %d 0x%08X\n", i, code[i]); */
	switch (truecode[(*truesize)++] = code[i++]) {/* escape sequence or actual instruction */
	case FIX_EXPRESSION:
	case FIX_ADISPLACEMENT:
	    /* DEBUG */ /* fprintf (stderr, "READ*: %d 0x%08X\n", i, code[i]); */
	    truecode[(*truesize)++] = code[i++]; /* "instruction" */
	    truecode[(*truesize)++] = (Dword)restore_expression (code, &i);
	    (*escapes)+=2;
	    break;
	case FIX_SEGMENT:
	case FIX_RDISPLACEMENT:
	    (*escapes)++;
	    break;
	default:
	    break;
	}
    }

    /* DEBUG */ /* fprintf (stderr, "TS=%d\n", *truesize);	*/
    truecode = realloc (truecode, sizeof (Dword) * (*truesize));
    if (!truecode) {
	perror ("realloc");
	return NULL;
    }

    return truecode;
}
