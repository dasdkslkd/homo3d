// homo3d.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "cmdline.h"
#include <fstream>
#include <sstream>
#include <string>

extern void cuda_test(void);
extern void testAutoDiff(void);
extern void testAutoDiff_cu(void);
extern void test_MMA(void);
extern void test_OC(void);
extern void testHomogenization(cfg::HomoConfig config);
extern void test_BulkModulus(void);
extern void test_ShearModulus(void);
extern void test_NegativePoisson(void);
extern void runInstance(cfg::HomoConfig);

namespace homo {
	extern std::string setPathPrefix(const std::string& fprefix);
}

int main(int argc, char** argv)
{
	cfg::HomoConfig config;
	config.parse(argc, argv);

    std::cout << "Hello World!\n";
	cuda_test();
	//testAutoDiff();
	//testAutoDiff_cu();
	//test_MMA();
	//test_OC();
	//test_BulkModulus();
	//test_ShearModulus();
	//test_NegativePoisson();
	homo::setPathPrefix(config.outprefix);
	try {
		if (config.obj == cfg::Objective::custom)
		{
#if 0
			std::ifstream inFile("C:\\Users\\lzx\\Desktop\\tpo\\ai\\code\\spinodal\\test.csv", std::ios::in);
			std::string lineStr;
			while (getline(inFile, lineStr))
			{
				std::stringstream ss(lineStr);
				std::string str;
				//std::cout << lineStr<<std::endl;
				while (getline(ss, str, ','))
				{
					config.clipboard.push_back(stoi(str));
				}
			}
#endif
			std::ifstream file(config.infile, std::ios::binary);
			if (!file) {
				std::cerr << "can't open file" << std::endl;
				return 1;
			}
			unsigned char byte;
			while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
				config.clipboard.emplace_back((byte) & 1);
				config.clipboard.emplace_back((byte >> 1) & 1);
				config.clipboard.emplace_back((byte >> 2) & 1);
				config.clipboard.emplace_back((byte >> 3) & 1);
				config.clipboard.emplace_back((byte >> 4) & 1);
				config.clipboard.emplace_back((byte >> 5) & 1);
				config.clipboard.emplace_back((byte >> 6) & 1);
				config.clipboard.emplace_back((byte >> 7) & 1);
			}
			file.close();
		}
		//std::cout << config.clipboard.size();
		//system("pause");
		testHomogenization(config);
		runInstance(config);
	}
	catch (std::runtime_error e) {
		std::cout << "\033[31m" << "Exception occurred: " << std::endl << e.what() << std::endl << ", aborting..." << "\033[0m" << std::endl;
		exit(-1);
	} catch (...) {
		std::cout << "\033[31m" << "Unhandled Exception occurred, aborting..." << "\033[0m" << std::endl;
		exit(-1);
	}
}


