#ifndef ZZL_LOGCONFIGURE_H
#define ZZL_LOGCONFIGURE_H


/// <summary>
/// All mutexes and lock guards will be disabled if ZL_LOG_NON_THREAD_SAVE is defined
/// </summary>
//#define ZL_LOG_NON_THREAD_SAVE

/// <summary>
/// The current time and date will not be added to the output string if ZL_NO_TIME_OUT is defined
/// </summary>
//#define ZL_NO_DATETIME_OUT

/// <summary>
/// The log level will not be added to the output string if ZL_NO_LOG_LEVEL_OUT is defined
/// </summary>
//#define ZL_NO_LOG_LEVEL_OUT

/// <summary>
/// No brackets will be added to place other information into if ZL_NO_BRACKETS is defined.
/// Also dissalows program from outputing anything else
/// </summary>
//#define ZL_NO_BRACKETS


#ifdef ZL_NO_BRACKETS
	#define ZL_NO_LOG_LEVEL_OUT
	#define ZL_NO_DATETIME_OUT
#endif

#endif // ZZL_LOGCONFIGURE_H
