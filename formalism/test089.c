int main()
{
    T* v;
    v->next = v;
    v->next->next = v;
    v->next->next->next = v;
    v->next->next->next->next = v;
    return 0;
}
