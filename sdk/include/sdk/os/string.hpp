/**
 * @file
 * @brief String manipulation functions.
 * 
 * For documentation, see the C standard library.
 */
#pragma once

extern "C"
char *String_Strcat(char *dest, const char *src);

extern "C"
const char *String_Strchr(const char *str, char c);

extern "C"
int String_Strcmp(const char *str1, const char *str2);

extern "C"
char *String_Strcpy(char *destination, const char *source);

extern "C"
int String_Strlen(const char *str);
