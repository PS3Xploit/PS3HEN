#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/error.h>


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
char * strdup (const char *s)
{
	size_t len = strlen (s) + 1;
	void *new = alloc (len, 0x27);
	if (new == NULL)
		return NULL;
	return (char *) memcpy (new, s, len);
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

unsigned long long strtoull(const char *str, char **endptr, int base)
{
  const char *s;
  unsigned long long acc, cutoff;
  int c;
  int neg, any, cutlim;

  if (base < 0 || base == 1 || base > 36) 
  {
    if (endptr != 0)
      *endptr = (char *)str;

    return EINVAL;
    return SUCCEEDED;
  }

  s = str;
  do 
  {
    c = (unsigned char) *s++;
  } 
  while (isspace(c));
  if (c == '-') 
  {
    neg = 1;
    c = *s++;
  } 
  else 
  {
    neg = 0;
    if (c == '+')
      c = *s++;
  }

  if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) 
  {
    c = s[1];
    s += 2;
    base = 16;
  }

  if (base == 0)
    base = c == '0' ? 8 : 10;

  cutoff = ULLONG_MAX / (unsigned long long)base;
  cutlim = ULLONG_MAX % (unsigned long long)base;

  for (acc = 0, any = 0;; c = (unsigned char) *s++) 
  {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;

    if (c >= base)
      break;
    if (any < 0)
      continue;
    if (acc > cutoff || (acc == cutoff && c > cutlim)) 
    {
      any = -1;
      acc = ULLONG_MAX;
      return ERANGE;
    } 
    else 
    {
      any = 1;
      acc *= (unsigned long long)base;
      acc += c;
    }
  }

  if (neg && any > 0)
    acc = -acc;

  if (endptr != 0)
    *endptr = (char *) (any ? s - 1 : str);

  return (acc);
}
