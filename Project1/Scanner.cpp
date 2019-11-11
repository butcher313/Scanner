#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class opTable {
	string id; //토큰 
	int t_num; //토큰 번호
	int t_count; //사용된 횟수 저장
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
	void increase() { //token 분리 중 operation 만나면 만난 횟수 증가
		this->t_count++;
	}
	int check_op(string id);//일치하는 operation 찾음
	void print() {
		cout << "<< opTable 출력 : >>\n" << endl;

		cout.setf(ios::left);
		cout << setw(15) <<"string" << setw(15) << "token number" << setw(15) << "token count\n" << endl;
		
		for (int i = 0; i < 100; i++) {
			if (this[i].t_count == 0)
				continue;
			cout << setw(15) << this[i].id << setw(15) << this[i].t_num << setw(15) << this[i].t_count << endl;
		}
		cout << endl;
	}

	//소멸자
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
		cout << "<< symbolTable 출력 : >>\n" << endl;
		cout.setf(ios::left);
		cout << setw(15) << "string" << setw(15) << "token number" << setw(15) << "token value" << endl;
		for (int i = 0; i < 100; i++) {
			if (i > 0 && this[i].t_val == 0)
				break;
			cout << setw(15) << this[i].id << setw(15) << this[i].t_num << setw(15) << this[i].t_val << endl;
		}
		cout << "\n\n" << endl;
	}
	int check_sym(string id);//symbol 중복 제거를 위한 함수 

	//소멸자
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

//정규표현으로 문법 체크, 토큰 분리
void tokenize(vector <string> &v, symbolTable *sym, opTable *op, int idx, int *sym_idx) {

	int start = 0;//id가 시작되는 처음 위치 저장
	string temp = v[idx]; //id를 임시 저장할 토큰
	int t_val = 0; //symbol table token값

	for (int i = 0; i < v[idx].length(); i++) {
	
		//문자가 '_' 혹은 알파벳이라면 
		if (v[idx][i] - 97 >= 0 && v[idx][i] - 97 <= 25 || v[idx][i] == '_'
			|| v[idx][i] - 65 >= 0 && v[idx][i] - 65 <= 25)
		{
			start = i; //id가 시작하는 처음 위치 저장 
			while (1) {
				//숫자나 id, _가 아닌 경우 optable 검색
				if (!(v[idx][i] - 97 >= 0 && v[idx][i] - 97 <= 25 || v[idx][i] == '_'
					|| v[idx][i] - 65 >= 0 && v[idx][i] - 65 <= 25))
				{
					if (op->check_op(temp.substr(start, i - start)))
						break;
					else if (sym->check_sym(temp.substr(start, i - start)))
						break; //중복되는것 있으면 중복 제거
					sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
					*sym_idx += 1;
					break;
				}
				i++;
			}
		}

		//숫자를 만난 경우 + 실수
		if (v[idx][i] - 48 >= 0 && v[idx][i] - 48 <= 9) {
			start = i;
			while (1) {
				if (v[idx][i] == '.') {//소수점이 나오는 경우
					i++; continue;
				}
				else if (!(v[idx][i] - 48 >= 0 && v[idx][i] - 48 <= 9)) {
					if (sym->check_sym(temp.substr(start, i - start)))
						break; //중복되는것 있으면 중복 제거
					sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
					*sym_idx += 1;
					break;
				}
				i++;
			}
		}

		//주석 기호 만난 경우
		if (v[idx][i] == '/') {
			start = i;
			if (v[idx][i + 1] == '/') {
				i++; // 처음에 '/'를 만난 후 i를 증가 시켜 다음 '/'로 이동함
				while (1) {
					if (v[idx][i] == '\0') {
						sym[*sym_idx].set(temp.substr(start, i - start), *sym_idx);
						*sym_idx += 1; break;
					}
					i++;
				}
			}
		}

		//"->" 처리
		if (v[idx][i] == '-') {
			if (v[idx][i + 1] == '>') {
				op[30].increase();
				i++;
			}
			else { // '-' 처리 
				op[31].increase();
			}
		}
		else if (v[idx][i] == '<') { //<<처리
			if (v[idx][i + 1] == '<') {
				op[32].increase();
				i++;
			}
			else { // '<' 처리
				op[33].increase();
			}
		}
		else if (v[idx][i] == '>') {
			op[34].increase();
		}
		//단일 문자 기호를 만나는 경우
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

//전체 페이지 tokenize
void tokenize_multile(vector <string> &v, symbolTable *sym, opTable *op) {

	int sym_idx = 0;//symbolTable 인덱스
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
		cout << "파일을 읽을 수 없음" << endl;
		return 0;
	}
	fileRead(wordVector, fin);
	fin.close();

	cout << "\n파일의 내용 : " << endl;
	for (int i = 0; i < wordVector.size(); i++)
		cout << wordVector[i] << endl;
	cout << "\n\n" << endl;

	////////////////////////////////////////////////////////////
	symbolTable *sym = new symbolTable[100];
	opTable *op = new opTable[100];

	//optable 초기화
	init_opTable(op);

	//토큰분리 
	tokenize_multile(wordVector, sym, op);

	//테이블 출력
	sym->print();
	op->print();
	
	return 0;
}