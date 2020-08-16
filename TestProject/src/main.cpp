#include "pch.h"


struct DateTime
{
	int year;
	int month;

	friend std::ostream& operator<<(std::ostream& os, const DateTime& dt);
};

DateTime g_Dt;


DateTime GetPNGCreationDate(const std::string& path)
{
	std::ifstream reader(path, std::ios::binary);

	std::string line;
	while (std::getline(reader, line))
	{
		if (line.find("</photoshop:DateCreated>") != std::string::npos && line.find("<photoshop:DateCreated>") != std::string::npos)
		{
			break;
		}
	}
	line.replace(0, 32, "").replace(10, line.size() - 1, "");
	std::string year = "";
	std::string month = "";

	for (int i = 0; i < 4; ++i)
		year += line[i];

	for (int i = 5; i < 7; ++i)
		month += line[i];

	return { std::stoi(year), std::stoi(month) };
}


DateTime GetJPGCreationDate(const std::string& path)
{
	std::ifstream reader(path, std::ios::binary);

	DateTime dt{ 0 };

	std::string line;
	while (std::getline(reader, line))
	{
		if (dt.year && dt.month)
			break;

		for (unsigned int i = 0; i < line.size(); ++i)
		{
			if (line[i] == '1' || line[i] == '2' || line[i] == '3' || line[i] == '4' || line[i] == '5' || line[i] == '6' || line[i] == '7' || line[i] == '8' || line[i] == '9' || line[i] == '0')
			{
				std::string str0 = "";
				std::string str1 = "";
				std::string str2 = "";
				std::string str3 = "";

				str0 += line[i];
				try
				{
					str1 += line.at(i + 1);
					str2 += line.at(i + 2);
					str3 += line.at(i + 3);
				}
				catch (std::out_of_range)
				{
					return dt;
				}
				
				try
				{
					std::stoi(str1);
					std::stoi(str2);
					std::stoi(str3);

					dt.year = std::stoi(str0 + str1 + str2 + str3);

				}
				catch (std::invalid_argument)
				{
					continue;
				}

				std::string str4 = "";
				std::string str5 = "";

				try
				{
					str4 += line[i + 5];
					str5 += line[i + 6];

					dt.month = std::stoi(str4 + str5);
				}
				catch (...)
				{
					return dt;
				}

				break;

			}
		}

	}
	return dt;
}

DateTime GetWindowsCreationDate(const std::string& path)
{

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (!hFile)
		return { 0 };

	FILETIME creationDate;
	FILETIME lastAccessDate;
	FILETIME lastWriteDate;

	BOOL success = GetFileTime(hFile, &creationDate, &lastAccessDate, &lastWriteDate);

	if (!success)
		return { 0 };

	SYSTEMTIME time;
	FileTimeToSystemTime(&creationDate, &time);
	return { time.wYear, time.wMonth };
}

inline bool FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}


