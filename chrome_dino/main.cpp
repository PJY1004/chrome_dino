// C++�� �̿��� chrome dino ���� ����.
#include<SFML/Graphics.hpp>
#include<iostream>
#include<time.h>
using namespace sf;
using namespace std;
#define WIDTH 1024   // ������â ���� ����
#define HEIGHT 576 //  ������â ���� ����

struct Position
{
	int x;
	int y;
};

int main(void)
{
	// SFML �������� �����ְ�, title�� ����.
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "chrome//dino(control : spacebar or up/down or mouse left click)");
	window.setFramerateLimit(60);	//������ �����ֱ�

	// dino ���¿� ���� �̹��� ����. ���º� ���� �̹��� ����� line 155 ����
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("images/dino_step_left.png");	//���� �׸��� �޾� �ɴϴ�.
	t2.loadFromFile("images/dino_step_right.png");
	t3.loadFromFile("images/dino_jump.png");
	t4.loadFromFile("images/dino_gameover.png");
	t5.loadFromFile("images/dino_sneak_left.png");
	t6.loadFromFile("images/dino_sneak_right.png");

	// ��������Ʈ�� �������
	Sprite dinoArr[7];       // sprite �迭 texture 6���� ����.
	dinoArr[1].setTexture(t1); dinoArr[2].setTexture(t2); dinoArr[3].setTexture(t3);
	dinoArr[4].setTexture(t4); dinoArr[5].setTexture(t5); dinoArr[6].setTexture(t6);

	// frame ���ǵ�� ������ ���� ���� ��������. line 155 ����
	int index = 0;
	float frame = 0.f;
	float frameSpeed = 0.4f;
	const int changeCount = 3;	//�� �����ӿ� ���� �������� ����.

	static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;	// ���� �ٴ���ġ y��ǥ.

	Position dinoPos;
	dinoPos.x = 50;					// ���� �⺻��ġ x��ǥ.
	dinoPos.y = DINO_Y_BOTTOM;		// ���� �⺻��ġ y��ǥ.

	const int gravity = 10;	//�߷�. �����Ҷ� ���
	bool isJumping = false;	//���� ������ ���� ��Ÿ��. FALSE �� �ٴ� ����, TRUE �� ���� ����.
	bool isBottom = true;	//�ٴڿ� ���� ��Ҵ���

	// tree
	Texture a0, a1, a2, a3;
	a0.loadFromFile("images/tree0.png"); a1.loadFromFile("images/tree1.png"); a2.loadFromFile("images/tree2.png"); a3.loadFromFile("images/tree3.png");
	Sprite tree[4];
	tree[0].setTexture(a0); tree[1].setTexture(a1); tree[2].setTexture(a2); tree[3].setTexture(a3);
	int tree_index = 0;

	static const int TREE_Y_BOTTOM = HEIGHT - a0.getSize().y;	// ������ y��ǥ.
	Position treePos;
	treePos.x = WIDTH - 20;  // ������ x��ǥ.
	treePos.y = TREE_Y_BOTTOM;

	const int treeSpeed = 12;	//���� ���ǵ�

	int score = 0;
	int score_highest = 0;

	sf::Font font;
