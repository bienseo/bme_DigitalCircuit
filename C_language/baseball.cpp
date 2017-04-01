#include<iostream>
#include<time.h>
using namespace std;

int main()
{
	int ans[3];
	int guess[3];
	int strike, ball, num=0;
	int i;
	int score;

	srand(time(NULL));
	
	while (true)
	{
		for (i = 0; i < 3; i++)
		{
			ans[i] = rand() % 10;
		}
		if (ans[0] != ans[1] && ans[0] != ans[2] && ans[1] != ans[2]) break;
	}

	cout << "숫자야구를 시작해봅시다."<<endl;

	while(true)
	{
		strike = ball = 0;
		cout << "\n 0에서 9사이의 숫자 3개를 생각해서 입력해주세요 \n"<<endl;
		
		for (i = 0; i < 3; i++)
		{
			cin >> guess[i];
		}
		
		if (ans[0] == guess[0])
		{
			strike++;
		}
		else if (ans[0] == guess[1] || ans[0] == guess[2])
		{
			ball++;
		}

		if (ans[1] == guess[1])
		{
			strike++;
		}
		else if (ans[1] == guess[0] || ans[1] == guess[2])
		{
			ball++;
		}

		if (ans[2] == guess[2])
		{
			strike++;
		}
		else if (ans[2] == guess[0] || ans[2] == guess[1])
		{
			ball++;
		}
		++num;
		cout << "\n";
		cout << strike << " strike " << ball << " ball " << num<<" 번\n"<<endl;

		score = (101 - num) * 50;

		if (strike == 3) {
			cout << "횟수: " << num <<" 점수: "<<score<<"입니다.\n"<<endl;
			break;
		}
	}

	return 0;
}
