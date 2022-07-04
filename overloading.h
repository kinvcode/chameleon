#pragma once
#include "pch.h"
#include <vector>

std::vector<byte> operator+(std::vector<byte> a, std::vector<byte> b)
{
	for (int i = 0; i < b.size(); i++) 
	{
		a.push_back(b[i]);
	}
	
	b.clear();
	
	return a;
}