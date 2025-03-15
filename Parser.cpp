#include <iostream>
#include "SLib.h"
using namespace std;
#define S_SIZE 8

enum mode
{
	DEFOULT,
	SELEKTOR,
	ATRYBUT,
	WARTOSC,
	DEFOULT_GLOB,
	WARTOSC_GLOB
};

enum mode2
{
	FIRST,
	SECOND,
	THIRD,
	BROKEN
};


class Section
{
public:
	SelectorList selList;
	AtrList atrList;
};

struct MainNode
{
	Section arr[S_SIZE];
	MainNode* next;
	MainNode* prev;
	int index;

	MainNode()
	{
		next = nullptr;
		prev = nullptr;
		index = 0;
	}
};

class MainList {
public:
	MainNode* head;
	MainNode* tail;

	MainList()
	{
		head = nullptr;
		tail = nullptr;
	}

	void PushFront()
	{
		MainNode* tmp = new MainNode();
		if (head == nullptr)
		{
			head = tmp;
			tail = tmp;
		}
		else
		{
			head->next = tmp;
			tmp->prev = head;
			head = tmp;
		}
	}

	void PushAtr(StringS val1, StringS val2)
	{
		head->arr[head->index].atrList.removeByName(val1);
		head->arr[head->index].atrList.PushFront(val1, val2);
	}

	void PushSel(StringS val)
	{
		if(head->arr[head->index].selList.FindByName(val) == -1)
			head->arr[head->index].selList.PushFront(val);
	}

	bool HeadEmpty()
	{
		if (head->arr[head->index].atrList.IsEmpty())
		{
			head->arr[head->index].selList.clear();
			return true;
		}
		return false;
	}

	void nextNode()
	{
		if (head->index < S_SIZE - 1)
		{
			head->index++;
		}
		else
		{
			PushFront();
		}
	}
	///

	bool CheckEmpty(MainNode* node)
	{
		for (int i = 0; i <= node->index; i++)
		{
			if (!node->arr[i].atrList.IsEmpty())
				return false;
		}
		return true;
	}

	void Delete(MainNode* node,int i)
	{
		node->arr[i].selList.clear();
		node->arr[i].atrList.clear();
		if (CheckEmpty(node))
		{
			if (node == head && node == tail)
			{
				return;
			}
			else if (node == head)
			{
				node->prev->next = nullptr;
				head = node->prev;
				delete node;
			}
			else if (node == tail)
			{
				node->next->prev = nullptr;
				tail = node->next;
				delete node;
			}
			else
			{
				node->prev->next = node->next;
				node->next->prev = node->prev;
				delete node;
			}
		}
	}
	///

	void Print()
	{
		MainNode* tmp;
		tmp = tail;
		cout << " {";
		while (tmp != nullptr)
		{
			cout << " <";
			for (int i = 0; i <= tmp->index; i++)
			{
				cout << " [  selectors: ";
				tmp->arr[i].selList.Print();
				cout << "  attributes: ";
				tmp->arr[i].atrList.Print();
				cout << " ]" << endl;
			}
			tmp = tmp->next;
			cout << "> ";
		}
		cout << "} ";
	}
};

struct Flags{
	int mode;
	int testMode;
	int qCounter; //liczy ilosc '?'
	int sCounter; //liczy ilosc '*'

	Flags()
	{
		mode = DEFOULT;
		testMode = FIRST;
		qCounter = 0;
		sCounter = 0;
	}
};


StringS FormatString(StringS& a)
{
	int to_trim_l = 0;
	int to_trim_r = 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (isspace(a[i]))
		{
			to_trim_l++;
		}
		else
			break;
	}
	for (int i = a.length() - 1; i >= to_trim_l; i--)
	{
		if (isspace(a[i]))
		{
			to_trim_r++;
		}
		else
			break;
	}
	return a.strPart(to_trim_l, a.length() - to_trim_l - to_trim_r);
}


bool isNumber(StringS& str)
{
	int size = str.length();
	for (int i = 0; i < size; i++)
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return false;
	}
	return true;
}


