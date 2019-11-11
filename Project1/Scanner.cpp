#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class opTable {
	string id; //��ū 
	int t_num; //��ū ��ȣ
	int t_count; //���� Ƚ�� ����
public:
	opTable() {
		this->id = '\0';
		this->t_num = 0;
		this->t_count = 0;
	}
	void set(string id, int t_num) {
		this->id = id;
		this->t_num = t_num;
	}
	void increase() { //token �и� �� operation ������ ���� Ƚ�� ����
		this->t_count++;
	}
	int check_op(string id);//��ġ�ϴ� operation ã��
	void print() {
		cout << "<< opTable ��� : >>\n" << endl;

		cout.setf(ios::left);
		cout << setw(15) <<"string" << setw(15) << "token number" << setw(15) << "token count\n" << endl;
		
		for (int i = 0; i < 100; i++) {
			if (this[i].t_count == 0)
				continue;
			cout << setw(15) << this[i].id << setw(15) << this[i].t_num << setw(15) << this[i].t_count << endl;
		}
		cout << endl;
	}

	//�Ҹ���
	~opTable() {
		for(int i = 0; i < 100; i++)
			delete[i] this;
	}
};

int opTable :: check_op(string id) {

	for (int i = 0; i < 100; i++) {
		if (!(this[i].id.compare(id))) {
			this[i].t_count++;
			return 1;
		}
	}
	return 0;
}

class symbolTable {
	string id;
	int t_num;
	int t_val;
public:
	symbolTable() {
		id = '\0'; t_num = 3; t_val = 0;
	}
	void set(string id, int t_val) {
		this->id = id;
		this->t_val = t_val;
	}
	void print() {
		cout << "<< symbolTable ��� : >>\n" << endl;
		cout.setf(ios::left);
		cout << setw(15) << "string" << setw(15) << "token number" << setw(15) << "token value" << endl;
		for (int i = 0; i < 100; i++) {
			if (i > 0 && this[i].t_val == 0)
				break;
			cout << setw(15) << this[i].id << setw(15) << this[i].t_num << setw(15) << this[i].t_val << endl;
		}
		cout << "\n\n" << endl;
	}
	int check_sym(string id);//symbol �ߺ� ���Ÿ� ���� �Լ� 

	//�Ҹ���
	~symbolTable() {
		for (int i = 0; i < 100; i++)
			delete[i] this;
	}
};

int symbolTable::check_sym(string id) {

	for (int i = 0; i < 100; i++) {
		if (!(this[i].id.compare(id))) 
			return 1;
	}
	return 0;
}

//����ǥ������ ���� üũ, ��ū �и�
void tokenize(vector <string> &v, symbolTable *sym, opTable *op, int idx, int *sym_idx) {

	int start = 0;//id�� ���۵Ǵ� ó�� ��ġ ����
	string temp = v[idx]; //id�� �ӽ� ������ ��ū
	int t_val = 0; //symbol table token��

	for (int i = 0; i < v[idx].length(); i++) {
	
		//���ڰ� '_' Ȥ�� ���ĺ��̶�� 
		if (v[idx][i] - 97 >= 0 && v[idx][i] - 97 <= 25 || v[idx][i] == '_'
			|| v[idx][i] - 65 >= 0 && v[idx][i] - 65 <= 25)
		{
			start = i; //id�� �����ϴ� ó�� ��ġ ���� 
			while (1) {
				//���ڳ� id, _�� �ƴ� ��� optable �˻�
				if (!(v[idx][i] - 97 >= 0 && v[idx][i] - 97 <= 25 || v[idx][i] == '_'
					|| v[idx][i] - 65 >= 0 && v[idx][i] - 65 <= 25))
				{
					if (op->check_op(temp.substr(start, i - start)))
						break;
					else if (sym->check_sym(temp.substr(start, i - start)))
						break; //�ߺ��Ǵ°� ������ �ߺ� ����
					sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
					*sym_idx += 1;
					break;
				}
				i++;
			}
		}

		//���ڸ� ���� ��� + �Ǽ�
		if (v[idx][i] - 48 >= 0 && v[idx][i] - 48 <= 9) {
			start = i;
			while (1) {
				if (v[idx][i] == '.') {//�Ҽ����� ������ ���
					i++; continue;
				}
				else if (!(v[idx][i] - 48 >= 0 && v[idx][i] - 48 <= 9)) {
					if (sym->check_sym(temp.substr(start, i - start)))
						break; //�ߺ��Ǵ°� ������ �ߺ� ����
					sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
					*sym_idx += 1;
					break;
				}
				i++;
			}
		}

		//�ּ� ��ȣ ���� ���
		if (v[idx][i] == '/') {
			start = i;
			if (v[idx][i + 1] == '/') {
				i++; // ó���� '/'�� ���� �� i�� ���� ���� ���� '/'�� �̵���
				while (1) {
					if (v[idx][i] == '\0') {
						sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
						*sym_idx += 1; break;
					}
					i++;
				}
			}
		}

		//"->" ó��
		if (v[idx][i] == '-') {
			if (v[idx][i + 1] == '>') {
				op[30].increase();
				i++;
			}
			else { // '-' ó�� 
				op[31].increase();
			}
		}
		else if (v[idx][i] == '<') { //<<ó��
			if (v[idx][i + 1] == '<') {
				op[32].increase();
				i++;
			}
			else { // '<' ó��
				op[33].increase();
			}
		}
		else if (v[idx][i] == '>') {
			op[34].increase();
		}
		//���� ���� ��ȣ�� ������ ���
		else if (v[idx][i] == '=') {
			op[2].increase();
		}
		else if (v[idx][i] == ';') {
			op[3].increase();
		}
		else if (v[idx][i] == '+') {
			op[10].increase();
		}
		else if (v[idx][i] == '(') {
			op[5].increase();
		}
		else if (v[idx][i] == ')') {
			op[6].increase();
		}
		else if (v[idx][i] == '{') {
			op[7].increase();
		}
		else if (v[idx][i] == '}') {
			op[8].increase();
		}
		else if (v[idx][i] == '#') {
			op[11].increase();
		}
		else if (v[idx][i] == ':') {
			op[29].increase();
		}
	}
}

