/*
	COMMON

	WORKLOAD FILE- READ/GENERATE
	Rev 27/02/2019
*/
#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;

class CWorkload
{
public:

	static const char * binfilename;
	static const char * csvfilename;

	CWorkload();

	~CWorkload();

	static void LoadCSVWorkLoad(std::stringstream & csvbuf, string filepath)
	{
		csvbuf.clear();

		std::ifstream file(filepath.c_str());

		if (file)
		{
			csvbuf << file.rdbuf();

			file.close();
		}
	}

	static void LoadBINWorkLoad(char *& binbuf, size_t & size, string filepath)
	{
		FILE * f;
		f = fopen(filepath.c_str(), "rb");
		if (f)
		{
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			rewind(f);
			binbuf = new char[size+1];
			fread(binbuf, size, 1, f);
			fclose(f);
		}
	}

	static void CreateCSVWorkLoad(const char * filename, int cols, int lines, long long maxval = INT16_MAX)
	{
		std::stringstream file;
		for (int j = 0; j < cols; j++)
		{
			file << "HEADER_" << (j + 1) << ';';
		}
		file << std::endl;

		for (int i = 0; i < lines; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				file << ((rand() % maxval) + 42) << ';';
			}
			file << std::endl;
		}

		for (int j = 0; j < cols; j++)
		{
			file << "<end>" << ';';
		}
		file << std::endl;

		std::ofstream outFile;
		outFile.open(filename, std::ofstream::binary | std::ofstream::trunc);
		outFile.write(file.str().c_str(),file.str().length());
	}


	static void CreateBINWorkLoad(const char * filename, int ids, int samplesperid, long long maxval = 12345678987654321)
	{
		char * buffer = nullptr;
		int step = sizeof(__int64);

		size_t t = (ids * (samplesperid+1) * step);
		buffer = new char[t];
		char * p = buffer;
		
		int64_t startid = 1111;
		
		for (int i=0; i < ids; i++)
		{
			int64_t startval = 241;
			memcpy(p,&startid, step);
			p += step;
			for (int n = 0; n < samplesperid; n++)
			{
				double tmp = rand() % maxval;
				memcpy(p, &tmp, step);
				p += step;
			}
			startid++;
		}
		assert((int)p <= ((int)buffer+t));

		std::ofstream outFile;

		outFile.open(filename, std::ofstream::binary | std::ofstream::trunc);

		outFile.write(buffer,t);

		delete[] buffer;
	}
};

