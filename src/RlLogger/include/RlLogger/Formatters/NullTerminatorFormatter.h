#ifndef ZZL_NULLTERMINATORFORMATTER_H
#define ZZL_NULLTERMINATORFORMATTER_H


#include "Formatter.h"


namespace At0::Log
{
	class NullTerminatorFormatter : public Formatter
	{
	public:
		/// <summary>
		/// Default NullTerminatorFormatter Constructor
		/// </summary>
		NullTerminatorFormatter() = default;

		/// <summary>
		/// Removes Null-Termination-Character from string
		/// </summary>
		/// <param name="str">Is the string to format</param>
		/// <param name="logLvl">Is the current log level</param>
		virtual void Format(std::string& str, LogLevel logLvl) override
		{
			for (uint32_t i = 0; i < str.size(); ++i)
			{
				if (str[i] == '\0')
				{
					str.erase(str.begin() + i, str.begin() + i + 1);
					--i;
				}
			}
		}
	};
}

#endif // ZZL_NULLTERMINATORFORMATTER_H
