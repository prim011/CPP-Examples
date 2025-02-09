#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdint>

using namespace std;

// Utility function for pudding a string to a desired length 
void padTo(string &str, const size_t num, const char paddingChar = ' ') {
    if(num > str.size())
      str.insert(0, num - str.size(), paddingChar);
    //else
    //  str.erase(num - str.size(), str.size());
}

// prints string as hex
string phex(uint8_t* str, size_t size)
{
  char sstr[size];
  for (int i = 0; i < size; ++i)
    sprintf(sstr+i, "%.2x", str[i]);
  
  return sstr;
}
