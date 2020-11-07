#ifndef ZLL_BRACKETFORMATTER_H
#define ZLL_BRACKETFORMATTER_H

#include "Formatter.h"


namespace At0::Log
{
	class BracketFormatter : public Formatter
	{
	public:
		/// <summary>
		/// Default BracketFormatter Constructor
		/// </summary>
		BracketFormatter() = default;

		/// <summary>
		/// Adds the initial brackets "[]" to the string beginning
		/// </summary>
		/// <param name="str">Is the string which will be modified</param>
		/// <param name="logLvl">Is the current log level, unused in this function specification</param>
		virtual void Format(std::string& str, LogLevel logLvl) override
		{
#ifndef ZL_NO_BRACKETS

			str.insert(0, "[] ");

#endif // !ZL_NO_BRACKETS
		}
	};
}

#endif // ZZL_BRACKETFORMATTER_H
