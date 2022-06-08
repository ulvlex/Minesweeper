#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>   // ��� SetConsoleCP()
#include <math.h>
using namespace std;
using namespace sf;
#define RUS 1251
#define texture_side 32 //������� ������ ���������� �������� 32x32 �������
#define num_sq 12 //���������� ����������� � ��������
#define win 1
#define lose -1
#define open_cell 1
#define close_cell 0
#define flag_cell 3
#define width_win_lose 108
#define height_win_lose 34
#define dop_width_win 200
#define pos_x_win_lose 46
#define pos_y_win_lose 40

int arr_check[625]; //������ ������������� �������, ���������� ���� ������� ���������� (������������ �������� ����)
int arr_check_open[625]; // ������, ���������� ���������� � ���, ������� �� ������ ��� ���
const int empty_sq = 0; //����� ������� ����������
//������ ��������, ���������� �����
const int one = 1;
const int two = 2;
const int three = 3;
const int four = 4;
const int five = 5;
const int six = 6;
const int seven = 7;
const int eight = 8;
const int sp_mine = 9; //����� ������� �����
const int sq = 10; //����� ��������� ����������
const int flag = 11; //����� ������� ������

//���������� ��� �������� �������� ���� � ���-�� ����
const int width_beginner = 9; const int height_beginner = 9; const int mine_beginner = 10; //�������
const int width_amateur = 16; const int height_amateur = 16; const int mine_amateur = 40; //��������
const int width_professional = 25; const int height_professional = 25; const int mine_professional = 99; //������������

void mine(int width_sq, int height_sq, int num_mine) { //������� ������������ ���� � �������� ����� ������� ����
	int cnt_mine = 0;
	int position_mine;
	while (cnt_mine < num_mine) {
		position_mine = rand() % (width_sq * height_sq) + 1;
		if (arr_check[position_mine] != sp_mine) { 
			arr_check[position_mine] = sp_mine; //����������� �����
			cnt_mine++; //����������� ���-�� ���������� ����
			
		}
	}

	int curr_cnt_mine = 0; //���������� ���-�� ����� ������� ���� 
	for (int i = 0; i < width_sq * height_sq; i++) { //����������, ����� ���-�� ���� ����� ����� � ��� ��� ���� �������
		if (arr_check[i] != sp_mine) {
			
			if (i - width_sq >= 0) { //������� ������
				if (arr_check[i - width_sq] == sp_mine) curr_cnt_mine++;
				if (i - width_sq - 1 > 0 and arr_check[i - width_sq - 1] == sp_mine and (i - width_sq - 1) % width_sq != width_sq-1) curr_cnt_mine++;
				if (i - width_sq + 1 < width_sq*height_sq and arr_check[i - width_sq + 1] == sp_mine and (i - width_sq + 1) % width_sq != 0) curr_cnt_mine++;
			}
			if (i + width_sq < width_sq*height_sq) { //������ ������
				if (arr_check[i + width_sq] == sp_mine) curr_cnt_mine++;
				if (i + width_sq - 1 > 0 and arr_check[i + width_sq - 1] == sp_mine and (i + width_sq - 1)%width_sq !=  width_sq - 1) curr_cnt_mine++;
				if (i + width_sq + 1 < width_sq * height_sq and arr_check[i + width_sq + 1] == sp_mine and (i + width_sq + 1)%width_sq != 0) curr_cnt_mine++;
			}
			if (i - 1 > 0 and (i - 1)%width_sq != width_sq - 1) { //����� ������
				if (arr_check[i - 1] == sp_mine) curr_cnt_mine++;
			}
			if (i + 1 < width_sq * height_sq and (i + 1)%width_sq != 0) { //������ ������
				if (arr_check[i + 1] == sp_mine) curr_cnt_mine++;
			}
			arr_check[i] = curr_cnt_mine;
			curr_cnt_mine = 0; 
		}
	}
}

