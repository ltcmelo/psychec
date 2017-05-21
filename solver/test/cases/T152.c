void f() {
    T x;
    x->y = 10;
    x->z->w = 10;
    x->m->n->p = 3.14;
    x->m->n->q.r = "a";
    x->m->n->q.s = 'b';
    B syn = x;

    JJ jj;
    jj->kk->hh->xkhi = 10;
    jj->kk->hh->abcd = 3.14;
    jj->kkkkkk.hhhhh->ptrobj = 10;
    jj->kkkkkk.hhhhh->ptrobj2 = "foo";

    YYY yyy;
    yyy->uuu = 10;
    x->xxx = yyy;

    Whatever orphan;
    x->i = orphan;

    double thedouble;
    x->known = thedouble;

    a->b = 10;
    a->c->d = 10;
    a->e.f.g = x;

    Rec rec;
    rec->next = rec;

    cer->prev = cer;
    cer->big = x;
    cer->prev->prev = cer;
}
