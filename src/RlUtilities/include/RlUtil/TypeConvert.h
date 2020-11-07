#ifndef RLU_TYPECONVERT_H
#define RLU_TYPECONVERT_H

#include <string>

#pragma region Windows.h
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <Windows.h>

#undef NOGDICAPMASKS
#undef NOVIRTUALKEYCODES
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOSYSMETRICS
#undef NOMENUS
#undef NOICONS
#undef NOKEYSTATES
#undef NOSYSCOMMANDS
#undef NORASTEROPS
#undef NOSHOWWINDOW
#undef OEMRESOURCE
#undef NOATOM
#undef NOCLIPBOARD
#undef NOCOLOR
#undef NOCTLMGR
#undef NODRAWTEXT
#undef NOGDI
#undef NOKERNEL
#undef NOUSER
#undef NONLS
#undef NOMB
#undef NOMEMMGR
#undef NOMETAFILE
#undef NOMINMAX
#undef NOMSG
#undef NOOPENFILE
#undef NOSCROLL
#undef NOSERVICE
#undef NOSOUND
#undef NOTEXTMETRIC
#undef NOWH
#undef NOWINOFFSETS
#undef NOCOMM
#undef NOKANJI
#undef NOHELP
#undef NOPROFILER
#undef NODEFERWINDOWPOS
#undef NOMCX
#pragma endregion

namespace DirectX
{
	struct XMMATRIX;
}

#include "RlUBase.h"


namespace At0::Util
{
	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::wstring)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	RLU_API std::wstring MultiByteToWideChar(const std::string_view str);

	/// <summary>
	/// Converts a wide character string (std::wstring) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	RLU_API std::string WideCharToMultiByte(const std::wstring_view str);

	/// <summary>
	/// Converts a transformation matrix to a windows rect structure
	/// </summary>
	/// <param name="matrix">Is the matrix to convert</param>
	/// <returns>The actual screen position of the transformed object</returns>
	RLU_API RECT TransformationToRect(const DirectX::XMMATRIX& matrix);
}

#endif // RLU_TYPECONVERT_H
