#include "readydeque.h"

/*********************************************************
 * Management of ReadyDeques
 *********************************************************/

/* 
 * functions that add/remove elements from the top/bottom
 * of deques
 *
 * ANGE: the precondition of these functions is that the worker w -> self 
 * must have locked worker pn's deque before entering the function
 */
Closure *deque_xtract_top(__cilkrts_worker *const w, int pn) {

  Closure *cl;

  /* ANGE: make sure w has the lock on worker pn's deque */
  deque_assert_ownership(w, pn);

  cl = w->g->deques[pn].top;
  if(cl) {
    CILK_ASSERT(w, cl->owner_ready_deque == pn);
    w->g->deques[pn].top = cl->next_ready;
    /* ANGE: if there is only one entry in the deque ... */
    if (cl == w->g->deques[pn].bottom) {
      CILK_ASSERT(w, cl->next_ready == (Closure *) NULL);
      w->g->deques[pn].bottom = (Closure *) NULL;
    } else {
      CILK_ASSERT(w, cl->next_ready);
      (cl->next_ready)->prev_ready = (Closure *) NULL;
    }
    WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
  } else {
    CILK_ASSERT(w, w->g->deques[pn].bottom == (Closure *)NULL);
  }

  return cl;
}

Closure *deque_peek_top(__cilkrts_worker *const w, int pn) {

  Closure *cl;

  /* ANGE: make sure w has the lock on worker pn's deque */
  deque_assert_ownership(w, pn);

  /* ANGE: return the top but does not unlink it from the rest */
  cl = w->g->deques[pn].top;
  if(cl) {
    CILK_ASSERT(w, cl->owner_ready_deque == pn);
  } else {
    CILK_ASSERT(w, w->g->deques[pn].bottom == (Closure *)NULL);
  }

  return cl;
}

Closure *deque_xtract_bottom(__cilkrts_worker *const w, int pn) {

  Closure *cl;

  /* ANGE: make sure w has the lock on worker pn's deque */
  deque_assert_ownership(w, pn);

  cl = w->g->deques[pn].bottom;
  if(cl) {
    CILK_ASSERT(w, cl->owner_ready_deque == pn);
    w->g->deques[pn].bottom = cl->prev_ready;
    if(cl == w->g->deques[pn].top) {
      CILK_ASSERT(w, cl->prev_ready == (Closure *) NULL);
      w->g->deques[pn].top = (Closure *) NULL;
    } else {
      CILK_ASSERT(w, cl->prev_ready);
      (cl->prev_ready)->next_ready = (Closure *) NULL;
    }

    WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
  } else {
    CILK_ASSERT(w, w->g->deques[pn].top == (Closure *)NULL);
  }

  return cl;
}

Closure *deque_peek_bottom(__cilkrts_worker *const w, int pn) {

  Closure *cl;

  /* ANGE: make sure w has the lock on worker pn's deque */
  deque_assert_ownership(w, pn);

  cl = w->g->deques[pn].bottom;
  if(cl) {
    CILK_ASSERT(w, cl->owner_ready_deque == pn);
  } else {
    CILK_ASSERT(w, w->g->deques[pn].top == (Closure *)NULL);
  }

  return cl;
}

void deque_assert_is_bottom(__cilkrts_worker *const w, Closure *t) {

  /* ANGE: still need to make sure the worker self has the lock */
  deque_assert_ownership(w, w->self);
  CILK_ASSERT(w, t == deque_peek_bottom(w, w->self));
}

/*
 * ANGE: this allow w -> self to append Closure cl onto worker pn's ready
 *       deque (i.e. make cl the new bottom).
 */
void deque_add_bottom(__cilkrts_worker *const w, Closure *cl, int pn) {

  deque_assert_ownership(w, pn);
  CILK_ASSERT(w, cl->owner_ready_deque == NOBODY);

  cl->prev_ready = w->g->deques[pn].bottom;
  cl->next_ready = (Closure *)NULL;
  w->g->deques[pn].bottom = cl;
  WHEN_CILK_DEBUG(cl->owner_ready_deque = pn);

  if (w->g->deques[pn].top) {
    CILK_ASSERT(w, cl->prev_ready);
    (cl->prev_ready)->next_ready = cl;
  } else {
    w->g->deques[pn].top = cl;
  }
}


/* ANGE: remove closure for frame f from bottom of pn's deque and _really_ 
 *       free them (i.e. not internal-free).  As far as I can tell. 
 *       This is called only in invoke_main_slow in invoke-main.c. 
 */
void Cilk_remove_and_free_closure_and_frame(__cilkrts_worker *const w,
					    __cilkrts_stack_frame *f, int pn) {
  Closure *t;

  deque_lock(w, pn);
  t = deque_xtract_bottom(w, pn);

  CILK_ASSERT(w, t->frame == f);
  USE_UNUSED(t); 
  deque_unlock(w, pn);

  /* ANGE: there is no splitter logging in the invoke_main frame */
  //Cilk_free(f);
  //Closure_destroy_malloc(w, t);
}
