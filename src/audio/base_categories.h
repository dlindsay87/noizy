#ifndef __BASE_CATEGORIES_H__
#define __BASE_CATEGORIES_H__

template <typename U, typename... Args> struct Category {
	const char *label;
	U (*fn)(Args...);
};

#endif