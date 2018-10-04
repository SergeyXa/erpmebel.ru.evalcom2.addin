#pragma once

#include <string>
#include <vector>

class CInterpreter
{
	std::string program;
	std::vector<double> values;
public:
	void Load(std::string program);
	void SetValue(double value);
	void SetValue(bool value);
	void SetValue(int value);
	bool Execute(double& result);
};

