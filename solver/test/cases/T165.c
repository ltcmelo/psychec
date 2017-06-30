GTree * g_tree_new_full ()
{
    g_return_val_if_fail (key_compare_func != NULL);
    tree->key_compare = key_compare_func;
    return tree;
}

void g_tree_insert_internal ()
{
    int cmp = tree->key_compare();
}