//��ü ������ tokenize
void tokenize_multile(vector <string> &v, symbolTable *sym, opTable *op) {

	int sym_idx = 0;//symbolTable �ε���
	for (int i = 0; i < v.size(); i++)
		tokenize(v, sym, op, i, &sym_idx);
}

void fileRead(vector <string> &v, ifstream &fin) {
	string line;

	while (getline(fin, line)) {
		v.push_back(line);
	}
}

void init_opTable(opTable *op) {

	op[0].set("int", 0);
	op[1].set("for", 1);
	op[2].set("=", 2);
	op[3].set(";", 3);
	op[4].set("while", 4);
	op[5].set("(", 5);
	op[6].set(")", 6);
	op[7].set("{", 7);
	op[8].set("}", 8);
	op[9].set("if", 9);
	op[10].set("+", 10);
	op[11].set("#", 11);
	op[12].set("include", 12);
	op[13].set("}", 13);
	op[14].set("if", 14);
	op[15].set("fstream", 15);
	op[16].set("iostream", 16);
	op[17].set("string", 17);
	op[18].set("vector", 18);
	op[19].set("using", 19);
	op[20].set("std", 20);
	op[21].set("class", 21);
	op[22].set("public", 22);
	op[23].set("this", 23);
	op[24].set("void", 24);
	op[25].set("cout", 25);
	op[26].set("endl", 26);
	op[27].set("continue", 27);
	op[28].set("namespace", 28);
	op[29].set(":", 29);
	op[30].set("->", 30);
	op[31].set("-", 31);
	op[32].set("<<", 32);
	op[33].set("<", 33);
	op[34].set(">", 34);

	op[35].set("iomanip", 35);
	op[36].set("setf", 36);
	op[37].set("ios", 37);
	op[38].set("setw", 38);
	op[39].set("delete", 39);
	op[40].set("return", 40);
	op[41].set("break", 41);
	op[42].set("else", 42);

}

int main() {

	vector<string> wordVector;
	ifstream fin("words.txt");
	if (!fin) {
		cout << "������ ���� �� ����" << endl;
		return 0;
	}
	fileRead(wordVector, fin);
	fin.close();

	cout << "\n������ ���� : " << endl;
	for (int i = 0; i < wordVector.size(); i++)
		cout << wordVector[i] << endl;
	cout << "\n\n" << endl;

	////////////////////////////////////////////////////////////
	symbolTable *sym = new symbolTable[100];
	opTable *op = new opTable[100];

	//optable �ʱ�ȭ
	init_opTable(op);

	//��ū�и� 
	tokenize_multile(wordVector, sym, op);

	//���̺� ���
	sym->print();
	op->print();
	
	return 0;
}