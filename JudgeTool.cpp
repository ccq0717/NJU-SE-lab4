#include<cstring>
#include<string>
#include<iostream>
#include"FileInput.h"
#include"JudgeTool.h"
using namespace std;

JudgeInput::JudgeInput(char* file1_path, char* file2_path, char* input_path)
{
	this->file1_path = new char[strlen(file1_path) + 1];
	strcpy(this->file1_path, file1_path);
	this->file2_path = new char[strlen(file2_path) + 1];
	strcpy(this->file2_path, file2_path);
	this->input_path = new char[strlen(input_path) + 1];
	strcpy(this->input_path, input_path);
}

void Judge::judge(JudgeInput* judgeInput)
{
	//运行第一个OJ程序
	string cmd = "g++ -w ";
	cmd += judgeInput->file1_path;
	system(cmd.c_str());//在子文件夹中编译，可执行文件生成在本程序的目录下
	//指定输入输出，output.txt也生成在本程序的目录下
	cmd = "./a.out <";
	cmd += judgeInput->input_path;
	cmd += " >output1.txt 2>&1";//错误信息，如clog也输出到文件中
	system(cmd.c_str());
	//运行第二个OJ程序
	cmd = "g++ -w ";
	cmd += judgeInput->file2_path;
	system(cmd.c_str());
	//指定输入输出
	cmd = "./a.out < ";
	cmd += judgeInput->input_path;
	cmd += " > output2.txt 2>&1";
	system(cmd.c_str());
	//比对输出文本，得到等价性判断结果
	FILE* f_output1 = fopen("output1.txt", "r");
	if (f_output1 == NULL) {
		cerr << "output1.txt打开错误";
		exit(-1);
	}
	FILE* f_output2 = fopen("output2.txt", "r");
	if (f_output2 == NULL) {
		cerr << "output2.txt打开错误";
		exit(-1);
	}
	//注意isEqual在构造函数中默认设置为true
	while (!feof(f_output1) && !feof(f_output2)) {
		//逐个比对
		char temp1[100] = { 0 }, temp2[100] = { 0 };
		fscanf(f_output1, "%s", temp1);
		fscanf(f_output2, "%s", temp2);
		if (strcmp(temp1, temp2) != 0) {
			//不相等
			isEqual = false;
			break;
		}
	}
	if (!feof(f_output1) || !feof(f_output2)) {
		//其中一个文件没读完，一定是输出不同（提前退出循环或输出个数不同）
		isEqual = false;
	}
	fclose(f_output1);
	fclose(f_output2);
}

void JudgeOutput::res_output(JudgeInput* judgeInput, Judge* judge)
{
	//equal.csv和inequal.csv存放在本程序目录中的output文件夹下
	if (judge->isEqual) {
		//等价
		FILE* f_equal = fopen("./output/equal.csv", "a");//建立和追加
		if (f_equal == NULL) {
			cerr << "equal.csv打开错误";
			exit(-1);
		}
		fprintf(f_equal, "%s,%s\n", judgeInput->file1_path, judgeInput->file2_path);
		fclose(f_equal);
	}
	else {
		//不等价
		FILE* f_inequal = fopen("./output/inequal.csv", "a");//建立和追加
		if (f_inequal == NULL) {
			cerr << "inequal.csv打开错误";
			exit(-1);
		}
		fprintf(f_inequal, "%s,%s\n", judgeInput->file1_path, judgeInput->file2_path);
		fclose(f_inequal);
	}
}

void JudgeTool::judgeAll()
{
	//根据filelist.txt进行等价判断
	FILE* f_list = fopen("input/filelist.txt", "r");
	if (f_list == NULL) {
		cerr << "filelist.txt打开失败";
		exit(-1);
	}
	char file1_path[100] = { 0 }, file2_path[100] = { 0 }, input_path[100] = { 0 };
	while (!feof(f_list)) {
		fscanf(f_list, "%s %s %s\n", file1_path, file2_path, input_path);
		//获取输入
		JudgeInput* judgeInput = new JudgeInput(file1_path, file2_path, input_path);
		//等价判断
		Judge* _judge = new Judge;
		_judge->judge(judgeInput);
		//输出
		JudgeOutput* judgeOutput = new JudgeOutput;
		judgeOutput->res_output(judgeInput, _judge);
		delete judgeInput;
		delete _judge;
		delete judgeOutput;
	}
	fclose(f_list);
}