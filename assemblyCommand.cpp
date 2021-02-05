#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string ConvertBinaryToHex(string bin);
string ConvertHexToBinary(string hex);

void main()
{
	string code, reg, w, mod, rm, dispLow, dispHigh, result;
	string d = "1";
	string data;
	string input;
	string disp;
	bool flag=false;

	map<string, vector<string>> regW = { {"al", {"000","0"}}, {"ax", {"000","1"}}, {"cl", {"001","0"}}, {"cx", {"001","1"}},
										{"dl", {"010","0"}}, {"dx", {"010","1"}}, {"bl", {"011","0"}}, {"bx", {"011","1"}}, 
										{"ah", {"100","0"}}, {"sp", {"100","1"}}, {"ch", {"101","0"}}, {"bp", {"101","1"}}, 
										{"dh", {"110","0"}}, {"si", {"110","1"}}, {"bh", {"111","0"}}, {"di", {"111","1"}}, };

	map<string, vector<string>> adress = { {"[bx+si]",{"00", "000"}}, {"[bx+di]",{"00", "001"}}, 
	{"[bp+si]",{"00", "010"}}, {"[bp+di]",{"00", "011"}},
	{"[bx+si+disp]",{"01", "000"}}, {"[bx+di+disp]",{"01", "001"}},
	{"[bp+si+disp]",{"01", "010"}}, {"[bp+di+disp]",{"01", "011"}},
	{"[si]",{"00", "100"}}, {"[di]",{"00", "101"}}, 
	{"[disp]",{"00", "110"}}, {"[bx]",{"00", "111"}}, 
	{"[si+disp]",{"01", "100"}}, {"[di+disp]",{"00", "101"}},
	{"[bp+disp]",{"01", "110"}}, {"[bx+disp]",{"01", "111"}},
	{"al",{"11", "000"}}, {"ax",{"11", "000"}}, 
	{"cl",{"11", "001"}}, {"cx",{"11", "001"}}, 
	{"bx",{"11", "011"}}, {"bl",{"11", "011"}}, 
	{"dx",{"11", "010"}}, {"dl",{"11", "010"}},  
	{"sp",{"11", "100"}}, {"ah",{"11", "100"}},
	{"bp",{"11", "101"}}, {"ch",{"11", "101"}}, 
	{"si",{"11", "110"}}, {"dh",{"11", "110"}}, 
	{"di",{"11", "111"}}, {"bh",{"11", "111"}} };

	cout << "Input command: ";
	getline(cin, input);
	int pos = input.find(" ");
	string command = input.substr(0, pos);
	int pos1 = input.find(",");
	string operand1 = input.substr(pos + 1, pos1 - pos - 1);
	int pos2 = input.find('\n');
	string operand2 = input.substr(pos1 + 2, pos2);

	transform(operand1.begin(), operand1.end(), operand1.begin(), ::tolower);
	transform(operand2.begin(), operand2.end(), operand2.begin(), ::tolower);

	for (auto i : regW) {//запоминаем reg  и w
		if (operand1 == i.first) {
			for (int j = 0; j < i.second.size(); j = j + 2) {
				reg = i.second[j];
				w = i.second[j + 1];
			}
		}
	}
	int isPlus = operand2.find("+");
	if (isPlus!=-1 && operand2[0] == '[') { // базово-индексная
		int p = operand2.find("+");
		string oper = operand2.substr(p + 1);// до конца вместе со ]
		int p3 = oper.find("]");
		int p2 = oper.find("+");
		if (p2 == -1) {
			for (auto i : adress) {
				if (i.first == operand2) {//[bx+si]
					for (int j = 0; j < i.second.size(); j = j + 2) {
						mod = i.second[j];
						rm = i.second[j + 1];
						code = "100010";
						data = code + d + w + mod + reg + rm;
						flag = true;
						result = ConvertBinaryToHex(data);
						cout << data << " <" << result << ">";
						break;
					}
				}
			}
			if (flag == false) {//[bx+disp]
				disp = operand2.substr(p + 1, p3);
				operand2= operand2.substr(0, p+1) + "disp]";
				for (auto i : adress) {
					if (i.first == operand2) {
						for (int j = 0; j < i.second.size(); j = j + 2) {
							mod = i.second[j];
							rm = i.second[j + 1];
							code = "100010";
							disp = ConvertHexToBinary(disp);
							data = code + d + w + mod + reg + rm + disp;
							flag = true;
							result = ConvertBinaryToHex(data);
							cout << data << " <" << result << ">";
							break;
						}
					}
				}
			}
			
		}
		else {//[ax+ax+1]
			disp = oper.substr(p2 + 1, p3-3);
			int p4 = operand2.find("]");
			string oper1 = operand2.substr(0, p + 1);
			string oper2 = oper.substr(0, p2 + 1);
			oper1 += oper2;
			oper1 += "disp]";
			for (auto i : adress) {
				if (i.first == oper1) {
					for (int j = 0; j < i.second.size(); j = j + 2) {
						mod = i.second[j];
						rm = i.second[j + 1];
						code = "100010";
						disp = ConvertHexToBinary(disp);
						data = code + d + w + mod + reg + rm + disp;
						flag = true;
						result = ConvertBinaryToHex(data);
						cout << data << " <" << result << ">";
						break;
					}
				}
			}
			
		}
	}
	else if(isPlus == -1 && operand2[0] == '[') { //[di] [bx] [si]
		for (auto i : adress) {
			if (i.first == operand2) {
				for (int j = 0; j < i.second.size(); j = j + 2) {
					mod = i.second[j];
					rm = i.second[j + 1];
					code = "100010";
					data = code + d + w + mod + reg + rm;
					flag = true;
					result = ConvertBinaryToHex(data);
					cout << data << " <" << result << ">";
					break;
				}
				
			}
		}
		if (flag == false) { //[disp] прямая адресация
			int p = operand2.find("[");
			int p2 = operand2.find("]");
			string oper = operand2.substr(p + 1, p2-1);
			mod = "00";
			rm = "110";
			code = "100010";
			oper = ConvertHexToBinary(oper);
			data = code + d + w + mod + reg + rm + oper;
			result = ConvertBinaryToHex(data);
			cout << data << " <" << result << ">";
		}
		
	}
	else {
		for (auto key : adress) {
			if (key.first == operand2) {//al-bh регистровая адресация
				for (int j = 0; j < key.second.size(); j=j+2) {
					mod = key.second[j];
					rm = key.second[j + 1];
					code = "100010";
					data = code + d + w + mod + reg + rm;
					flag = true;
					result = ConvertBinaryToHex(data);
					cout << data << " <" << result << ">";
					break;
				}
				
			}
		}
		if (flag == false) {//непосредственная адресация
			string oper = ConvertHexToBinary(operand2);
			code = "1011";
			if (w == "1") {
				dispHigh = "00000000";
			}
			else {
				dispHigh = "";
			}
			data = code + w + reg + oper + dispHigh;
			result = ConvertBinaryToHex(data);
			cout << data << " <" << result << ">";
		}
	}
	
}
string ConvertBinaryToHex(string bin)
{
	string hex; char k;
	char* buf = const_cast<char*>(bin.c_str()); 
	int sum = 0;
	for (int i = 0; i < size(bin); i++) {
		if (buf[i] == '1') {
			sum += pow(2, 3 - i % 4);
		}
		if (i % 4 == 3) {
			switch (sum) {
			case 10: 
				k = 'a'; 
			break; 
			case 11: 
				k = 'b'; 
			break; 
			case 12: 
				k = 'c'; 
			break; 
			case 13: 
				k = 'd'; 
			break; 
			case 14: 
				k = 'e'; 
			break; 
			case 15: 
				k = 'f'; 
				break;
			default: 
				k = sum + '0'; 
				break;
			}
			hex += k; sum = 0;
		}
	}
	return hex;
}

string ConvertHexToBinary(string hex)
{
	string binary, bit4; int k;
	char* buf = const_cast<char*>(hex.c_str());

	for (int i = 0; i < size(hex); i++) {
		switch (buf[i]) {
		case 'a': 
		k = 10; 
		break; 
		case 'b': 
		k = 11; 
		break; 
		case 'c': 
		k = 12; 
		break; 
		case 'd': 
		k = 13; 
		break; 
		case 'e': 
		k = 14; 
		break; 
		case 'f': 
			k = 15; 
			break;
		default: 
			k = buf[i] - '0'; 
			break;
		}
		while (k > 0) {
			bit4 += '0' + k % 2;
			k /= 2;
		}
		while (size(bit4) < 4) {
			bit4 += '0';
		}
		reverse(begin(bit4), end(bit4)); binary += bit4;
		bit4 = "";
	}
	reverse(begin(binary), end(binary));
	while (size(binary) != 8 && size(binary) != 16) {
		binary += '0';
	}
	reverse(begin(binary), end(binary));

	return binary;
}