void empty_cell(int width_sq, int height_sq, int index) { //������� �������� ������ ������ 
	arr_check_open[index] = open_cell;
	if (index - width_sq >= 0 and arr_check[index] == empty_sq) { //������� ������
		if (arr_check[index - width_sq] != sp_mine and arr_check_open[index - width_sq] == close_cell) { //���� �� ����� � �� �������
			if (arr_check[index - width_sq] >= one and arr_check[index - width_sq] <= eight) //���� ����� 
				arr_check_open[index - width_sq] = open_cell;
			else {
				if (arr_check[index - width_sq] == empty_sq and arr_check_open[index - width_sq] == close_cell) //���� ������ ������ � ��� ��� �� �������
					arr_check_open[index - width_sq] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index - width_sq);
			}
		}
		if (index - width_sq - 1 > 0 and (index - width_sq - 1) % width_sq != width_sq - 1 and arr_check[index - width_sq - 1] != sp_mine and arr_check_open[index - width_sq - 1] == close_cell) { //���� �� ����� � �� �������
			if (arr_check[index - width_sq - 1] >= one and arr_check[index - width_sq - 1] <= eight) //���� ����� 
				arr_check_open[index - width_sq - 1] = open_cell;
			else {
				if (arr_check[index - width_sq - 1] == empty_sq) //���� ������ ������
					arr_check_open[index - width_sq - 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index - width_sq - 1);
			}
		}
		if (arr_check[index - width_sq + 1] != sp_mine and index - width_sq + 1 < width_sq * height_sq and arr_check_open[index - width_sq + 1] == close_cell and (index - width_sq + 1) % width_sq != 0) { //���� �� ����� � �� �������
			if (arr_check[index - width_sq + 1] >= one and arr_check[index - width_sq + 1] <= eight) //���� ����� 
				arr_check_open[index - width_sq + 1] = open_cell;
			else {
				if (arr_check[index - width_sq + 1] == empty_sq) //���� ������ ������
					arr_check_open[index - width_sq + 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index - width_sq + 1);
			}
		}
	}
	if (index + width_sq < width_sq * height_sq and arr_check[index] == empty_sq) { //������ ������
		if (arr_check[index + width_sq] != sp_mine and arr_check_open[index + width_sq] == close_cell) { //���� �� ����� � �� �������
			if (arr_check[index + width_sq] >= one and arr_check[index + width_sq] <= eight) //���� ����� 
				arr_check_open[index + width_sq] = open_cell;
			else {
				if (arr_check[index + width_sq] == empty_sq) //���� ������ ������
					arr_check_open[index + width_sq] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index + width_sq);
			}
		}
		if (arr_check[index + width_sq - 1] != sp_mine and index + width_sq - 1 > 0 and (index + width_sq - 1) % width_sq != width_sq - 1 and arr_check_open[index + width_sq - 1] == close_cell) { //���� �� ����� � �� �������
			if (arr_check[index + width_sq - 1] >= one and arr_check[index + width_sq - 1] <= eight) //���� ����� 
				arr_check_open[index + width_sq - 1] = open_cell;
			else {
				if (arr_check[index + width_sq - 1] == empty_sq) //���� ������ ������
					arr_check_open[index + width_sq - 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index + width_sq - 1);
			}
		}
		if (arr_check[index + width_sq + 1] != sp_mine and index + width_sq + 1 < width_sq * height_sq and (index + width_sq + 1) % width_sq != 0 and arr_check_open[index + width_sq + 1] == close_cell) { //���� �� ����� � �� �������
			if (arr_check[index + width_sq + 1] >= one and arr_check[index + width_sq + 1] <= eight) //���� ����� 
				arr_check_open[index + width_sq + 1] = open_cell;
			else {
				if (arr_check[index + width_sq + 1] == empty_sq) //���� ������ ������
					arr_check_open[index + width_sq + 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index + width_sq + 1);
			}
		}
	}
	if (index - 1 > 0 and (index - 1) % width_sq != width_sq - 1 and arr_check[index] == empty_sq) { //����� ������
		if (arr_check[index - 1] != sp_mine and arr_check_open[index - 1] == close_cell) //���� �� ����� � �� �������
		{
			if (arr_check[index - 1] >= one and arr_check[index - 1] <= eight) //���� ����� 
				arr_check_open[index - 1] = open_cell;
			else {
				if (arr_check[index - 1] == empty_sq) //���� ������ ������
					arr_check_open[index - 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index - 1);
			}
		}
	}
	if (index + 1 < width_sq * height_sq and (index + 1) % width_sq != 0 and arr_check[index] == empty_sq) { //������ ������
		if (arr_check[index + 1] != sp_mine and arr_check_open[index + 1] == close_cell) //���� �� ����� � �� �������
		{
			if (arr_check[index + 1] >= one and arr_check[index + 1] <= eight) //���� ����� 
				arr_check_open[index + 1] = open_cell;
			else {
				if (arr_check[index + 1] == empty_sq) //���� ������ ������
					arr_check_open[index + 1] = open_cell; //��������� ������
				empty_cell(width_sq, height_sq, index + 1);
			}
		}	
	}
}

