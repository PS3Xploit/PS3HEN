#include <lv2/lv2.h>
#include <lv2/libc.h>

char *strstr(const char *haystack, const char *needle)
{
	size_t needlelen;
	/* Check for the null needle case.  */
	if (*needle == '\0')
		return (char *) haystack;

	needlelen = strlen(needle);
	for (; (haystack = strchr(haystack, *needle)) != NULL; haystack++)
		if (strncmp(haystack, needle, needlelen) == 0)
			return (char *) haystack;		
		return NULL;
}

int strncasecmp (__const char *s1, __const char *s2, size_t n)
{
	/* TODO: locale support */
  int c1, c2;
  while (n > 0)
    {
      /* Use "unsigned char" to make the implementation 8-bit clean */
      c1 = *((unsigned char *)(s1++));
      if (c1 >= 'A' && c1 <= 'Z')
        c1 = c1 + ('a' - 'A');
      c2 = *((unsigned char *)(s2++));
      if (c2 >= 'A' && c2 <= 'Z')
        c2 = c2 + ('a' - 'A');
      if (c1 != c2)
        {
          return (c1 - c2);
        }
      if (c1 == '\0')
        {
          return 0;
        }
      --n;
    }
  return 0;
}
