/*
	COMMON

	DEFAULT CONFIGURATION FILE
	Rev 27/02/2019
*/
#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <string>
#include <list>

struct JobInfo
{
	std::string type;
	std::string path;
	int dim_x;
	int dim_y;
	int interval;
	int id;
	int window_size_msec;
};

class CDefs
{
public:

	inline CDefs(const char * path)
	{
		using namespace rapidjson;
		std::ifstream file(path);
		if (file.is_open())
		{
			IStreamWrapper isw(file);
			Document document;
			document.ParseStream(isw);

			if (!document.HasParseError() && document.IsObject() && document.HasMember("inputs") && document["inputs"].IsArray())
			{
				const Value& a = document["inputs"];
				assert(a.IsArray());
				for (SizeType i = 0; i < a.Size(); i++) // Uses SizeType instead of size_t
				{
					JobInfo info;
					info.type = a[i]["type"].GetString();
					info.path = a[i]["path"].GetString();
					info.dim_x = a[i]["x"].GetInt();
					info.dim_y = a[i]["y"].GetInt();
					info.interval = a[i]["interval"].GetInt();

					if (a[i].HasMember("window_size_msec"))
						info.window_size_msec = a[i]["window_size_msec"].GetInt();
					else
						info.window_size_msec = 0;

					if (a[i].HasMember("id"))
						info.id = a[i]["id"].GetInt();
					else
						info.id = 1;

					jobs.push_back(info);
				}

				output_path = document["output_path"].GetString();
			}
		}
	}

	~CDefs() {};

	std::string output_path;

	std::list<JobInfo> jobs;
};

