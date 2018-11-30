#include <iostream>
using namespace std;

int main ()
{
	for (int i = 13; i < 65; i++)
	{
		if (i == 30 || i == 34 || i == 38 || i == 48)
			continue;

		cout << "echo No." << i << " >> out.txt" << endl;
		cout << "./main --verbose --median --midpoint --transfers < " << i << ".in > " << i << ".txt"<<endl;
		cout << "diff " << i << ".txt " << i << ".out >> out.txt" << endl;
	}
}
