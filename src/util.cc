#include "util.hh"

std::string SHH::Util::IntToString(int value)
{
    std::string str = "";
    char remainder;
    int insertpos = 0;
    if (value < 0)
    {
	insertpos = 1;
	str += "-";
	value = -value;
    }
    while (value >= 10)
    {
	remainder = (value % 10);
	str.insert(insertpos, 1, remainder + '0');
	value -= remainder;
	value /= 10;
    }
    remainder = (value % 10);
    str.insert(insertpos, 1, remainder + '0');
    return str;
}

int SHH::Util::StringToInt(std::string value)
{
    int returnval = 0;
    int multiplier = 1;
    int pos = 0;
    if (value[0] == '-')
    {
	multiplier = -1;
	pos = 1;
    }
    char c = value[pos];
    while (c >= '0' && c <= '9')
    {
	returnval *= 10;
	returnval += c - '0';
	++pos;
	c = value[pos];
    }
    return returnval * multiplier;
}