//i,S,?
void SelectorAmount(MainList& mList, int number)
{
	bool end = false;
	int result = -1;
	int counter = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (number == counter)
				{
					result = tmp->arr[i].selList.Size();
					//goto ESC;
					end = true;
					break;
				}
			}
		}
		if (end == true) break;
		tmp = tmp->next;
	}
	//ESC:
	if (result > -1)
	{
		cout << number << ",S,? == " << result << endl;
	}
}


//i,S,j
void ChooseSelector(MainList& mList, int blockNumber, int selectorNumber)
{
	int counter = 0;
	MainNode* tmp = mList.tail;
	StringS result;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (blockNumber == counter)
				{
					if (tmp->arr[i].selList.GetByIndex(selectorNumber - 1, result))
						cout << blockNumber << ",S," << selectorNumber << " == " << result << endl;
				}
			}
		}
		tmp = tmp->next;
	}
}


//i,A,?
void AtrybuteAmount(MainList& mList, int number)
{
	bool end = false;
	int result = -1;
	int counter = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (number == counter)
				{
					result = tmp->arr[i].atrList.Size();
					//goto ESC;
					end = true;
					break;
				}
			}

		}
		if (end == true) break;
		tmp = tmp->next;
	}
	//ESC:
	if (result > -1)
	{
		cout << number << ",A,? == " << result << endl;
	}
}


//i,A,n
void WartoscAtr(MainList& mList, int blockNumber, StringS name)
{
	int counter = 0;
	MainNode* tmp = mList.tail;
	StringS result;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (blockNumber == counter)
				{
					if (tmp->arr[i].atrList.ValueByName(name,result))
						cout << blockNumber << ",A,"<< name <<" == " << result << endl;
				}
			}
		}
		tmp = tmp->next;
	}
}

//i,D,* 
void DeleteSection(MainList& mList, int sectionNr)
{
	int counter = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (sectionNr == counter)
				{
					//tmp->Delete(i);
					mList.Delete(tmp, i);
					cout << sectionNr << ",D,* == deleted" << endl;
					return;
				}
			}
		}
		tmp = tmp->next;
	}
}

//i,D,n
void DeleteAtribute(MainList& mList,int sectionNr,StringS& name)
{
	int counter = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				counter++;
				if (sectionNr == counter)
				{
					if (tmp->arr[i].atrList.removeByName(name))
					{
						cout << sectionNr << ",D,"<< name <<" == deleted" << endl;
						if(tmp->arr[i].atrList.IsEmpty())
							mList.Delete(tmp, i);
						return;
					}
				}
			}
		}
		tmp = tmp->next;
	}
}

//z,S,?
void SelectorAmount(MainList& mList, StringS& name)
{
	int result = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				if (tmp->arr[i].selList.FindByName(name) != -1)
					result++;
			}
		}
		tmp = tmp->next;
	}
	cout << name << ",S,? == " << result << endl;
}

//n,A,?
void AttributeAmount(MainList& mList, StringS& name)
{
	int result = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
			{
				if (tmp->arr[i].atrList.FindByName(name) != -1)
					result++;
			}
		}
		tmp = tmp->next;
	}
	cout << name << ",A,? == " << result << endl;
}

//z,E,n
void ValueOfAtrFromSel(MainList& mList, StringS& selName, StringS& atrName)
{
	MainNode* tmp = mList.head;
	StringS result;
	while (tmp != nullptr)
	{
		for (int i = tmp->index; i >= 0; i--)
		{
			if (tmp->arr[i].selList.FindByName(selName) != -1)
			{
				if (tmp->arr[i].atrList.ValueByName(atrName, result))
				{
					cout << selName << ",E," << atrName << " == " << result << endl;
					return;
				}
			}
		}
		tmp = tmp->prev;
	}
	//ESC:
}