sf:Text GAMEOVER;  // gameover �� ������ �ؽ�Ʈ
	GAMEOVER.setString("GAME OVER");
	font.loadFromFile("arial.ttf");
	GAMEOVER.setFont(font);
	GAMEOVER.setCharacterSize(50);
	GAMEOVER.setFillColor(sf::Color::Red);
	GAMEOVER.setPosition(350, 180);

	sf::Text score_text;		// score �ؽ�Ʈ
	score_text.setFont(font);
	score_text.setCharacterSize(50);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(50, 0);

	sf::Text score_high_text;		// highest score �ؽ�Ʈ
	score_high_text.setString("highest score : " + to_string(score_highest));
	score_high_text.setFont(font);
	score_high_text.setCharacterSize(50);
	score_high_text.setFillColor(sf::Color::Black);
	score_high_text.setPosition(50, 50);

	while (1) {
		score = 0; float time_count = 0;
		score_text.setString("score : " + to_string(score));

		sf::Clock clk; // �ð� ������ Ŭ���� Clocksf::Clock 
		while (window.isOpen())		//�����찡 ��������
		{
			// score(����) ����.
			if (time_count >= 1) { score += time_count; time_count = 0; }
			score_text.setString("score : " + to_string(score));
			time_count += clk.restart().asSeconds();

			Event e;
			while (window.pollEvent(e))	//������ �̺�Ʈ�� �޴µ�
			{
				if (e.type == Event::Closed)	// ������â �ݱ� ��ư Ŭ�� �̺�Ʈ ������ â�� �ݴ´�.
				{
					window.close();
				}
			}
			//logic.

			//dino jump���¿��� Ȯ��.
			if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Up) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{	// �����̽���, ���� ����Ű, �Ǵ� ���콺 ��Ŭ�� �Է� ����
				if (isBottom && !isJumping)	//�ٴ��̰�, �������� �ƴҶ� ���� ����
				{
					//make jumping stage;
					isJumping = true;
					isBottom = false;
				}
			}

			//dino jump(up and down)
			if (isJumping)
			{
				if (dinoPos.y >= DINO_Y_BOTTOM - 70)
				{
					dinoPos.y -= 1.5 * gravity;
				}
				else if (dinoPos.y >= DINO_Y_BOTTOM - 190)
				{
					dinoPos.y -= gravity;
				}
				else if (dinoPos.y >= DINO_Y_BOTTOM - 200)
				{
					dinoPos.y -= 0.2 * gravity;			// ���̺� �ӵ� ������ ���� �ö󰡴� �ӵ� �ٵ��� ��.
				}
				else {
					isJumping = false;
				}
			}
			else { // ������ 200��ŭ �ٸ� ���������� ����.
				dinoPos.y += gravity;
			}

			//dino jump limit, dino bottom limit.
			if (dinoPos.y >= DINO_Y_BOTTOM)	//�ٴ� �Ʒ��� �������� �ʰ���.
			{
				dinoPos.y = DINO_Y_BOTTOM;
				isBottom = true;
			}


			//dino ���ۺ� �̹��� ����.
			frame += frameSpeed;
			if (Keyboard::isKeyPressed(Keyboard::Down) && (dinoPos.y == DINO_Y_BOTTOM)) {
				if (frame >= changeCount)  // ���� �߹ٲ��ִ� �۾�.
				{
					frame -= changeCount;
					if (index == 5) index = 6;
					else index = 5;
				}
				dinoPos.y = HEIGHT - t5.getSize().y;
			}
			else if (dinoPos.y < DINO_Y_BOTTOM) index = 3;
			else if (frame > changeCount)  // ���� �߹ٲ��ִ� �۾�.
			{
				frame -= changeCount;
				if (index == 1) index = 2;
				else index = 1;
			}

			//tree move.
			if (treePos.x <= -50)     // ������ �Ⱥ��̴� ��ǥ�� ���� ��, �������� ���� ������ �ٽ� ������ ������ �̵����� �������� ��.
			{
				srand((unsigned)time(NULL));
				tree_index = rand() % 4;
				treePos.x = WIDTH;
			}
			else
			{
				treePos.x -= treeSpeed;
			}

			//dino Position.
			dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

			//tree Position.
			tree[tree_index].setPosition(treePos.x, treePos.y);

			//draw.
			window.clear(Color::White);

			//game over
			if (dinoArr[index].getGlobalBounds().intersects(tree[tree_index].getGlobalBounds())) {
				score_highest = max(score, score_highest);
				score_high_text.setString("highest score : " + to_string(score_highest));
				index = 4;
				dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

				window.draw(tree[tree_index]);
				window.draw(dinoArr[index]);
				window.draw(score_text);
				window.draw(score_high_text);
				window.draw(GAMEOVER); // gameover text ǥ��
				window.display();

				treePos.x = WIDTH;
				while (1) {  // �����̽���, ����, �Ǵ� ���콺 ��Ŭ�� �Է� �޾� ������ ������� ������ ����.
					while (window.pollEvent(e))
					{
						if (e.type == Event::Closed)	// Ȥ�� ������â �ݱ� ��ư Ŭ�� �̺�Ʈ�� ������ �����Ѵ�.
						{
							window.close();
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Space) || (Keyboard::isKeyPressed(Keyboard::Return)) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) break;
				}
				break;
			}
			// game over ���� ���� ��� ������ �״�� ������.
			window.draw(tree[tree_index]);
			window.draw(dinoArr[index]);
			window.draw(score_text);
			window.draw(score_high_text);
			window.display();
		}
	}
	return 0;
}