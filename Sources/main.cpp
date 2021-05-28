#include "pch.h"

// template</*typename... Args, */ std::source_location location = std::source_location::current()>
// static void Trace(std::string_view msg /*, Args&&... args*/)
//{
//	// std::ostringstream oss;
//	// oss << location.file_name << '\n';
//
//	// auto insert = [&oss](auto arg) { oss << arg << ' '; };
//	//(insert(args), ...);
//	// std::cout << oss.str() << '\n';
//}

class Log
{
public:
	template<typename... Ts>
	struct Trace
	{
		Trace(std::string_view msg, Ts&&... ts, std::source_location loc = std::source_location::current())
		{
			std::cout << loc.function_name() << " line " << loc.line() << ": ";
			std::cout << msg << '\n';
			((std::cout << std::forward<Ts>(ts) << " "), ...);
			std::cout << std::endl;
		}
	};

	template<typename... Ts>
	Trace(std::string_view msg, Ts&&...) -> Trace<Ts...>;
};


int main()
{
	auto location = std::source_location::current();

	Log::Trace("Test", 5, 'A', 3.14f, "foo");
}
