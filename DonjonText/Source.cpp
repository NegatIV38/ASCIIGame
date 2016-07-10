#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <ctime>
#include <cmath>
#include "Rooms.h"
#include <memory>

using namespace std;

#define SIZE_H 50
#define SIZE_V 20
#define N_MAX 1024
#define T_HM 20
#define T_VM 20
//#define AFF_TABLE

bool help;
bool inMarch;
bool inInventory;

struct Personnage{
	int HP;
	int HPM;
	int PT_ACTION;
	int PTM;
	int LVL;
	int EXP;
	int EXPM;
	int score;
	Armure arm;
	vector<Objet> objs;
};

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void initDunTest(char dun[T_VM][T_HM]){
	for (int i = 0; i < T_VM; i++){
		for (int j = 0; j < T_HM; j++){
			dun[i][j] = ' ';
		}
	}
}

bool isRoom(char c){
	if (c == '0' + 1 || c == '0' + 2 || c == '0' + 3){
		return true;
	}
	return false;
}

bool allowUp(char dun[T_VM][T_HM], pair<int, int>& pos){
	if (isRoom(dun[pos.first][pos.second]) || dun[pos.first][pos.second] == 'I' || dun[pos.first][pos.second] == ']' || dun[pos.first][pos.second] == 'L'){
		return true;
	}
	return false;
}

bool allowDown(char dun[T_VM][T_HM], pair<int, int>& pos){
	if (isRoom(dun[pos.first][pos.second]) || dun[pos.first][pos.second] == 'I' || dun[pos.first][pos.second] == '7' || dun[pos.first][pos.second] == 'P'){
		return true;
	}
	return false;
}

bool allowD(char dun[T_VM][T_HM], pair<int, int>& pos){
	if (isRoom(dun[pos.first][pos.second]) || dun[pos.first][pos.second] == '-' || dun[pos.first][pos.second] == 'P' || dun[pos.first][pos.second] == 'L'){
		return true;
	}
	return false;
}

bool allowG(char dun[T_VM][T_HM], pair<int, int>& pos){
	if (isRoom(dun[pos.first][pos.second]) || dun[pos.first][pos.second] == '-' || dun[pos.first][pos.second] == ']' || dun[pos.first][pos.second] == '7'){
		return true;
	}
	return false;
}

void affMap(char dun[T_VM][T_HM], pair<int, int>& pos,HANDLE& consoleHandle){
	for (int i = 0; i < T_VM; i++){
		for (int j = 0; j < T_HM; j++){
			if (i == pos.first && j == pos.second){
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << dun[i][j];
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
			}
			else{
				cout << dun[i][j];
			}
		}
		cout << endl;
	}
}

void generateDunTest(bool room, char dun[T_VM][T_HM], pair<int, int>& pos){
	system("cls");
	
	int r1 = rand() % listRooms.size();
	Cote d ;
	int c1 = rand() % 6;

	if (!room){
		
	}
	else{
		dun[pos.first][pos.second] = '0' + r1 + 1;
	}
	pair<int, int> temp = pos;
		d = Cote(rand() % 4);
	int cmp = 0;
	do{
		switch (d)
		{
		case HAUT:
			if (dun[pos.first - 1][pos.second] == ' ' && allowUp(dun,pos)){
				pos.first--;
			}
			break;
		case GAUCHE:
			if (dun[pos.first][pos.second - 1] == ' ' && allowG(dun,pos)){
				pos.second--;
			}
			break;
		case DROITE:
			if (dun[pos.first][pos.second + 1] == ' ' && allowD(dun,pos)){
				pos.second++;
			}
			break;
		case BAS:
			if (dun[pos.first + 1][pos.second] == ' ' && allowDown(dun,pos)){
				pos.first++;
			}
			break;
		default:
			break;
		}
		cmp++;
		d = Cote(d - 1);
		if (d < 0){
			d = Cote(3);
		}
	} while (pos == temp && cmp < 20);

	//affMap(dun,consoleHandle);
	cout << "LOADING...";
	if (pos.first != 0 && pos.second != 0 && pos.first != T_VM && pos.second != T_HM && cmp < 20){
		generateDunTest(room, dun, pos);
	}
}

string armureToString(Armure arg){
	switch (arg)
	{
	case SS_ARM:
		return "Aucun";
		break;
	case ARM_BASE:
		return "Armure basique";
		break;
	case ARM_STD:
		return "Armure standard";
		break;
	case ARM_GUERR:
		return "Armure de Guerrier";
		break;
	case ARM_GUERI:
		return "Armure de Guerrisseur";
		break;
	case ARM_ULTI:
		return "Armure Ultime";
		break;
	default:
		break;
	}
}

string objetToString(Objet obj){
	switch (obj)
	{
	case SS_OBJ:
		return "Aucun";
		break;
	case COFFRE:
		return "Coffre";
		break;
	default:
		break;
	}
}

