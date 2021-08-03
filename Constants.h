#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace Utils
{

namespace Constants
{


constexpr unsigned long long Kilo = 1000;
constexpr unsigned long long Mega = Kilo*Kilo;
constexpr unsigned long long Giga = Mega*Kilo;
constexpr unsigned long long Tera = Giga*Kilo;

constexpr unsigned long long operator""_Kilo(unsigned long long const  val )
{
	return Kilo * val;
}

constexpr unsigned long long operator""_Mega( unsigned long long const  val )
{
	return Mega * val;
}

constexpr unsigned long long operator""_Giga( unsigned long long const  val )
{
	return Giga * val;
}

constexpr unsigned long long operator""_Tera( unsigned long long const  val )
{
	return Tera * val;
}

constexpr unsigned long long KB = 1024;
constexpr unsigned long long MB = KB*KB;
constexpr unsigned long long GB = MB*KB;
constexpr unsigned long long TB = GB*KB;

constexpr unsigned long long operator""_KB( unsigned long long const  val )
{
	return KB * val;
}

constexpr unsigned long long operator""_MB( unsigned long long const  val )
{
	return MB * val;
}

constexpr unsigned long long operator""_GB( unsigned long long const  val )
{
	return GB * val;
}

constexpr unsigned long long operator""_TB( unsigned long long const  val )
{
	return TB * val;
}

}

} // NS Utils

#endif // CONSTANTS_H
