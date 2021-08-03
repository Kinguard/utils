#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace Utils
{

namespace Constants
{


constexpr uint64_t Kilo = 1000;
constexpr uint64_t Mega = Kilo*Kilo;
constexpr uint64_t Giga = Mega*Kilo;
constexpr uint64_t Tera = Giga*Kilo;

constexpr uint64_t operator""_Kilo( unsigned long long const  val )
{
	return Kilo * val;
}

constexpr uint64_t operator""_Mega( unsigned long long const  val )
{
	return Mega * val;
}

constexpr uint64_t operator""_Giga( unsigned long long const  val )
{
	return Giga * val;
}

constexpr uint64_t operator""_Tera( unsigned long long const  val )
{
	return Tera * val;
}

constexpr uint64_t KB = 1024;
constexpr uint64_t MB = KB*KB;
constexpr uint64_t GB = MB*KB;
constexpr uint64_t TB = GB*KB;

constexpr uint64_t operator""_KB( unsigned long long const  val )
{
	return KB * val;
}

constexpr uint64_t operator""_MB( unsigned long long const  val )
{
	return MB * val;
}

constexpr uint64_t operator""_GB( unsigned long long const  val )
{
	return GB * val;
}

constexpr uint64_t operator""_TB( unsigned long long const  val )
{
	return TB * val;
}

}

} // NS Utils

#endif // CONSTANTS_H
