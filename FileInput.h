#include<string>
using namespace std;

//读取input文件夹中的每个子文件夹中的文件名，将所有cpp文件的路径记录下来，两两生成程序对存放到filelist.txt中，便于之后判断
class FileInput {
public:
	void fileInput();
	string ojInput(string stdin_path);//创建OJ输入
};