#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	string strInput;
	cout << "#> ";
	getline(cin,strInput);

	cout << "Input:" << strInput << endl;
	size_t end = strInput.find(';');

	if( end == string::npos )
		cout << "query terminator missing. Use ; at the end\n";
	else
	{
		size_t param_sel = strInput.find("select");
		if(param_sel != string::npos)
		{
			string selStr;
			param_sel += strlen("select");

			size_t param_where = strInput.find("where");
			if(param_where != string::npos)
			{
				selStr = strInput.substr(param_sel,param_where-1);
			}
			else
			{
				selStr = strInput.substr(param_sel,end-1);
			}

			//trim whitespaces
			size_t trail = selStr.find_last_not_of(";");
			if(trail != string::npos)
				selStr.erase(trail+1);
			
			size_t lead = selStr.find_first_not_of(" ");
			if(lead != string::npos)
				selStr.erase(lead+1);

			if( !selStr.empty() )
				cout << "Param string:" << selStr <<"\n";
			else
				cout << "Empty params...\n";
		}
	}

	return 0;
}
