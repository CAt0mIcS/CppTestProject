#include "pch.h"


//struct DateTime
//{
//	int year;
//	int month;
//	int dayOfWeek;
//	int day;
//	int hour;
//	int minute;
//	int second;
//	int milisecond;
//
//	friend std::ostream& operator<<(std::ostream& os, const DateTime& dt);
//};

//DateTime g_Dt;

struct SystemTimes
{
	SYSTEMTIME creationTime, lastAccessTime, lastWriteTime;
};

SystemTimes GetWindowsCreationDate(const std::string& path)
{

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (!hFile || hFile == INVALID_HANDLE_VALUE)
		return { 0 };

	FILETIME creationDate;
	FILETIME lastAccessDate;
	FILETIME lastWriteDate;

	BOOL success = GetFileTime(hFile, &creationDate, &lastAccessDate, &lastWriteDate);

	if (!success)
		return { 0 };

	SYSTEMTIME creationTime;
	SYSTEMTIME lastAccessTime;
	SYSTEMTIME lastWriteTime;
	FileTimeToSystemTime(&creationDate, &creationTime);
	FileTimeToSystemTime(&creationDate, &lastAccessTime);
	FileTimeToSystemTime(&creationDate, &lastWriteTime);

	CloseHandle(hFile);
	return { creationTime, lastAccessTime, lastWriteTime };
}

BOOL SetWindowsCreationDate(const SystemTimes& dt, const std::string& path)
{
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (!hFile || hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	FILETIME cDate;
	FILETIME lADate;
	FILETIME lWDate;
	SystemTimeToFileTime(&dt.creationTime, &cDate);
	SystemTimeToFileTime(&dt.lastAccessTime, &lADate);
	SystemTimeToFileTime(&dt.lastWriteTime, &lWDate);

	BOOL success = SetFileTime(hFile, &cDate, &lADate , &lWDate);
	if (!success)
	{
		CHAR buffer[256];
		std::cout << FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buffer, sizeof(buffer), 0) << '\n';
	}

	CloseHandle(hFile);
	return success;
}

inline bool FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

std::vector<std::string> GetBaseFileDirs()
{
	std::vector<std::string> dirs{};
	dirs.reserve(100);
	std::ifstream reader("D:\\dev\\Python\\Projects\\SortPicturesByDate\\output.txt");

	std::string line;
	while (std::getline(reader, line))
	{
		dirs.emplace_back(std::move(line));
	}

	return dirs;
}

std::string GetFileName(const std::string& path)
{
	std::string filename = path;
	unsigned short lastId = path.find_last_of("\\");

	filename.replace(0, lastId + 1, "");

	return filename;
}

bool CopyFiles(unsigned int minYear, unsigned int maxYear, const SystemTimes& dates, const std::string& path)
{
	if (dates.creationTime.wYear > maxYear || dates.creationTime.wYear < minYear)
		return false;

	const char* months[] = { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
	std::stringstream ss;

	std::stringstream ss2;
	ss2 << "F:\\Test\\" << dates.creationTime.wYear << "\\" << months[dates.creationTime.wMonth - 1];

	ss << "copy " << "\"" << path << "\" " << ss2.str();
	bool success = !(bool)system(ss.str().c_str());

	std::string filename = GetFileName(path);

	if(success)
		std::cout << SetWindowsCreationDate(dates, ss2.str() + "\\" + filename) << '\n';

	return success;
}

void MoveTemp()
{
	int runs = 0;
	for (auto& dir : GetBaseFileDirs())
	{
		SystemTimes dates = GetWindowsCreationDate(dir);

		//TODO: Copy files to ordered directory using the creationDate
		if (!CopyFiles(2016, 2020, dates, dir))
		{
			std::cout << "Failed to copy file " << dir << '\n';
		}
		++runs;
	}
	std::cout << "Program ran through " << runs << " file(s)\n";
	//Ran 521 times
}


int main()
{
	MoveTemp();
}
