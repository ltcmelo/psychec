void f()
{
  char const *groups = NULL;

  if (uid_set (uid) && !groups) {}

  if (uid_set (uid) || groups) {}
}
