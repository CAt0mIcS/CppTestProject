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

void WriteLog(const std::string& msg)
{
	std::ofstream writer("D:\\dev\\Cpp\\Projects\\TestProject\\log.txt", std::ios::app);
	writer << msg;
	writer.close();
}

std::string GetFileName(const std::string& path)
{
	std::string filename = path;
	size_t lastId = path.find_last_of("\\");

	filename.replace(0, lastId + 1, "");

	return filename;
}

bool CopyFiles(unsigned int minYear, unsigned int maxYear, const SystemTimes& dates, const std::string& path)
{
	if (dates.creationTime.wYear > maxYear || dates.creationTime.wYear < minYear)
	{
		std::stringstream ssLog;
		ssLog << "Skipping file " << path << " due to invalid date: \t\t" << dates.creationTime.wYear << '-' << dates.creationTime.wMonth << '\n';
		std::cout << ssLog.str();
		WriteLog(ssLog.str());
		return false;
	}

	const char* months[] = { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
	std::stringstream ss;

	std::stringstream ss2;
	ss2 << "F:\\Test\\" << dates.creationTime.wYear << "\\" << months[dates.creationTime.wMonth - 1];

	ss << "copy " << "\"" << path << "\" " << ss2.str();
	bool success = !(bool)system(ss.str().c_str());

	std::string filename = GetFileName(path);

	if (success)
	{
		SetWindowsCreationDate(dates, ss2.str() + "\\" + filename);
		
		std::stringstream ssLog;
		ssLog << "Copying file with date: " << dates.creationTime.wYear << '-' << dates.creationTime.wMonth << " to " << ss2.str() << '\n';
		std::cout << ssLog.str();
		WriteLog(ssLog.str());

		std::stringstream ssRem;
		ssRem << "del F:\\Temp\\" << filename;
		
		bool successDel = !(bool)system(ssRem.str().c_str());

		if (successDel)
		{
			std::stringstream ssRemLog;
			ssRemLog << "Removed file " << filename << " from F:\\Temp\\\n";
			std::cout << ssRemLog.str();
			WriteLog(ssRemLog.str());
		}
		else
		{
			std::stringstream ssRemLog;
			ssRemLog << "Failed to remove file " << filename << " from F:\\Temp\\\n";
			std::cout << ssRemLog.str();
			WriteLog(ssRemLog.str());
			return false;
		}

	}
	else
	{
		std::stringstream ssLog;
		ssLog << "Failed to copy file with date: " << dates.creationTime.wYear << '-' << dates.creationTime.wMonth << " to " << ss2.str() << '\n';
		std::cout << ssLog.str();
		WriteLog(ssLog.str());
	}

	return success;
}

void MoveTemp()
{

	//Clear log
	std::ofstream writer("D:\\dev\\Cpp\\Projects\\TestProject\\log.txt");
	writer.close();

	int runs = 0;
	int succRuns = 0;
	for (auto& dir : GetBaseFileDirs())
	{
		SystemTimes dates = GetWindowsCreationDate(dir);

		//TODO: Copy files to ordered directory using the creationDate
		if (!CopyFiles(2016, 2020, dates, dir))
		{
			++runs;
			continue;
		}
		++runs;
		++succRuns;
	}
	std::cout << "Program ran through " << runs << " file(s)\n";
	WriteLog(std::string("Program ran through ") + std::to_string(runs) + "file(s)\n");
	//Ran 521 times
	std::cout << "Program coppied " << succRuns << " file(s)\n";
	WriteLog(std::string("Program coppied ") + std::to_string(succRuns) + " file(s)\n");
	//Coppied 332 files
}


int main()
{
	MoveTemp();
}
