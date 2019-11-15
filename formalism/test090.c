int main()
{
    T* v;
    v->next = v;
    v->next->next = v;
    v->next->next->next = v;
    v->next->next->next->next = v;
    v->next->next = v->next->next;
    return 0;
}
