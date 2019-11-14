#define  MAX_INDEX 34
#define  MAX_HANDCOUNT 14
#include <stdio.h>
#include <vector>

//������µ����Ƿ������˳���ߺ�
bool CheckCanContinuity(char const CardIndex[MAX_INDEX],  char GhostCount)
{
	char CardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(CardIndexTemp, CardIndex, sizeof(CardIndexTemp));

	char GhostCardNeeded = 0;
	for (char i = 0; i < MAX_INDEX; ++i)
	{
		if (0 == CardIndexTemp[i])
			continue;
		if (CardIndexTemp[i] < 0)
		{
			GhostCardNeeded -= CardIndexTemp[i];  //��Ϊ����Ϊ�� �����ü�
			CardIndexTemp[i] = 0;
			continue;
		}
		if ((i >= 0 && i <= 6) || (i >= 9 && i <= 15) || (i >= 18 && i <= 24))  //����ж���������ֵ�ڣ�1-7��֮��(�������[i,i+1,i+2]��˳��)
		{
			switch (CardIndexTemp[i])
			{
				case 1:
				case 4:
				{
					//���˳
					--CardIndexTemp[i + 1];
					--CardIndexTemp[i + 2];
					break;
				}
				case 2:
				{
					if ((CardIndexTemp[i + 1] < 2 && CardIndexTemp[i + 2] < 2)
						|| (3 == CardIndexTemp[i + 1] && 0 == CardIndexTemp[i + 2])
						|| (0 == CardIndexTemp[i + 1] && 3 == CardIndexTemp[i + 2]))
					{
						//��ɺ�
						++GhostCardNeeded;
					}
					else
					{
						//���˳
						CardIndexTemp[i + 1] -= 2;
						CardIndexTemp[i + 2] -= 2;
					}
					break;
				}
				case 3:
				{
					break;
				}
			}
		}
		else if (7 == i || 8 == i || 16 == i || 17 == i || 25 == i || 26 == i)//����ж���������ֵ�ڣ�8-9��֮��(���������[i,i+1,i+2]��˳�ӣ�������ɺ����iΪ8�������[i-1,i,i+1])
		{
			switch (CardIndexTemp[i])
			{
				case 1:
				case 4:
				{
					if ((7 == i || 16 == i || 25 == i) && CardIndexTemp[i + 1] > 0)
					{
						--CardIndexTemp[i + 1];
						++GhostCardNeeded;
					}
					else
					{
						//���i+1Ϊ0 ��ֱ����ɺ�
						GhostCardNeeded += 2;
					}
					break;
				}
				case 2:
				{
					++GhostCardNeeded; 
					break;
				}
				case 3:
				{
					break;
				}
			}
		}
		else
		{
			//���ƺͷ��Ʋ��ܳ�˳ ֻ����ɺ�
			if (CardIndexTemp[i] < 3)
			{
				GhostCardNeeded += (3 - CardIndexTemp[i]);
			}
			if (CardIndexTemp[i] > 3)
			{
				return false;
			}
		}
		CardIndexTemp[i] = 0;
	}

	//�����Ҫ����������ʵ�ʹ��Ƶ�����
	if (GhostCardNeeded > GhostCount)
	{
		return false;
	}
	else
	{
		return true;
	}

}


bool CheckCanHu(char const HandCard[MAX_INDEX], size_t HandCardSize,size_t GhostCount)
{
	char TotalCount = HandCardSize + GhostCount;
	if ((TotalCount - 2) % 3 != 0 || TotalCount > MAX_HANDCOUNT)
	{
		//������������3n+2�������14��
		return false;
	}
	//����������ֻ��������ʱ
	if (TotalCount == GhostCount)
	{
		return true;
	}
	char CardIndexTemp[MAX_INDEX];
	memcpy(CardIndexTemp, HandCard, sizeof(CardIndexTemp));
	std::vector<char> PairsInfo;  //���ڴ�������ܹ��ɺ��ƵĶ��ӵ�ֵ
	char CheckZero = ((GhostCount < 2) ? 2 : 0);//(0-δ��� 1-�ɹ� 2-ʧ�� )
	//1.�������е�ÿһ�ŵ������ӣ����ƣ���Ȼ�������µ����Ƿ��ܴﵽ��������(���˳�ӻ��ߺ�)
	for (char i = 0; i < MAX_INDEX; ++i)
	{
		if (0 == CardIndexTemp[i])
		{
			//���Լ��һ�ι��Ʊ��������û�е���
			if (CheckZero == 0)  
			{
				//CheckZero��Ϊ0˵�������� ���ټ��
				GhostCount -= 2;
				if (CheckCanContinuity(CardIndexTemp, GhostCount))
				{
					CheckZero = 1;
					//PairsInfo.clear();
					//PairsInfo.push_back(-1);
					//����任������û�е��ƶ��ܺ��� ��˵�����Ժ������ƣ������ټ������
					return true;
				}
				else
				{
					CheckZero = 2;
				}
				//���ʧ�ܣ���ԭ�ƣ�������һ�ּ��
				GhostCount += 2;
			}
			continue;
		}
		if (CardIndexTemp[i] > 4)
		{
			//��������4��
			return false;
		}
		if (CardIndexTemp[i] >= 2)
		{
			CardIndexTemp[i] -= 2;
			if (CheckCanContinuity(CardIndexTemp, GhostCount))
			{
				//PairsInfo.push_back(i);//�����Ҫ֪����ϸ�ĺ���Щ���ӣ���˴���Ҫ����
				return true;
			}
			//���ʧ�ܣ���ԭ�ƣ�������һ�ּ��
			CardIndexTemp[i] += 2;
		}
		else if (GhostCount > 0)  //ֻ��һ����,��ô��Ҫʹ��һ�Ź��Ƶ�������
		{

			--GhostCount;
			--CardIndexTemp[i];
			if (CheckCanContinuity(CardIndexTemp, GhostCount))
			{
				//PairsInfo.push_back(i);  //�����Ҫ֪����ϸ�ĺ���Щ���ӣ���˴���Ҫ����
				return true;
			}
			//���ʧ�ܣ���ԭ�ƣ�������һ�ּ��
			++CardIndexTemp[i];
			++GhostCount; 
		}
	}
	return false;
	//return (PairsInfo.size() > 0);
}
char TransToIndexModel(char const & _c)
{
	return (_c / 10) * 9 + (_c % 10) - 1;
}
int TransToIndexArrayModel(char const src[MAX_HANDCOUNT], char Des[MAX_INDEX])
{
	memset(Des,0,sizeof(char)*MAX_INDEX);
	size_t AddCount = 0;
	for (size_t i = 0; i < MAX_HANDCOUNT;++i)
	{
		if(src[i]!=0)
		{
			char TempIndex = TransToIndexModel(src[i]);
			++Des[TempIndex];
			++AddCount;
		}
	}
	return AddCount;
}




int main()
{
	char HandCard[MAX_HANDCOUNT] = {31,2,2,1,1,1,11,12,13,0,0,0,0,0};
	char CardIndex[MAX_INDEX];
	size_t HandCardSize = TransToIndexArrayModel(HandCard, CardIndex);

	bool res =  CheckCanHu(CardIndex, HandCardSize,2);
	printf("���κ��ƽ��:%d\n", res);
	return 0;
}