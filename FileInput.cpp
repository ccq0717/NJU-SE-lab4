#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<dirent.h>
#include"FileInput.h"
using namespace std;

void FileInput::fileInput()
{
	FILE* f_list = fopen("input/filelist.txt", "w");//filelist.txt存放在input文件夹下
	if (f_list == NULL) {
		cerr << "filelist.txt打开失败";
		exit(-1);
	}
	dirent* ptr;
	DIR* dir;
	dir = opendir("input");//打开input文件夹（存放在本程序目录下）进行读取
	vector<string>dirs;//子文件夹的名字
	while ((ptr = readdir(dir)) != NULL)
	{
		if (ptr->d_name[0] == '.' || strstr(ptr->d_name, ".txt") != NULL) {//跳过.和..以及filelist.txt
			continue;
		}
		dirs.push_back(ptr->d_name);
	}
	//对每一个子文件夹，先读取其中后缀为.cpp的文件名，保存到数组中，最后再两两配对保存到filelist.txt中
	//然后读取stdin_format.txt，创建随机输入保存到同目录下的input.txt，并将该input.txt的路径存放到filelist.txt中
	//即filelist.txt的每行内容：input/a/1.cpp input/a/2.cpp input/a/input.txt
	for (int i = 0; i < dirs.size(); i++) {
		char path[100] = "input/";
		strcat(path, dirs[i].c_str());//获取子文件夹的路径
		dir = opendir(path);
		string stdin_path = path;
		stdin_path += "/stdin_format.txt";//标准输入文件的路径
		string input_path = ojInput(stdin_path);//创建OJ随机输入
		vector<string>files;//.cpp文件的名字
		while ((ptr = readdir(dir)) != NULL)
		{
			if (strstr(ptr->d_name, ".cpp") != NULL) {//查找.cpp文件
				files.push_back(ptr->d_name);
			}
		}
		//两两形成程序对
		for (int i = 0; i < files.size(); i++) {
			for (int j = i + 1; j < files.size(); j++) {
				string tmp = path;
				tmp += "/";
				string tmp1 = tmp + files[i];
				string tmp2 = tmp + files[j];
				fprintf(f_list, "%s %s %s\n", tmp1.c_str(), tmp2.c_str(), input_path.c_str());
			}
		}
	}
	fclose(f_list);
}

string FileInput::ojInput(string stdin_path)
{
	//将input.txt放到stdin_format.txt的同目录下，构造一组随机输入
	string temp_path = stdin_path;
	temp_path.replace(temp_path.find("stdin_format.txt"), strlen("stdin_format.txt"), "input.txt");
	string input_path = temp_path;
	//打开文件，解析提供的标准输入
	FILE* f_stdin = fopen(stdin_path.c_str(), "r");
	if (f_stdin == NULL) {
		cerr << "stdin_format.txt打开错误";
		exit(-1);
	}
	FILE* f_input = fopen(input_path.c_str(), "w");
	if (f_input == NULL) {
		cerr << "input.txt打开错误";
		exit(-1);
	}
	char temp[100] = { 0 };
	while (!feof(f_stdin)) {
		fscanf(f_stdin, "%s", temp);//因为不同类型的参数类型用空格分割，所以可以按顺序一个个读入
		string str_input = temp;
		if (str_input.find("int") != string::npos) {//是数字类型
			str_input = str_input.substr(4, str_input.length() - 5);//提取出括号内的参数，4是第一个参数的起始下标，减5是去掉int和两个括号
			int left, right;//左右边界
			int ind = str_input.find(",");//找到两个参数的分界点
			//将字符串转数字
			left = stoi(str_input.substr(0, ind));
			right = stoi(str_input.substr(ind + 1));
			int num = rand() % (right - left + 1) + left;//生成[left, right]中的随机数
			fprintf(f_input, "%d ", num);//注意加一个空格
		}
		else if (str_input.find("string") != string::npos) {//是字符串类型
			str_input = str_input.substr(7, str_input.length() - 8);//提取出括号内的参数
			int left, right;//左右边界
			int ind = str_input.find(",");//找到两个参数的分界点
			//将字符串转数字
			left = stoi(str_input.substr(0, ind));
			right = stoi(str_input.substr(ind + 1));
			int len = rand() % (right - left + 1) + left;//生成[left, right]中的随机数，作为该字符串的长度
			string str;
			for (int i = 0; i < len; i++) {
				if (rand() % 2) {//随机选择是大写字母还是小写
					char c = 'a' + rand() % 26;//随机选取字母
					str += c;
				}
				else {
					char c = 'A' + rand() % 26;//随机选取字母
					str += c;
				}
			}
			fprintf(f_input, "%s ", str.c_str());//注意加一个空格
		}
		else if (str_input.find("char") != string::npos) {//是字符类型
			char ch;
			if (rand() % 2) {//随机选择是大写字母还是小写
				ch = 'a' + rand() % 26;//随机选取字母
			}
			else {
				ch = 'A' + rand() % 26;//随机选取字母
			}
			fprintf(f_input, "%c ", ch);//注意加一个空格
		}
	}
	fclose(f_stdin);
	fclose(f_input);
	return input_path;
}