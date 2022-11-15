//负责读取filelist.txt，对每一行（即每一个程序对）进行等价性判断
class JudgeTool {
public:
	void judgeAll();
};

//判断输入模块
class JudgeInput {
	//源文件的路径
	char* file1_path;
	char* file2_path;
	//随机输入文件的路径
	char* input_path;
	//友元
	friend class Judge;
	friend class JudgeOutput;
public:
	JudgeInput(char* file1_path, char* file2_path, char* input_path);
	~JudgeInput() { delete[]file1_path; delete[]file2_path; delete[]input_path; }
};

//判断模块
class Judge {
	bool isEqual;//判断结果
	friend class JudgeOutput;
public:
	Judge() { isEqual = true; }
	void judge(JudgeInput* judgeInput);//进行等价性判断
};

//输出模块
class JudgeOutput {
public:
	void res_output(JudgeInput* judgeInput, Judge* judge);//将结果存储到.csv文件中
};