void affStat(Personnage& p, speRoom& r_spe){
	int ligne = 0;
	if (help){
		ligne += 12;
	}
	ligne += r_spe.lenV;
	ligne++;
	ligne += T_VM;
	gotoxy(0, ligne);

	cout << "                                                           " << endl;
	cout << "                                                           " << endl;
	cout << "                                                           " << endl;
	cout << "                                                           " << endl;
	cout << "                                                           " << endl;
	cout << "                                                           " << endl;
	cout << "                                                           " << endl;

	gotoxy(0, ligne);

	cout << "Lv." << p.LVL << endl;
	cout << "HP : " << p.HP << "/" << p.HPM << endl;
	cout << "Action Points : " << p.PT_ACTION << "/" << p.PTM << endl;
	cout << "Exp. : " << p.EXP << "/" << p.EXPM << endl;
	cout << "Credits : " << p.score <<"$"<< endl;
	cout << "Equipement : " << armureToString(p.arm) << endl;
	cout << "Objet : " << objetToString(p.objs.at(0)) << endl;
}

void changerChar(int posX, int posY, char c){
	for (int i = 0; i < 3; i++){
		cout << '\r';
	}
	for (int j = 0; j < 5; j++){
		cout << '\b';
	}
	cout << c;
}

void affMarchand(int select){
	inMarch = true;
	system("cls");
	switch (select)
	{
	case 1:
		cout << "-->\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "\t" << "QUITTER" << endl;
		break;
	case 2:
		cout << "\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "-->\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "\t" << "QUITTER" << endl;
		break;
	case 3:
		cout << "\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "-->\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "\t" << "QUITTER" << endl;
		break;
	case 4:
		cout << "\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "-->\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "\t" << "QUITTER" << endl;
		break;
	case 5:
		cout << "\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "-->\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "\t" << "QUITTER" << endl;
		break;
	case 6:
		cout << "\t" << armureToString(ARM_BASE) << " : 100$" << endl;
		cout << "\t" << armureToString(ARM_STD) << " : 500$" << endl;
		cout << "\t" << armureToString(ARM_GUERR) << " : 1000$" << endl;
		cout << "\t" << armureToString(ARM_GUERI) << " : 8000$" << endl;
		cout << "\t" << armureToString(ARM_ULTI) << " : 20000$" << endl;
		cout << "-->\t" << "QUITTER" << endl;
		break;
	default:
		break;
	}
	
}

void affInventory(Personnage& p, int selection){
	inInventory = true;
	system("cls");
	cout << "--Objet Equipe--" << endl;
	cout << "\t" << objetToString(p.objs.at(0)) << endl;
	cout << endl << "--Inventaire--" << endl;

	for (int i = 1; i < p.objs.size(); i++){
		if (i == selection){
			cout << "-->\t" << objetToString(p.objs.at(i)) << endl;
		}
		else{
			cout << "\t" << objetToString(p.objs.at(i)) << endl;
		}
	}
}

void draw(pair<int, int>& pos, char& pers, vector<string>& room, char map[T_VM][T_HM], pair<int,int>& posDun){
	inMarch = false;
	system("cls");

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);//Ici, on récupère un handle vers ce qui représente la sortie standard sous Windows.
	
	if (help){

		cout << "AIDE : " << endl;
		cout << "Z,Q,S,D : Deplacements" << endl;
		cout << "SpaceBar : Attaquer/Activer" << endl;
		cout << "E : Inventaire" << endl;
		cout << "H : Activer/Desactiver l'aide" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "@";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Ennemi Normal" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "?";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Ennemi Hesitant" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "!";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Ennemi Puissant" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "$";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Marchand" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "O";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Bonus de vie" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << ".";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Bonus d'action" << endl;
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "+";
		SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		cout << " : Objet" << endl;
	}
	

	/*for (int i = 0; i < SIZE_V; i++){
		for (int j = 0; j < SIZE_H; j++){
			if (i == pos.first && j == pos.second){
				cout << pers;
			}
			else if (i == 0 || i == SIZE_V - 1 || j == 0 || j == SIZE_H - 1){
				cout << char(219);
			}
			else{
				cout << " ";
			}
		}
		cout << endl;
	}*/


	/*int i = 0;
	int j = 0;
	while (!room.eof()){
		char a;
		a = room.get();
		switch (a)
		{
		case '1':
			cout << char(219);
			i++;
			break;
		case '.':
			cout << " ";
			i++;
			break;
		default:
			j++;
			i = 0;
			cout << a;
			break;
		}
		
	}*/

	int i = 0;
	while (room.at(i) != "END"){
		int j = 0;
		while (room.at(i).at(j) != '\n')
		{
			switch (room.at(i).at(j))
			{
			case '1':
				cout << char(219);
				break;
			case '.':
				cout << " ";
				break;
			case '2':
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "@";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case '3':
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "!";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case '4':
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "?";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case '0':
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "$";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case 'o'://HP
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED |FOREGROUND_GREEN| FOREGROUND_INTENSITY);
				cout << "O";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case '-'://ACTION
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << ".";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			case '+':
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "+";
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
				break;
			default:
				cout << room.at(i).at(j);
				break;
			}
			if (i == pos.first && j == pos.second){
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << '\b' << pers;
				SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
			}
			j++;
		}
		i++;
		cout << endl;
	}
	cout << "MAP : " << endl;
	affMap(map,posDun,consoleHandle);
}

