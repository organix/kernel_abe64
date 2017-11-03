/*
 * cons.h -- LISP-like "CONS" cell management
 *
 * Copyright 2008-2017 Dale Schumacher.  ALL RIGHTS RESERVED.
 */
#ifndef CONS_H
#define CONS_H

#include <assert.h>
#include "types.h"

extern CELL		nil__cons;

#define	NIL		as_cons(&nil__cons)
#define	nilp(p)	((p) == NIL))
#if 0
#define	car(p)	((p)->first)
#define	cdr(p)	((p)->rest)
#else
#define	car(p)	_car(p)
#define	cdr(p)	_cdr(p)
#endif

#define	BM_TYPE		as_word(3)	/* 2#0000...0011 */
#define	BF_CONS		as_word(0)	/* 2#0000...0000 */
#define	BF_ACTOR	as_word(1)	/* 2#0000...0001 */
#define	BF_OBJECT	as_word(1)	/* 2#0000...0001 */
#define	BF_ATOM		as_word(2)	/* 2#0000...0010 */
#define	BF_NUMBER	as_word(3)	/* 2#0000...0011 */
#define	BF_FUNC		as_word(3)	/* 2#0000...0011 */
#define	TYPE_OF(p)	(as_word(p) & BM_TYPE)

#define	MK_CONS(p)	as_cons((as_word(p) & ~BM_TYPE) | BF_CONS)
#define	MK_ACTOR(p)	as_cons((as_word(p) & ~BM_TYPE) | BF_ACTOR)
#define	MK_OBJECT(p) as_cons((as_word(p) & ~BM_TYPE) | BF_OBJECT)
#define	MK_ATOM(p)	as_cons((as_word(p) & ~BM_TYPE) | BF_ATOM)
#define	MK_NUMBER(p) as_cons((as_word(p) << 2) | BF_NUMBER)
#define	MK_FUNC(p)	as_cons((as_word(p) << 2) | BF_FUNC)
#define MK_REF(p)	MK_FUNC(p)

#define	MK_INT(p)	((int)(as_word(p)>>2))
#define	MK_PTR(p)	((void*)(as_word(p)>>2))

#define	consp(p)	(((p) != FALSE) && (TYPE_OF(p) == BF_CONS))
#define	actorp(p)	(((p) != TRUE) && (TYPE_OF(p) == BF_ACTOR))
#define	objectp(p)	(((p) != TRUE) && (TYPE_OF(p) == BF_OBJECT))
#define	atomp(p)	(TYPE_OF(p) == BF_ATOM)
#define	numberp(p)	(TYPE_OF(p) == BF_NUMBER)
#define	funcp(p)	(TYPE_OF(p) == BF_FUNC)

#define	map_get(map,key)	map_get_def((map), (key), NULL)

BOOL	_nilp(CONS* cons);
#if 0
BOOL	_actorp(CONS* cons);
#endif
CONS*	cons(CONS* car, CONS* cdr);
CONS*	_car(CONS* cons);
CONS*	_cdr(CONS* cons);
CONS*	rplaca(CONS* cons, CONS* car);
CONS*	rplacd(CONS* cons, CONS* cdr);
BOOL	equal(CONS* x, CONS* y);
CONS*	append(CONS* x, CONS* y);
CONS*	reverse(CONS* list);
int		length(CONS* list);
CONS*	replace(CONS* form, CONS* map);
CONS*	map_find(CONS* map, CONS* key);
CONS*	map_get_def(CONS* map, CONS* key, CONS* def);
CONS*	_map_get(CONS* map, CONS* key);
CONS*	map_put(CONS* map, CONS* key, CONS* value);
CONS*	map_put_all(CONS* map, CONS* amap);
CONS*	map_def(CONS* map, CONS* keys, CONS* values);
CONS*	map_remove(CONS* map, CONS* key);
CONS*	map_cut(CONS* map, CONS* key);

void	test_cons();
void	report_cons_usage();
BOOL	assert_equal_cons(char* msg, CONS* expect, CONS* actual);

#define	assert_equal(e,a)	assert(equal((e),(a)))

/* O(1) queue management macros */
#define	CQ_EMPTY(q)		nilp(car(q))
#define	CQ_PUT(q, e)	rplacd((q), nilp(car(q)) ? rplaca((q), (e)) : rplacd(cdr(q), (e)))
#define	CQ_PUSH(q, e)	rplaca((q), nilp(rplacd((e), car(q))) ? rplacd((q), (e)) : (e))
#define	CQ_POP(q)		rplaca((q), cdr(car(q)))
#define	CQ_PEEK(q)		car(q)

#endif /* CONS_H */
