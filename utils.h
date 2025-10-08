//utils.h

//Header guards
#ifndef UTILS_H    //Check if UTILS_H is not defined
#define UTILS_H    //Defining UTILS_H now
//Header files
#include <stdio.h>
#include <stdbool.h>
//Functions included
int string_compare(const char *str1,const char *str2);      //Compare two strings
bool string_starts(const char *str1,const char *prefix);    //Check whether string starts with given prefix or not
void to_lowercase(char *str1);                              //Conver Upper case to lower case
void remove_symbols(char *str1);                            //Remove unwanted symbols
#endif                                                      //End of guard