void CreateDirectoriesAndMove(unsigned int minYear, unsigned int maxYear, const std::string& searchPath, const std::string& createDir)
{
	std::array<std::string, 12> months{ "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

	for (int i = minYear; i <= maxYear; ++i)
	{
		std::stringstream ss2;
		ss2 << createDir << i;
		std::filesystem::create_directory(ss2.str());
	
		for (int j = 0; j < 12; ++j)
		{
			std::stringstream ss;
			ss << createDir << i << '\\' << months[j];
			std::filesystem::create_directory(ss.str());
		}
		
	}


	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	int fileAmnt = 0;
	for (const auto& dirEntry : recursive_directory_iterator(searchPath))
	{
		try
		{
			if (dirEntry.path().string().find(".jpg") != std::string::npos || dirEntry.path().string().find(".JPG") != std::string::npos && dirEntry.path().string().find(searchPath) == std::string::npos)
			{
				if (dirEntry.path().string().find("A - Schulfest 2016") != std::string::npos)
					__debugbreak();

				g_Dt = GetJPGCreationDate(dirEntry.path().string());
				if (g_Dt.year == 0 || g_Dt.month == 0 || g_Dt.month > 12 || g_Dt.year > 2030 || g_Dt.month < 1 || g_Dt.year < 1900)
					continue;
				++fileAmnt;

			}
			else if (dirEntry.path().string().find(".PNG") != std::string::npos || dirEntry.path().string().find(".png") != std::string::npos && dirEntry.path().string().find(searchPath) == std::string::npos)
			{
				g_Dt = GetPNGCreationDate(dirEntry.path().string());

				if (g_Dt.year == 0 || g_Dt.month == 0 || g_Dt.month > 12 || g_Dt.year > 2030 || g_Dt.month < 1 || g_Dt.year < 1900)
					continue;
				++fileAmnt;
			}

		}
		catch (std::system_error)
		{
			continue;
		}
	}

	int filesCoppied = 0;
	for (const auto& dirEntry : recursive_directory_iterator(searchPath))
	{
		//std::cout << dirEntry.path() << std::endl;
	
		try
		{
			if (dirEntry.path().string().find(".jpg") != std::string::npos || dirEntry.path().string().find(".JPG") != std::string::npos && dirEntry.path().string().find(searchPath) == std::string::npos)
			{
				g_Dt = GetJPGCreationDate(dirEntry.path().string());
				if (g_Dt.year == 0 || g_Dt.month == 0 || g_Dt.month > 12 || g_Dt.year > 2030 || g_Dt.month < 1 || g_Dt.year < 1900)
					continue;
			}
			else if (dirEntry.path().string().find(".PNG") != std::string::npos || dirEntry.path().string().find(".png") != std::string::npos && dirEntry.path().string().find(searchPath) == std::string::npos)
			{
				g_Dt = GetPNGCreationDate(dirEntry.path().string());
	
				if (g_Dt.year == 0 || g_Dt.month == 0 || g_Dt.month > 12 || g_Dt.year > 2030 || g_Dt.month < 1 || g_Dt.year < 1900)
					continue;
	
			}
			else
				continue;
	
			if (g_Dt.year <= minYear && g_Dt.year >= maxYear || dirEntry.path().string().find(".Trashes") != std::string::npos)
				continue;
	
			for (int i = minYear; i <= maxYear; ++i)
			{
				if (i == g_Dt.year)
				{
					for (int j = 0; j < 12; ++j)
					{
						if (j + 1 == g_Dt.month)
						{
							std::stringstream ss;
	
							std::stringstream ss2;
							ss2 << createDir << i << '\\' << months[j] << '\\' << dirEntry.path().filename();
							if (FileExists(ss2.str()))
							{
								auto filePath = dirEntry.path().filename().string();
								std::stringstream ss3;
	
								ss3 << "copy " << dirEntry.path() << ' ' << searchPath << "Temp\\";
								system(ss3.str().c_str());
	
								std::stringstream ss4;
								std::stringstream ss5;
								std::stringstream ss6;
	
	
								if (filePath.find(".jpg") != std::string::npos)
								{
									ss4 << "rename " << searchPath << "Temp\\" << dirEntry.path().filename().string() << ' ' << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".jpg";
									ss5 << "copy " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".jpg" << ' ' << createDir << i << '\\' << months[j] << '\\';
									ss6 << "del " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".jpg";
	
								}
								else if (filePath.find(".PNG") != std::string::npos)
								{
									ss4 << "rename " << searchPath << "Temp\\" << dirEntry.path().filename().string() << ' ' << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".PNG";
									ss5 << "copy " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".PNG" << ' ' << createDir << i << '\\' << months[j] << '\\';
									ss6 << "del " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".PNG";
	
								}
								else
								{
									ss4 << "rename " << searchPath << "Temp\\" << dirEntry.path().filename().string() << ' ' << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".png";
									ss5 << "copy " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".png" << ' ' << createDir << i << '\\' << months[j] << '\\';
									ss6 << "del " << searchPath << "Temp\\" << (dirEntry.path().filename().string().replace(filePath.size() - 4, filePath.size() - 1, "") + "_DUP" + std::to_string(i) + std::to_string(j)) << ".png";
	
								}
	
								system(ss4.str().c_str());
	
								system(ss5.str().c_str());
	
								system(ss6.str().c_str());
							}
							else
							{
								ss << "copy " << dirEntry.path() << ' ' << createDir << i << '\\' << months[j] << '\\';
								system(ss.str().c_str());
								std::cout << "Coppied file " << dirEntry.path() << " to " << createDir << i << '\\' << months[j] << '\n';
							}
							
							++filesCoppied;
							std::cout << filesCoppied * 100 / fileAmnt << "%\n";
	
						}
					}
				}
			}
		}
		catch (std::system_error)
		{
			continue;
		}

	}

}


int main()
{
	//Trailing backslash is mandatory
	CreateDirectoriesAndMove(2016, 2020, "F:\\", "F:\\Test\\");

}

std::ostream& operator<<(std::ostream& os, const DateTime& dt)
{
	std::cout << dt.month << '-' << dt.year;
	return os;
}