bool isHole(vector<string>& room, speRoom& r_spe,Cote c){
	switch (c)
	{
	case HAUT:
		for (int i = 0; i < r_spe.lenH; i++){
			if (room.at(0).at(i) == '.'){
				return true;
			}
		}
		break;
	case GAUCHE:
		for (int i = 0; i < r_spe.lenV; i++){
			if (room.at(i).at(0) == '.'){
				return true;
			}
		}
		break;
	case DROITE:
		for (int i = 0; i < r_spe.lenV; i++){
			if (room.at(i).at(r_spe.lenH-1) == '.'){
				return true;
			}
		}
		break;
	case BAS:
		for (int i = 0; i < r_spe.lenH; i++){
			if (room.at(r_spe.lenV-1).at(i) == '.'){
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

void makeHole(vector<string>& room, speRoom& r_spe){
	int c = rand() % 3;
	c++;
	while (c != 0)
	{
		int r = rand() % 4;
		//if (!isHole(room, r_spe, Cote(r))){
			switch (r)
			{
			case 0:
				room.at(0).at(room.at(0).size() / 2) = '.';
				break;
			case 1:
				room.at(r_spe.lenV / 2).at(0) = '.';
				break;
			case 2:
				room.at(r_spe.lenV / 2).at(r_spe.lenH - 1) = '.';
				break;
			case 3:
				room.at(r_spe.lenV - 1).at(room.at(r_spe.lenV - 1).size() / 2) = '.';
				break;
			default:
				break;
			}
			
		//}
		c--;
	}
}

void makeHole(vector<string>& room, speRoom& r_spe, Cote c){
	int r = c;
	switch (r)
	{
	case 0:
		room.at(0).at(room.at(0).size() / 2) = '.';
		break;
	case 1:
		room.at(r_spe.lenV / 2).at(0) = '.';
		break;
	case 2:
		room.at(r_spe.lenV / 2).at(r_spe.lenH - 1) = '.';
		break;
	case 3:
		room.at(r_spe.lenV - 1).at(room.at(r_spe.lenV - 1).size() / 2) = '.';
		break;
	default:
		break;
	}
}

void makeHole(vector<string>& room, speRoom& r_spe,Cote c ,pair<int, int> pos){
	switch (c)
	{
	case HAUT:
		room.at(0).at(pos.second) = '.';
		break;
	case GAUCHE:
		room.at(pos.first).at(0) = '.';
		break;
	case DROITE:
		room.at(pos.first).at(r_spe.lenH - 1) = '.';
		break;
	case BAS:
		room.at(r_spe.lenV - 1).at(pos.second) = '.';
		break;
	default:
		break;
	}
}

void makeHole(vector<string>& room, speRoom& r_spe, char dun[T_VM][T_HM], pair<int, int>& pos){
	if (dun[pos.first-1][pos.second] != ' '){
		makeHole(room, r_spe, HAUT);
	}
	if (dun[pos.first+1][pos.second] != ' '){
		makeHole(room, r_spe, BAS);
	}
	if (dun[pos.first][pos.second-1] != ' '){
		makeHole(room, r_spe, GAUCHE);
	}
	if (dun[pos.first][pos.second+1] != ' '){
		makeHole(room, r_spe, DROITE);
	}
}

void removeHoles(vector<string>& room, speRoom& r_spe){
	room.at(0).at(room.at(0).size() / 2) = '1';
	room.at(r_spe.lenV / 2).at(0) = '1';
	room.at(r_spe.lenV / 2).at(r_spe.lenH - 1) = '1';
	room.at(r_spe.lenV - 1).at(room.at(r_spe.lenV - 1).size() / 2) = '1';
}

void addEnn(vector<string>& room, speRoom& r_spe){
	int r = rand() % 3;
	int x, y,z;
	r++;
	int k = rand() % 10;
	while (r > 0){
		do{
			x = rand() % (r_spe.lenH - 2);
			x++;
			y = rand() % (r_spe.lenV - 2);
			y++;
			/*if (room[y].at(x) == '.'){
				
			}*/
		} while (room.at(y).at(x) != '.');
		if (k != 0){
			z = rand() % 101;
			if (z <= 35){
				room.at(y).at(x) = '4';
			}
			else if (z > 35 && z < 85){
				room.at(y).at(x) = '2';
			}
			else{
				room.at(y).at(x) = '3';
			}
			r--;
		}
		else{
			room.at(y).at(x) = '0';
			r = 0;
		}
	}
}

bool isEnn(char c){
	if (c == '2' || c == '3' || c == '4'){
		return true;
	}
	return false;
}

bool isMarch(char c){
	if (c == '0'){
		return true;
	}
	return false;
}

bool isBonus(char c){
	if (c == 'o' || c == '-'){
		return true;
	}
	return false;
}

bool isObject(char c){
	if (c == '+'){
		return true;
	}
	return false;
}

void removeEnns(vector<string>& room, speRoom& r_spe){
	for (int i = 0; i < r_spe.lenV; i++){
		for (int j = 0; j < r_spe.lenH; j++){
			if (isEnn(room.at(i).at(j)) || isMarch(room.at(i).at(j))){
				room.at(i).at(j) = '.';
			}
		}
	}
}

void removeBonus(vector<string>& room, speRoom& r_spe){
	for (int i = 0; i < r_spe.lenV; i++){
		for (int j = 0; j < r_spe.lenH; j++){
			if (isBonus(room.at(i).at(j)) || isObject(room.at(i).at(j))){
				room.at(i).at(j) = '.';
			}
		}
	}
}

void addBonus(vector<string>& room, speRoom& r_spe){
	int r = rand() % 4;
	int x, y, z;
	while (r > 0){
		do{
			x = rand() % r_spe.lenH - 1;
			x++;
			y = rand() % r_spe.lenV - 1;
			y++;
			/*if (room[y].at(x) == '.'){

			}*/
		} while (room.at(y).at(x) != '.');
		z = rand() % 2;
		switch (z)
		{
		case 0:
			room.at(y).at(x) = 'o';
			break;
		case 1:
			room.at(y).at(x) = '-';
			break;
		default:
			break;
		}
		
		r--;
	}
}

bool isEmpty(vector<string>& room, speRoom& r_spe){
	for (int i = 0; i < r_spe.lenV; i++){
		for (int j = 0; j < r_spe.lenH; j++){
			if (isEnn(room.at(i).at(j))){
				return false;
			}
		}
	}
	return true;
}

void addObject(vector<string>& room, speRoom& r_spe){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (isEmpty(room, r_spe)){
		int r = rand() % 5;
		if (r == 0){
			int ligne = 0;
			if (help){
				ligne += 12;
			}
			ligne += r_spe.lenV/2;
			room.at(r_spe.lenV / 2).at(r_spe.lenH / 2) = '+';
			gotoxy(r_spe.lenH / 2, ligne);
			SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << "+";
			SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
		}
	}
}

void objectNewInInventory(Personnage& p){
	int r = rand() % NB_OBJECTS;
	r++;
	p.objs.push_back(Objet(r));
}

int nbEnnOnLines(pair<int, int>& pos, vector<string>& room, speRoom& r_spe, Personnage& p){
	int s = 0;
	
	for (int i = pos.second; i < r_spe.lenH; i++){
		switch (room.at(pos.first).at(i))
		{
		case '1':
			i = r_spe.lenH;
			break;
		case '2'://@
			s++;
			break;
		case '3'://!
			s += 2;
			break;
		case '4'://?
			s += rand() % 2;
			break;
		default:
			break;
		}
	}
	for (int k = pos.second; k > 0; k--){
		switch (room.at(pos.first).at(k))
		{
		case '1':
			k = 0;
			break;
		case '2'://@
			s++;
			break;
		case '3'://!
			s += 2;
			break;
		case '4'://?
			s += rand() % 2;
			break;
		default:
			break;
		}
	}
	for (int j = pos.first; j < r_spe.lenV; j++){
		switch (room.at(j).at(pos.second))
		{
		case '1':
			j = r_spe.lenV;
			break;
		case '2'://@
			s++;
			break;
		case '3'://!
			s += 2;
			break;
		case '4'://?
			s += rand() % 2;
			break;
		default:
			break;
		}
	}

	for (int l = pos.first; l >0; l--){
		switch (room.at(l).at(pos.second))
		{
		case '1':
			l = 0;
			break;
		case '2'://@
			s++;
			break;
		case '3'://!
			s += 2;
			break;
		case '4'://?
			s += rand() % 2;
			break;
		default:
			break;
		}
	}


	int e;
	switch (p.arm)
	{
	case ARM_BASE:
		if (rand() % 2 == 0){
			s = max(s - 1, 0);
		}
		break;
	case ARM_STD:
		if (rand() % 4 != 0){
			s = max(s - 2, 0);
		}
		break;
	case ARM_GUERR:
		s /= 2;
		break;
	case ARM_GUERI:
		e = rand() % 5;
		if (e == 0){
			s *= -1;
		}
		break;
	case ARM_ULTI:
		s = 1;
		break;
	default:
		break;
	}
	return s;
}

void addPoint(char c, Personnage& p){
	switch (c){
	case '2':
		if (p.objs.at(0) == COFFRE){
			p.score += 60;
		}
		else{
			p.score += 30;
		}
		break;
	case '3':
		if (p.objs.at(0) == COFFRE){
			p.score += 100;
		}
		else{
			p.score += 50;
		}
		break;
	case '4':
		if (p.objs.at(0) == COFFRE){
			p.score += 20;
		}
		else{
			p.score += 10;
		}
		break;
	}
}

void changeRoom(pair<vector<string>*, speRoom*>& actual){
	int r = rand() % listRooms.size();
	actual.first = listRooms.at(r).first;
	actual.second = listRooms.at(r).second;
}

void changeRoom(pair<vector<string>*, speRoom*>& actual, char newRoom){
	switch (newRoom)
	{
	case '3':
		actual.first = listRooms.at(2).first;
	actual.second = listRooms.at(2).second;
		break;
	case '1':

		actual.first = listRooms.at(0).first;
		actual.second = listRooms.at(0).second;
		break;
	case '2':

		actual.first = listRooms.at(1).first;
		actual.second = listRooms.at(1).second;
		break;
	default:
		break;
	}
	
}

bool isObstacle(char c){
	if (isBonus(c) || isEnn(c) || isMarch(c) || isObject(c) || c == '1'){ 
		return true; 
	}
	return false;
}

void levelUp(Personnage& p){
	if (p.EXP >= p.EXPM){
		p.EXP = 0;
		p.EXPM += 15;
		p.HPM += 10;
		p.HP = p.HPM;
		p.LVL++;
		p.PTM += 5;
		p.PT_ACTION = p.PTM;
	}
}

char intToEnn(char c){
	switch (c)
	{
	case '2':
		return '@';
		break;
	case '3': 
		return '!';
		break;
	case '4':
		return '?';
		break;
	default:
		break;
	}
}

void initTabMove(vector<vector<bool>>& tabMove, speRoom& r_spe){
	tabMove.resize(r_spe.lenV);
	for (int i = 0; i < tabMove.size(); i++){
		tabMove.at(i).resize(r_spe.lenH);
	}
	for (int i = 0; i < tabMove.size(); i++){
		for (int j = 0; j < tabMove.at(i).size(); j++){
			tabMove.at(i).at(j) = false;
		}
	}
}

void moveEnn(vector<string>& room, speRoom& r_spe, pair<int, int>& pos,HANDLE& consoleHandle){
	char enn;
	vector<vector<bool>> tabMove;
	initTabMove(tabMove, r_spe);
	for (int i = 0; i < r_spe.lenV; i++){
		for (int j = 0; j < r_spe.lenH; j++){
			if (isEnn(room.at(i).at(j)) && tabMove.at(i).at(j) == false){
				enn = intToEnn(room.at(i).at(j));
				int dx, dy;
				dx = abs(j - pos.second);
				dy = abs(i - pos.first);
				if (dx >= dy ){
					if (j > pos.second){
						if (!isObstacle(room.at(i).at(j - 1))){
							room.at(i).at(j - 1) = room.at(i).at(j);
							room.at(i).at(j) = '.';
							if (help){
								gotoxy(j - 1, i + 12);
							}
							else{
								gotoxy(j - 1, i);
							}
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << enn << " ";
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
							tabMove.at(i).at(j-1) = true;
						}
					}
					else{
						if (!isObstacle(room.at(i).at(j + 1))){
							room.at(i).at(j + 1) = room.at(i).at(j);
							room.at(i).at(j) = '.';
							if (help){
								gotoxy(j, i + 12);
							}
							else{
								gotoxy(j, i);
							}
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << " " << enn;
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

							tabMove.at(i).at(j + 1) = true;
						}
					}
				}
				else if (dx < dy || tabMove.at(i).at(j) == false){
					if (i > pos.first){
						if (!isObstacle(room.at(i - 1).at(j) )){
							room.at(i - 1).at(j) = room.at(i).at(j);
							room.at(i).at(j) = '.';
							if (help){
								gotoxy(j, i + 11);
							}
							else{
								gotoxy(j, i - 1);
							}
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << enn;
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

							if (help){
								gotoxy(j, i + 12);
							}
							else{
								gotoxy(j, i);
							}
							cout << " ";
							tabMove.at(i- 1).at(j ) = true;
						}
					}
					else{
						if (!isObstacle(room.at(i + 1).at(j)) ){
							room.at(i + 1).at(j) = room.at(i).at(j);
							room.at(i).at(j) = '.';
							if (help){
								gotoxy(j, i + 13);
							}
							else{
								gotoxy(j, i + 1);
							}
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << enn << " ";
							SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

							if (help){
								gotoxy(j, i + 12);
							}
							else{
								gotoxy(j, i);
							}
							cout << " ";
							tabMove.at(i + 1).at(j) = true;
						}
					}
				}
			}
		}
	}
}

int main(){
	srand(time(NULL));
	int t = time(NULL);
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
	ios::sync_with_stdio(false);
	help = true;
	pair<int, int> position;
	pair<vector<string>*, speRoom*> actualRoom;
	actualRoom.first = &room1;
	actualRoom.second = &room1_spe;
	position.first = 5;
	position.second = 5;
	bool fin = false;
	inMarch = false;
	inInventory = false;
	char personnage = char(180); //193
	Personnage principal;
	principal.score = 0;
	principal.arm = SS_ARM;
	principal.objs.push_back(SS_OBJ);
	principal.HP = 25;
	principal.HPM = 25;
	principal.LVL = 1;
	principal.PT_ACTION = 5;
	principal.PTM = 5;
	principal.EXP = 0;
	principal.EXPM = 20;
	int selectionMarch = 1;
	int selectionInv = 1;

	pair<int, int> posDun;
	posDun.first = T_VM / 2;
	posDun.second = T_HM / 2;
	char dun[T_VM][T_HM];
	char map[T_VM][T_HM];
	initDunTest(map);
	initDunTest(dun);
	generateDunTest(true, dun, posDun);

	posDun.first = T_VM / 2;
	posDun.second = T_HM / 2;
	changeRoom(actualRoom, dun[posDun.first][posDun.second]);
	position.first = actualRoom.second->lenV / 2;
	position.second = actualRoom.second->lenH / 2;
	makeHole(*actualRoom.first, *actualRoom.second, dun, posDun);
	map[posDun.first][posDun.second] = dun[posDun.first][posDun.second];
	draw(position, personnage, *actualRoom.first, map, posDun);
	affStat(principal, *actualRoom.second);

	while (!fin)
	{
		if ((time(NULL) - t) >= 1){
			moveEnn(*actualRoom.first, *actualRoom.second, position, consoleHandle);
			//draw(position, personnage, *actualRoom.first, map, posDun);
			//affStat(principal, *actualRoom.second);
			t = time(NULL);
		}

		if (kbhit()){
			switch (_getch())
			{
			case 'e':
				if (inInventory){
					inInventory = false;
					selectionInv = 1;
					draw(position, personnage, *actualRoom.first, map, posDun);
					affStat(principal, *actualRoom.second);
				}
				else{
					affInventory(principal, selectionInv);
				}
				break;

			case 'd':
				if (!inMarch && !inInventory){
					personnage = 195;
					if (position.second == actualRoom.second->lenH - 1){
						removeHoles(*actualRoom.first, *actualRoom.second);
						removeEnns(*actualRoom.first, *actualRoom.second);
						removeBonus(*actualRoom.first, *actualRoom.second);
						posDun.second++;
						map[posDun.first][posDun.second] = dun[posDun.first][posDun.second];
						changeRoom(actualRoom, dun[posDun.first][posDun.second]);

						makeHole(*actualRoom.first, *actualRoom.second, dun, posDun);
						addEnn(*actualRoom.first, *actualRoom.second);
						addBonus(*actualRoom.first, *actualRoom.second);
						position.second = 0;
						position.first = actualRoom.second->lenV / 2;
						//makeHole(*actualRoom.first, *actualRoom.second, GAUCHE);
						draw(position, personnage, *actualRoom.first, map, posDun);

					}
					else if (actualRoom.first->at(position.first).at(position.second + 1) == '.'){
						position.second++;
						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << "\b " << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
						principal.HP -= nbEnnOnLines(position, *actualRoom.first, *actualRoom.second, principal);
					}
					else{
						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
					}
					affStat(principal, *actualRoom.second);
				}
				break;
			case 'q':
				if (!inMarch && !inInventory){
					personnage = 180;
					if (position.second == 0){
						removeHoles(*actualRoom.first, *actualRoom.second);
						removeEnns(*actualRoom.first, *actualRoom.second);
						removeBonus(*actualRoom.first, *actualRoom.second);
						posDun.second--;
						map[posDun.first][posDun.second] = dun[posDun.first][posDun.second];
						changeRoom(actualRoom, dun[posDun.first][posDun.second]);
						makeHole(*actualRoom.first, *actualRoom.second, dun, posDun);
						addEnn(*actualRoom.first, *actualRoom.second);
						addBonus(*actualRoom.first, *actualRoom.second);
						position.second = actualRoom.second->lenH - 1;
						position.first = actualRoom.second->lenV / 2;
						//makeHole(*actualRoom.first, *actualRoom.second, DROITE);
						draw(position, personnage, *actualRoom.first, map, posDun);
					}
					else if (actualRoom.first->at(position.first).at(position.second - 1) == '.'){
						position.second--;

						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage << " \b";
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

						principal.HP -= nbEnnOnLines(position, *actualRoom.first, *actualRoom.second, principal);
					}
					else{
						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
					}
					affStat(principal, *actualRoom.second);
				}
				break;
			case 's':
				if (inMarch){
					if (selectionMarch < 6){
						selectionMarch++;
						affMarchand(selectionMarch);
					}
				}
				else if (inInventory){
					if (selectionInv < principal.objs.size() - 1){
						selectionInv++;
						affInventory(principal, selectionInv);
					}
				}
				else if (!inMarch && !inInventory){
					personnage = 194;
					if (position.first == actualRoom.second->lenV - 1){
						removeHoles(*actualRoom.first, *actualRoom.second);
						removeEnns(*actualRoom.first, *actualRoom.second);
						removeBonus(*actualRoom.first, *actualRoom.second);
						posDun.first++;
						map[posDun.first][posDun.second] = dun[posDun.first][posDun.second];
						changeRoom(actualRoom, dun[posDun.first][posDun.second]);
						makeHole(*actualRoom.first, *actualRoom.second, dun, posDun);
						addEnn(*actualRoom.first, *actualRoom.second);
						addBonus(*actualRoom.first, *actualRoom.second);
						position.first = 0;
						position.second = ceil(actualRoom.second->lenH / 2.0);
						//makeHole(*actualRoom.first, *actualRoom.second, HAUT);
						draw(position, personnage, *actualRoom.first, map, posDun);
					}
					if (actualRoom.first->at(position.first + 1).at(position.second) == '.'){
						position.first++;

						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

						if (help){
							gotoxy(position.second, position.first + 11);
						}
						else{
							gotoxy(position.second, position.first - 1);
						}
						cout << " ";
						principal.HP -= nbEnnOnLines(position, *actualRoom.first, *actualRoom.second, principal);
					}
					else{
						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
					}
					affStat(principal, *actualRoom.second);
				}
				break;
			case 'z':
				if (inMarch){
					if (selectionMarch > 1){
						selectionMarch--;
						affMarchand(selectionMarch);
					}
				}
				else if (inInventory){
					if (selectionInv > 1){
						selectionInv--;
						affInventory(principal, selectionInv);
					}
				}
				else if (!inMarch && !inInventory){
					personnage = 193;
					if (position.first == 0){
						removeHoles(*actualRoom.first, *actualRoom.second);
						removeEnns(*actualRoom.first, *actualRoom.second);
						removeBonus(*actualRoom.first, *actualRoom.second);
						posDun.first--;
						map[posDun.first][posDun.second] = dun[posDun.first][posDun.second];
						changeRoom(actualRoom, dun[posDun.first][posDun.second]);
						makeHole(*actualRoom.first, *actualRoom.second, dun, posDun);
						addEnn(*actualRoom.first, *actualRoom.second);
						addBonus(*actualRoom.first, *actualRoom.second);
						position.first = actualRoom.second->lenV - 1;
						position.second = ceil(actualRoom.second->lenH / 2.0);
						//makeHole(*actualRoom.first, *actualRoom.second, BAS);
						draw(position, personnage, *actualRoom.first, map, posDun);
					}
					if (actualRoom.first->at(position.first - 1).at(position.second) == '.'){
						position.first--;

						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);

						if (help){
							gotoxy(position.second, position.first + 13);
						}
						else{
							gotoxy(position.second, position.first + 1);
						}
						cout << " ";

						principal.HP -= nbEnnOnLines(position, *actualRoom.first, *actualRoom.second, principal);
					}
					else{
						if (help){
							gotoxy(position.second, position.first + 12);
						}
						else{
							gotoxy(position.second, position.first);
						}
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << personnage;
						SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
					}
					affStat(principal, *actualRoom.second);
				}
				break;
			case 'h':
				if (!inInventory && !inMarch){
					help = !help;
					draw(position, personnage, *actualRoom.first, map, posDun);
					affStat(principal, *actualRoom.second);
				}
				break;
			case ' ':
				if (!(position.first == actualRoom.second->lenV - 1 || position.second == actualRoom.second->lenH - 1 || position.first == 0 || position.second == 0) && !inMarch && !inInventory){
					switch (personnage)
					{
					case char(180) ://gauche
						if (isEnn(actualRoom.first->at(position.first).at(position.second - 1)) && principal.PT_ACTION > 0){
							addPoint(actualRoom.first->at(position.first).at(position.second - 1), principal);
							actualRoom.first->at(position.first).at(position.second - 1) = '.';
							addObject(*actualRoom.first, *actualRoom.second);
							principal.PT_ACTION--;
							principal.EXP++;
							levelUp(principal);

							if (help){
								gotoxy(position.second - 1, position.first + 12);
							}
							else{
								gotoxy(position.second - 1, position.first);
							}
							cout << " ";
							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isBonus(actualRoom.first->at(position.first).at(position.second - 1))){
							switch (actualRoom.first->at(position.first).at(position.second - 1))
							{
							case 'o':
								if (principal.HP < principal.HPM){
									principal.HP += 2;
									if (principal.HP > principal.HPM){
										principal.HP = principal.HPM;
									}
								}
								break;
							case '-':
								if (principal.PT_ACTION < principal.PTM){
									principal.PT_ACTION++;
								}
								break;
							default:
								break;
							}
							actualRoom.first->at(position.first).at(position.second - 1) = '.';
							if (help){
								gotoxy(position.second - 1, position.first + 12);
							}
							else{
								gotoxy(position.second - 1, position.first);
							}
							cout << " ";
							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isMarch(actualRoom.first->at(position.first).at(position.second - 1))){
							affMarchand(selectionMarch);
						}
						else if (isObject(actualRoom.first->at(position.first).at(position.second - 1))){
							//ADD
							objectNewInInventory(principal);
							actualRoom.first->at(position.first).at(position.second - 1) = '.';
							if (help){
								gotoxy(position.second - 1, position.first + 12);
							}
							else{
								gotoxy(position.second - 1, position.first);
							}
							cout << " ";
							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						break;
					case char(193) ://haut
						if (isEnn(actualRoom.first->at(position.first - 1).at(position.second)) && principal.PT_ACTION > 0){
							addPoint(actualRoom.first->at(position.first - 1).at(position.second), principal);
							actualRoom.first->at(position.first - 1).at(position.second) = '.';
							addObject(*actualRoom.first, *actualRoom.second);
							principal.PT_ACTION--;
							principal.EXP++;
							levelUp(principal);

							if (help){
								gotoxy(position.second, position.first + 11);
							}
							else{
								gotoxy(position.second, position.first - 1);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isBonus(actualRoom.first->at(position.first - 1).at(position.second))){
							switch (actualRoom.first->at(position.first - 1).at(position.second))
							{
							case 'o':
								if (principal.HP < principal.HPM){
									principal.HP += 2;
									if (principal.HP > principal.HPM){
										principal.HP = principal.HPM;
									}
								}
								break;
							case '-':
								if (principal.PT_ACTION < principal.PTM){
									principal.PT_ACTION++;
								}
								break;
							default:
								break;
							}
							actualRoom.first->at(position.first - 1).at(position.second) = '.';
							if (help){
								gotoxy(position.second, position.first + 11);
							}
							else{
								gotoxy(position.second, position.first - 1);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isMarch(actualRoom.first->at(position.first - 1).at(position.second))){
							affMarchand(selectionMarch);
						}
						else if (isObject(actualRoom.first->at(position.first - 1).at(position.second))){
							//ADD
							objectNewInInventory(principal);
							actualRoom.first->at(position.first - 1).at(position.second) = '.';

							if (help){
								gotoxy(position.second, position.first + 11);
							}
							else{
								gotoxy(position.second, position.first - 1);
							}
							cout << " ";
							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						break;
					case char(194) ://bas
						if (isEnn(actualRoom.first->at(position.first + 1).at(position.second)) && principal.PT_ACTION > 0){
							addPoint(actualRoom.first->at(position.first + 1).at(position.second), principal);
							actualRoom.first->at(position.first + 1).at(position.second) = '.';
							addObject(*actualRoom.first, *actualRoom.second);
							principal.PT_ACTION--;
							principal.EXP++;
							levelUp(principal);

							if (help){
								gotoxy(position.second, position.first + 13);
							}
							else{
								gotoxy(position.second, position.first + 1);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isBonus(actualRoom.first->at(position.first + 1).at(position.second))){
							switch (actualRoom.first->at(position.first + 1).at(position.second))
							{
							case 'o':
								if (principal.HP < principal.HPM){
									principal.HP += 2;
									if (principal.HP > principal.HPM){
										principal.HP = principal.HPM;
									}
								}
								break;
							case '-':
								if (principal.PT_ACTION < principal.PTM){
									principal.PT_ACTION++;
								}
								break;
							default:
								break;
							}
							actualRoom.first->at(position.first + 1).at(position.second) = '.';

							if (help){
								gotoxy(position.second, position.first + 13);
							}
							else{
								gotoxy(position.second, position.first + 1);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isMarch(actualRoom.first->at(position.first + 1).at(position.second))){
							affMarchand(selectionMarch);
						}
						else if (isObject(actualRoom.first->at(position.first + 1).at(position.second))){
							//ADD
							objectNewInInventory(principal);
							actualRoom.first->at(position.first + 1).at(position.second) = '.';

							if (help){
								gotoxy(position.second, position.first + 13);
							}
							else{
								gotoxy(position.second, position.first + 1);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						break;
					case char(195) ://droite
						if (isEnn(actualRoom.first->at(position.first).at(position.second + 1)) && principal.PT_ACTION > 0){
							addPoint(actualRoom.first->at(position.first).at(position.second + 1), principal);
							actualRoom.first->at(position.first).at(position.second + 1) = '.';
							addObject(*actualRoom.first, *actualRoom.second);
							principal.PT_ACTION--;
							principal.EXP++;
							levelUp(principal);

							if (help){
								gotoxy(position.second + 1, position.first + 12);
							}
							else{
								gotoxy(position.second + 1, position.first);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isBonus(actualRoom.first->at(position.first).at(position.second + 1))){
							switch (actualRoom.first->at(position.first).at(position.second + 1))
							{
							case 'o':
								if (principal.HP < principal.HPM){
									principal.HP += 2;
									if (principal.HP > principal.HPM){
										principal.HP = principal.HPM;
									}
								}
								break;
							case '-':
								if (principal.PT_ACTION < principal.PTM){
									principal.PT_ACTION++;
								}
								break;
							default:
								break;
							}
							actualRoom.first->at(position.first).at(position.second + 1) = '.';

							if (help){
								gotoxy(position.second + 1, position.first + 12);
							}
							else{
								gotoxy(position.second + 1, position.first);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						else if (isMarch(actualRoom.first->at(position.first).at(position.second + 1))){
							affMarchand(selectionMarch);
						}
						else if (isObject(actualRoom.first->at(position.first).at(position.second + 1))){
							//ADD
							objectNewInInventory(principal);
							actualRoom.first->at(position.first).at(position.second + 1) = '.';

							if (help){
								gotoxy(position.second + 1, position.first + 12);
							}
							else{
								gotoxy(position.second + 1, position.first);
							}
							cout << " ";

							//draw(position, personnage, *actualRoom.first,map,posDun);
							affStat(principal, *actualRoom.second);
						}
						break;
					default:
						break;
					}
				}
				else if (inMarch){
					switch (selectionMarch)
					{
					case 1:
						if (principal.score >= 100){
							principal.score -= 100;
							principal.arm = ARM_BASE;
						}
						break;
					case 2:
						if (principal.score >= 500){
							principal.score -= 500;
							principal.arm = ARM_STD;
						}
						break;
					case 3:
						if (principal.score >= 1000){
							principal.score -= 1000;
							principal.arm = ARM_GUERR;
						}
						break;
					case 4:
						if (principal.score >= 8000){
							principal.score -= 8000;
							principal.arm = ARM_GUERI;
						}
						break;
					case 5:
						if (principal.score >= 20000){
							principal.score -= 20000;
							principal.arm = ARM_ULTI;
						}
						break;
					default:
						break;
					}
					selectionMarch = 1;
					draw(position, personnage, *actualRoom.first, map, posDun);
					affStat(principal, *actualRoom.second);
				}
				else if (inInventory){
					if (principal.objs.size() > 1){
						Objet temp = principal.objs.at(0);
						principal.objs.at(0) = principal.objs.at(selectionInv);
						principal.objs.at(selectionInv) = temp;
						affInventory(principal, selectionInv);
					}
				}
				break;
			case 'a':
				cout << '\a';
				break;
			default:
				break;
			}
		}
		

		if (principal.HP <= 0){
			fin = true;
			cout << "GAME OVER\t\tp pour quitter" << endl;
		}
		if (principal.arm == ARM_ULTI){
			fin = true;
			cout << "BRAVO !!! Vous etes invincible\n\tp pour quitter" << endl;
		}
	}



	while (_getch() != 'p')
	{

	}

#ifdef AFF_TABLE
	for (int i = 0; i < N_MAX; i++){
		cout << i << " : " << char(i) << "\t";

	}
#endif
	system("PAUSE");
	return 0;
}
