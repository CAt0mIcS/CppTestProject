#include "Logger.h"


void QRD::Logger::Init(const std::string& filepath)
{
	std::ofstream ofs;
	ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

