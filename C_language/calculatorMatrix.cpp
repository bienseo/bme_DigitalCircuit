/*
	Term Project: 2 x 2 행렬 계산기
*/
#include<iostream>
using namespace std;

class CesMatrix
{
public:
	double CesElement[2][2];
	int CesRow, CesColumn;

	void CesGetMatrix()
	{
		for (CesRow = 0; CesRow < 2; CesRow++){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				cin >> CesElement[CesRow][CesColumn];
			}
		}
	}

	void CesShowMatrix()
	{
		cout << endl;
		for (CesRow = 0; CesRow < 2; CesRow++){
			cout << "[  ";
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				cout << CesElement[CesRow][CesColumn] << "  ";
			}
			cout << "]" << endl;
		}
		cout << endl << endl;
	}

	CesMatrix operator +(CesMatrix CesMatrix2)
	{
		for (CesRow = 0; CesRow < 2; CesRow++){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				CesElement[CesRow][CesColumn] += CesMatrix2.CesElement[CesRow][CesColumn];
			}
		}
		return *this;
	}

	CesMatrix operator -(CesMatrix CesMatrix2)
	{
		for (CesRow = 0; CesRow < 2; CesRow++){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				CesElement[CesRow][CesColumn] -= CesMatrix2.CesElement[CesRow][CesColumn];
			}
		}
		return *this;
	}

	CesMatrix operator *(CesMatrix CesMatrix2)
	{
		CesMatrix temp;
		temp = *this;

		for (CesRow = 0; CesRow < 2; ++CesRow){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				CesElement[CesRow][CesColumn] = temp.CesElement[CesRow][0] * CesMatrix2.CesElement[0][CesColumn] + temp.CesElement[CesRow][1] * CesMatrix2.CesElement[1][CesColumn];
			}
		}
		return *this;
	}

	CesMatrix operator *=(CesMatrix CesMatrix2)
	{
		for (CesRow = 0; CesRow < 2; CesRow++){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				CesElement[CesRow][CesColumn] *= CesMatrix2.CesElement[CesRow][CesColumn];
			}
		}
		return *this;
	}

	CesMatrix operator /=(CesMatrix CesMatrix2)
	{
		for (CesRow = 0; CesRow < 2; CesRow++){
			for (CesColumn = 0; CesColumn < 2; CesColumn++){
				CesElement[CesRow][CesColumn]/= CesMatrix2.CesElement[CesRow][CesColumn];
			}
		}
		return *this;
	}

	CesMatrix operator ||(CesMatrix CesMatrix2)
	{
		CesMatrix temp;

		if (CesInverse(CesMatrix2) != 0){

			temp = *this;

			for (CesRow = 0; CesRow < 2; ++CesRow){
				for (CesColumn = 0; CesColumn < 2; CesColumn++){
					CesElement[CesRow][CesColumn] = temp.CesElement[CesRow][0] * CesMatrix2.CesElement[0][CesColumn] + temp.CesElement[CesRow][1] * CesMatrix2.CesElement[1][CesColumn];
				}
			}			
		}
		return *this;
	}

		int CesInverse(CesMatrix &CesMatrix2)
		{
			double a, b, c, d, Det;

			a = CesMatrix2.CesElement[0][0];
			b = CesMatrix2.CesElement[0][1];
			c = CesMatrix2.CesElement[1][0];
			d = CesMatrix2.CesElement[1][1];

			Det = a*d - b*c;

			if (Det == 0){

				cout << "Det=0: 역행렬이 존재하지 않습니다. " << endl;
				return false;

			}
			else {

				CesMatrix2.CesElement[0][0] = d / Det;
				CesMatrix2.CesElement[0][1] = -b / Det;
				CesMatrix2.CesElement[1][0] = -c / Det;
				CesMatrix2.CesElement[1][1] = a / Det;
				return true;

			}
		}
	};

	int main()
	{

		int cesCase = 0;
		CesMatrix CesMatrix1, CesMatrix2;
		
		cout << "**** 2 x 2 행렬 계산기 ****" << endl<<endl;
		cout << "첫 번째 행렬의 원소 4개를 차례로  입력하시오. (1,1) (1,2) (2,1) (2,2)" << endl;
		CesMatrix1.CesGetMatrix();
		CesMatrix1.CesShowMatrix();
		do{
			cout << "원하는 연산자를 선택하시오. (1: +  2: -  3: *  4: .* 5: ./ 6: Inverse)  ";
			cin >> cesCase;
			cout << endl<<endl;
			cout << "연산에 필요한 행렬의 원소 4개를 차례로  입력하시오. (1,1) (1,2) (2,1) (2,2)" << endl;
			CesMatrix2.CesGetMatrix();
			CesMatrix2.CesShowMatrix();
			
			switch (cesCase){
			case 1:
			{
				CesMatrix1 = CesMatrix1 + CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();
				break;
			}
			case 2:
			{
				CesMatrix1 = CesMatrix1 - CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();
				break;
			}
			case 3:
			{
				CesMatrix1 = CesMatrix1 *  CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();
				break;
			}
			case 4:
			{
				CesMatrix1 = CesMatrix1 *= CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();
				break;
			}
			case 5:
			{
				CesMatrix1 = CesMatrix1 /= CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();
				break;
			}
			case 6:
			{
				CesMatrix1 = CesMatrix1 || CesMatrix2;
				cout << "**** Answer ****" << endl;
				CesMatrix1.CesShowMatrix();

				break;
			}
		}
			cout << "계속 하시겠습니까? ( 종료: 0    계속: 아무키나 입력)   ";
			cin >> cesCase;

		} while (cesCase != 0);

		return 0;
	}