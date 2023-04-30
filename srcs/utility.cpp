#include "utility.hpp"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

#include <stdio.h>

string Utility::delete_space(string& str)
{
    const std::string& space= "\t\n\v\f\r ";
    ssize_t first_space = str.find_first_not_of(space);
    ssize_t last_space = str.find_last_not_of(space);
    if (first_space < 0 || last_space < 0)
    {
        return ("");
    }
    return (str.substr(first_space, last_space+1));
}


size_t Utility::strlen(const char *str)
{
    size_t cnt = 0;
    while(str[cnt]){
        cnt++;
    }
    return (cnt);
}

char *Utility::memcpy(char *dst, const char *src, size_t n)
{
    size_t           i;
    unsigned char    *dst_p;
    unsigned char    *src_p;

    if (n == 0)
        return (dst);
    else if (dst == NULL && src == NULL)
        return (NULL);
    i = n;
    dst_p = (unsigned char *)dst;
    src_p = (unsigned char *)src;
    while (i--){
        //cout << "i=" << i << endl;
        *(dst_p++) = *(src_p++);
    }
    return (dst);
}


static char	*process(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	k;
	size_t	len_n;

	i = -1;
	k = 0;
	len_n = Utility::strlen(needle);
	if (len_n == 0)
		return ((char *)haystack);
	if (len_n > Utility::strlen(haystack) || len_n > len)
		return (NULL);
	while (++i < len)
	{
		if (haystack[i] == needle[k])
			k++;
		else
		{
			i -= k;
			k = 0;
		}
		if (len_n == k)
			return (&(((char *)haystack)[i - len_n + 1]));
	}
	return (NULL);
}

char	*Utility::strnstr(const char *haystack, const char *needle, size_t len)
{
	if (len == 0)
	{
		if (needle == NULL || needle[0] == '\0')
			return ((char *)haystack);
		return (NULL);
	}
	return (process(haystack, needle, len));
}