void minesweeper(int width_sq, int height_sq, int num_mine) {
	int height; int width; //������� �������� ���� � ��������
	int win_lose = 0; //���� = 0, ������ = 1, �������� = -1
	int cnt_flag = 0; //������� ���-�� ��������� ������������ ������

	//������� ������� �������� ����
	width = width_sq * texture_side;
	height = height_sq * texture_side;

	//�������� �������� � �������
	Texture texture;
	texture.loadFromFile("C:\\Users\\ASUS\\Desktop\\�������\\����������������\\2 ���\\��������\\�������.jpg"); //�������� ��������
	Sprite sprite[num_sq]; //������ ��������
	for (int i = 0; i < num_sq; i++) { //���� �������� ������� ��������
		sprite[i].setTexture(texture);
		sprite[i].setTextureRect(IntRect(i * texture_side, 0, texture_side, texture_side)); //�������� ������ ��������� �� ��������
	}

	//������� ��������� � ������
	Texture texture1;
	texture1.loadFromFile("C:\\Users\\ASUS\\Desktop\\�������\\����������������\\2 ���\\��������\\Im.jpg"); //�������� ��������
	Sprite sprite_win_lose[2]; //������ ��������
	sprite_win_lose[0].setTexture(texture1); //��������
	sprite_win_lose[0].setTextureRect(IntRect(0, 0, width_win_lose, height_win_lose));
	sprite_win_lose[1].setTexture(texture1); //�������
	sprite_win_lose[1].setTextureRect(IntRect(0, height_win_lose, width_win_lose, height_win_lose));

	RenderWindow window(VideoMode(width + dop_width_win, height), L"Minesweeper"); // ������� ���� 
	while (window.isOpen()) //���� ������� ����
	{
		Event event;
		while (window.pollEvent(event)) {//��������� ������� ����
			if (event.type == Event::Closed) window.close(); //��������� ����, ���� ��� "������ ��������"

			window.clear(Color::White); //����� ���� ����

			int index; //������ ������� arr_check;
			

			for (int j = 0; j < height_sq; j++) { //���� �������� � ���� ����� ��������
				for (int i = 0; i < width_sq; i++) {
					if (arr_check_open[i * width_sq + j] == open_cell) {
						sprite[arr_check[i * width_sq + j]].setPosition(j * texture_side, i * texture_side); //������������� ������ � ������ �������
						window.draw(sprite[arr_check[i * width_sq + j]]); //������ ������
						if (win_lose == lose) { //���� ��������
							sprite_win_lose[0].setPosition(width + pos_x_win_lose, pos_y_win_lose);
							window.draw(sprite_win_lose[0]);
						}
						else if (win_lose == win) { //���� ������
							sprite_win_lose[1].setPosition(width + pos_x_win_lose, pos_y_win_lose);
							window.draw(sprite_win_lose[1]);
						}
					}
					else if(arr_check_open[i * width_sq + j] == flag_cell){ //����� ����
						sprite[flag].setPosition(j * texture_side, i * texture_side); //������������� ������ ����� � ������ �������
						window.draw(sprite[flag]); //������ ������
					}
					else {
						sprite[sq].setPosition(j * texture_side, i * texture_side); //������������� ������ � ������ �������
						window.draw(sprite[sq]); //������ ������
					}
				}
			}

			if (event.type == Event::MouseButtonPressed) { //������� ������ 
				if (event.mouseButton.button == Mouse::Left and event.mouseButton.x < width) { //���� ������ ����� ������ �����, �� ������ ����������� ������
					index = (event.mouseButton.x / texture_side) + (event.mouseButton.y / texture_side) * width_sq;
					if (arr_check_open[index] == close_cell) { //�������� �� ��, ���� �� ������� ������ 

						if (arr_check[index] == sp_mine) { //���� ��������� ������ - �����, �.�. ��������
							win_lose = lose;
							for (int i = 0; i < width_sq; i++) { //��������� ��� ������
								for (int j = 0; j < height_sq; j++) {
									arr_check_open[i * width_sq + j] = open_cell; //��������� �� ����
								}
							}
						}
						else {
							if (arr_check[index] == empty_sq) {//���� ��������� ������ - ������
								empty_cell(width_sq, height_sq, index);
							}
							else { //���� ��������� ������ - �����
								arr_check_open[index] = open_cell;
							}
						}
					}
				}
				if (event.mouseButton.button == Mouse::Right and event.mouseButton.x < width) { //���� ������ ������ ������ �����, �� �������� ������
					index = (event.mouseButton.x / texture_side) + (event.mouseButton.y / texture_side) * width_sq;
					if (arr_check_open[index] == close_cell) { //�������� �� ��, ������� �� ��� ������
						arr_check_open[index] = flag_cell;//������������ ���������� �����
						if (arr_check[index] == sp_mine) {
							cnt_flag+=1;
							if (cnt_flag == num_mine) { //������
								win_lose = win;
								for (int i = 0; i < width_sq; i++) { //��������� ��� ������
									for (int j = 0; j < height_sq; j++) {
										arr_check_open[i * width_sq + j] = open_cell; //��������� �� ����
									}
								}
							}
						}
					}
				}
			}
			window.display(); //�� ������� � �����
		}
	}
}

