/*
 숫자야구과제
 */
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
    
    
    while(true)
    {
        strike = ball = 0;
        cout << "숫자야구 게임!\n제가 생각하고 있는 세 자리수를 맞춰보세용~ 최고점은 5000점 입니다 ^.^* \n(*한자리씩 입력해주세요.) \n"<<endl;
        
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
        cout << strike << " strike " << ball << " ball " << num<<" 회 \n"<<endl;
        
        score = (101 - num) * 50;
        
        if (strike == 3) {
            cout << "정답입니다! " << num <<" 회 만에 맞추신 당신의 점수는 "<<score<<" 입니다."<<endl;
            break;
        }
    }
    
    return 0;
}