void execute(MainList& mList,StringS& tmp1, StringS& tmp2, StringS& tmp3)
{
	if (isNumber(tmp1))//i,S,?  i,A,?  i,S,j  i,A,n  i,D,*  i,D,n
	{
		if (tmp2[0] == 'S')
		{
			if (tmp3[0] == '?')//i,S,?
			{
				SelectorAmount(mList, tmp1.toInt());
			}
			else//i,S,j
			{
				ChooseSelector(mList, tmp1.toInt(), tmp3.toInt());
			}
		}
		else if (tmp2[0] == 'A')  
		{
			if (tmp3[0] == '?')//i,A,?
			{
				AtrybuteAmount(mList, tmp1.toInt());
			}
			else//i,A,n
			{
				WartoscAtr(mList, tmp1.toInt(), tmp3);
			}
		}
		else if (tmp2[0] == 'D')
		{
			if (tmp3[0] == '*')//i,D,* 
			{
				DeleteSection(mList, tmp1.toInt());
			}
			else//i,D,n
			{
				DeleteAtribute(mList,tmp1.toInt(), tmp3);
			}
		}
	}
	else//n,A,?   z,S,?   z,E,n
	{
		if (tmp2[0] == 'S')//z,S,?
		{
			SelectorAmount(mList, tmp1);
		}
		else if (tmp2[0] == 'A')//n,A,?
		{
			AttributeAmount(mList, tmp1);
		}
		else if (tmp2[0] == 'E')//z,E,n
		{
			ValueOfAtrFromSel(mList, tmp1, tmp3);
		}
	}
}


// ? ==
void sectionAmount(MainList& mList)
{
	int counter = 0;
	MainNode* tmp = mList.tail;
	while (tmp != nullptr)
	{
		for (int i = 0; i <= tmp->index; i++)
		{
			if (!tmp->arr[i].atrList.IsEmpty())
				counter++;
		}
		tmp = tmp->next;
	}
	cout << "? == " << counter << endl;
}


void TrybPytan(MainList& mList,Flags& mainFlags)
{
	bool trybPytan = true;
	StringS tmp1;
	StringS tmp2;
	StringS tmp3;
	tmp1 = "";
	tmp2 = "";
	tmp3 = "";
	char x;
	mainFlags.testMode = FIRST;
	while (cin && trybPytan)
	{
		if (!cin.get(x))
			break;
		if (mainFlags.testMode == FIRST)
		{
			if (x == '?')
			{
				mainFlags.sCounter = 0;
				sectionAmount(mList);
			}
			else if (x == ',')
			{
				mainFlags.sCounter = 0;
				mainFlags.testMode = SECOND;
				//cout << "SEC";
			}
			else if (x == '*')
			{
				mainFlags.sCounter++;
				if (mainFlags.sCounter >= 4)
				{
					trybPytan = false;
				}
			}
			else if (x != '\n')
			{
				mainFlags.sCounter = 0;
				tmp1.append(x);
			}
			else
			{
				mainFlags.sCounter = 0;
			}
		}
		else if (mainFlags.testMode == SECOND)
		{
			if (x == ',')
			{
				mainFlags.testMode = THIRD;
				//cout << "THIRD";
			}
			else if (x != '\n')
			{
				tmp2.append(x);
			}
		}
		else if (mainFlags.testMode == THIRD)
		{
			if (x == ',')
			{
				mainFlags.testMode = BROKEN;
				//cout << "BROKEN";
			}
			else if (x == '\n')
			{
				execute(mList,tmp1,tmp2,tmp3);
				tmp1 = "";
				tmp2 = "";
				tmp3 = "";
				mainFlags.testMode = FIRST;
				//cout << "EXECUTED";
			}
			else
			{
				tmp3.append(x);
			}
		}
		else if (mainFlags.testMode == BROKEN)
		{
			if (x == '\n')
			{
				tmp1 = "";
				tmp2 = "";
				tmp3 = "";
				mainFlags.testMode = FIRST;
			}
		}
	}
}