int main()
{
	SetConsoleCP(RUS);			    // ��������� ������� ����� � ������ �����
	SetConsoleOutputCP(RUS);		// ��������� ������� ����� � ������ ������
	srand(time(NULL)); //��� ��������� ��������� ����� 

	int selecting_mode; // ���������� ������ ������ ����
	int width_sq; int height_sq; // ������ � ������ �������� ���� 
	int num_mine; //���-�� ��� �� ������� ����
	bool error_selecting_mode; //�������� �� ��, ��������� �� ��� ������ �����

	cout << "�������� ����� ����:\n<1> �������\n<2> ��������\n<3> ������������" << endl;
	cin >> selecting_mode;
	//����� ������ ����
	switch (selecting_mode) {
	case 1:
		width_sq = width_beginner; height_sq = height_beginner; //����� ������� �������� ���� � ����������� (�� 32x32)
		num_mine = mine_beginner; //����� ���-�� ���
		error_selecting_mode = false;
		break;
	case 2:
		width_sq = width_amateur; height_sq = height_amateur; //����� ������� �������� ���� � ����������� (�� 32x32)
		num_mine = mine_amateur; //����� ���-�� ���
		error_selecting_mode = false;
		break;
	case 3:
		width_sq = width_professional; height_sq = height_professional; //����� ������� �������� ���� � ����������� (�� 32x32)
		num_mine = mine_professional; //����� ���-�� ���
		error_selecting_mode = false;
		break;
	default:
		error_selecting_mode = true;
		break;
	}

	if (error_selecting_mode == false) {
		mine(width_sq, height_sq, num_mine);
		minesweeper(width_sq, height_sq, num_mine);
	}
	return 0;
}