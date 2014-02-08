#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "venus_net/venus_net.h"
#include "config_manager.h"
#include "json/json.h"
#include <fstream>

class Configuration : Json::Reader
{
public:
	void initialize(ConfigManager* manager)
	{
		manager->registerConfig(this);
	}

	bool parse(const std::string& filename, Json::Value& value)
	{
		/*std::ios::sync_with_stdio(false);
		std::ifstream fs;
		fs.open(filename, std::ios::in | std::ios::binary);
		if (fs.is_open())
		{
			char bom[3] = {0};
		}

		FILE *fp = fopen(filename.c_str(), "rb");
		if (fp == 0)
		{
			error_log("failed to open configuration : %s", filename.c_str());
			return false;
		}

		char buffTest[3];
		memset(buffTest, 0, 3);
		fread(buffTest, 1, 3, fp);

		if((buffTest[0] == (char)0xEF) || (buffTest[1] == (char)0xBB) || buffTest[2] == (char)0xBF)
		{
			return syncLoadUTF8(file, value, fp);
		}

		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::string config_doc = "";
		char *buffer = new char[size + 1];
		buffer[size] = 0;

		if (fread(buffer, 1, size, fp) == (unsigned long) size)
		{
			config_doc = buffer;
		}

		fclose(fp);
		delete[] buffer;

		if (this->parse(config_doc, value) == false)
		{
			return false;
		}

		//jsonValue.convert();

		//onLoadComplete(url, jsonValue);*/

	}

	virtual bool loadConfig() = 0;

private:
	bool isUTF8(char* s, size_t size)
	{
		uint8 x = 0, i = 0, j = 0, nbytes = 0, n = 0;

		for(i = 1; i < 7; i++)
		{
			x = (uint8)(255 << i);
			if(((uint8)*s & x) == x)
			{
				n = nbytes = (8 - i);
				for(j = 0; (j < nbytes && j < size); j++)
				{
					if((uint8)s[j] <= 0x80 && (uint8)s[j] >= 0xc0)
						break;

					else n--;
				}

				if(n == 0) return nbytes;
			}
		}

		return 0;
	}

	bool loadUTF8(const std::ifstream& fs, Json::Value& value)
	{
	
	}
};

#endif