int main()
{
	MainList mList;
	StringS tmp1;
	StringS tmp2;
	Flags mainFlags;
	tmp1 = "";
	tmp2 = "";

	char x;

	mList.PushFront();

	while (cin)
	{
		cin.get(x);
		/*
		if(x == '*')
		{
			mList.Print();
		}
		else*/
		if (mainFlags.mode == DEFOULT)
		{
			if (x == '{')
			{
				mainFlags.qCounter = 0;
				if (tmp1.length() > 0)
				{
					tmp1 = FormatString(tmp1);
					mList.PushSel(tmp1);
					tmp1 = "";
				}
				mainFlags.mode = ATRYBUT;
			}
			else if (x == ',')
			{
				mainFlags.qCounter = 0;
				if (tmp1.length() > 0)
				{
					tmp1 = FormatString(tmp1);
					mList.PushSel(tmp1);
					tmp1 = "";
				}
				mainFlags.mode = SELEKTOR;
			}
			else if (x == '?')
			{
				mainFlags.qCounter++;
				if (mainFlags.qCounter >= 4)
				{
					tmp1 = "";
					mainFlags.qCounter = 0;
					TrybPytan(mList,mainFlags);
				}
			}
			else if (x != '\n')
			{
				mainFlags.qCounter = 0;
				tmp1.append(x);
			}
		}
		else if (mainFlags.mode == SELEKTOR)
		{
			if (x == ',')
			{
				mainFlags.qCounter = 0;
				if (tmp1.length() > 0)
				{
					tmp1 = FormatString(tmp1);
					mList.PushSel(tmp1);
					tmp1 = "";
				}
				mainFlags.mode = SELEKTOR;
			}
			else if (x == '{')
			{
				mainFlags.qCounter = 0;
				if (tmp1.length() > 0)
				{
					tmp1 = FormatString(tmp1);
					mList.PushSel(tmp1);
					tmp1 = "";
				}
				mainFlags.mode = ATRYBUT;
			}
			else if (x == '?')
			{
				mainFlags.qCounter++;
				if (mainFlags.qCounter >= 4)
				{
					tmp1 = "";
					mainFlags.qCounter = 0;
					TrybPytan(mList, mainFlags);
				}
			}
			else if (x != '\n')
			{
				mainFlags.qCounter = 0;
				tmp1.append(x);
			}
		}
		else if (mainFlags.mode == ATRYBUT)
		{
			if (x == '}')
			{
				mainFlags.qCounter = 0;
				if(!mList.HeadEmpty())
					mList.nextNode();
				tmp1 = "";
				mainFlags.mode = DEFOULT;
			}
			else if (x == ':')
			{
				mainFlags.qCounter = 0;
				mainFlags.mode = WARTOSC;
			}
			else if (x == '?')
			{
				mainFlags.qCounter++;
				if (mainFlags.qCounter >= 4)
				{
					tmp1 = "";
					mainFlags.qCounter = 0;
					TrybPytan(mList, mainFlags);
				}
			}
			else if (x != '\n')
			{
				mainFlags.qCounter = 0;
				tmp1.append(x);
			}
		}
		else if (mainFlags.mode == WARTOSC)
		{
			if (x == '}')
			{
				mainFlags.qCounter = 0;
				tmp1 = FormatString(tmp1);
				tmp2 = FormatString(tmp2);
				mList.PushAtr(tmp1, tmp2);
				mList.nextNode();
				tmp1 = "";
				tmp2 = "";
				mainFlags.mode = DEFOULT;
			}
			else if (x == ';')
			{
				mainFlags.qCounter = 0;
				tmp1 = FormatString(tmp1);
				tmp2 = FormatString(tmp2);
				mList.PushAtr(tmp1, tmp2);
				tmp1 = "";
				tmp2 = "";
				mainFlags.mode = ATRYBUT;
			}
			else if (x == '?')
			{
				mainFlags.qCounter++;
				if (mainFlags.qCounter >= 4)
				{
					tmp1 = "";
					mainFlags.qCounter = 0;
					TrybPytan(mList, mainFlags);
				}
			}
			else if (x != '\n')
			{
				mainFlags.qCounter = 0;
				tmp2.append(x);
			}
		}
	}
}
