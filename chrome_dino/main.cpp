// C++을 이용한 chrome dino 게임 구현.
#include<SFML/Graphics.hpp>
#include<iostream>
#include<time.h>
using namespace sf;
using namespace std;
#define WIDTH 1024   // 윈도우창 가로 길이
#define HEIGHT 576 //  윈도우창 세로 길이

struct Position
{
	int x;
	int y;
};

int main(void)
{
	// SFML 윈도우을 열어주고, title을 정함.
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "chrome//dino(control : spacebar or up/down or mouse left click)");
	window.setFramerateLimit(60);	//프레임 정해주기

	// dino 상태에 따른 이미지 받음. 상태별 동작 이미지 제어는 line 155 참고
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("images/dino_step_left.png");	//실제 그림을 받아 옵니다.
	t2.loadFromFile("images/dino_step_right.png");
	t3.loadFromFile("images/dino_jump.png");
	t4.loadFromFile("images/dino_gameover.png");
	t5.loadFromFile("images/dino_sneak_left.png");
	t6.loadFromFile("images/dino_sneak_right.png");

	// 스프라이트로 만들어줌
	Sprite dinoArr[7];       // sprite 배열 texture 6개를 받음.
	dinoArr[1].setTexture(t1); dinoArr[2].setTexture(t2); dinoArr[3].setTexture(t3);
	dinoArr[4].setTexture(t4); dinoArr[5].setTexture(t5); dinoArr[6].setTexture(t6);

	// frame 스피드로 공룡의 동작 상태 제어해줌. line 155 참고
	int index = 0;
	float frame = 0.f;
	float frameSpeed = 0.4f;
	const int changeCount = 3;	//몇 프레임에 동작 변경할지 정함.

	static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;	// 공룡 바닥위치 y좌표.

	Position dinoPos;
	dinoPos.x = 50;					// 공룡 기본위치 x좌표.
	dinoPos.y = DINO_Y_BOTTOM;		// 공룡 기본위치 y좌표.

	const int gravity = 10;	//중력. 점프할때 사용
	bool isJumping = false;	//점프 중인지 여부 나타냄. FALSE 시 바닥 상태, TRUE 시 점프 상태.
	bool isBottom = true;	//바닥에 발이 닿았는지

	// tree
	Texture a0, a1, a2, a3;
	a0.loadFromFile("images/tree0.png"); a1.loadFromFile("images/tree1.png"); a2.loadFromFile("images/tree2.png"); a3.loadFromFile("images/tree3.png");
	Sprite tree[4];
	tree[0].setTexture(a0); tree[1].setTexture(a1); tree[2].setTexture(a2); tree[3].setTexture(a3);
	int tree_index = 0;

	static const int TREE_Y_BOTTOM = HEIGHT - a0.getSize().y;	// 나무의 y좌표.
	Position treePos;
	treePos.x = WIDTH - 20;  // 나무의 x좌표.
	treePos.y = TREE_Y_BOTTOM;

	const int treeSpeed = 12;	//나무 스피드

	int score = 0;
	int score_highest = 0;

	sf::Font font;
sf:Text GAMEOVER;  // gameover 시 나오는 텍스트
	GAMEOVER.setString("GAME OVER");
	font.loadFromFile("arial.ttf");
	GAMEOVER.setFont(font);
	GAMEOVER.setCharacterSize(50);
	GAMEOVER.setFillColor(sf::Color::Red);
	GAMEOVER.setPosition(350, 180);

	sf::Text score_text;		// score 텍스트
	score_text.setFont(font);
	score_text.setCharacterSize(50);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(50, 0);

	sf::Text score_high_text;		// highest score 텍스트
	score_high_text.setString("highest score : " + to_string(score_highest));
	score_high_text.setFont(font);
	score_high_text.setCharacterSize(50);
	score_high_text.setFillColor(sf::Color::Black);
	score_high_text.setPosition(50, 50);

	while (1) {
		score = 0; float time_count = 0;
		score_text.setString("score : " + to_string(score));

		sf::Clock clk; // 시간 측정용 클래스 Clocksf::Clock 
		while (window.isOpen())		//윈도우가 열렸으면
		{
			// score(점수) 구함.
			if (time_count >= 1) { score += time_count; time_count = 0; }
			score_text.setString("score : " + to_string(score));
			time_count += clk.restart().asSeconds();

			Event e;
			while (window.pollEvent(e))	//윈도우 이벤트를 받는데
			{
				if (e.type == Event::Closed)	// 윈도우창 닫기 버튼 클릭 이벤트 윈도우 창을 닫는다.
				{
					window.close();
				}
			}
			//logic.

			//dino jump상태여부 확인.
			if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Up) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{	// 스페이스바, 위쪽 방향키, 또는 마우스 좌클릭 입력 감지
				if (isBottom && !isJumping)	//바닥이고, 점프중이 아닐때 점프 가능
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
					dinoPos.y -= 0.2 * gravity;			// 높이별 속도 설정해 점점 올라가는 속도 줄도록 함.
				}
				else {
					isJumping = false;
				}
			}
			else { // 점프해 200만큼 뛰면 내려오도록 설정.
				dinoPos.y += gravity;
			}

			//dino jump limit, dino bottom limit.
			if (dinoPos.y >= DINO_Y_BOTTOM)	//바닥 아래로 떨어지지 않게함.
			{
				dinoPos.y = DINO_Y_BOTTOM;
				isBottom = true;
			}


			//dino 동작별 이미지 설정.
			frame += frameSpeed;
			if (Keyboard::isKeyPressed(Keyboard::Down) && (dinoPos.y == DINO_Y_BOTTOM)) {
				if (frame >= changeCount)  // 공룡 발바꿔주는 작업.
				{
					frame -= changeCount;
					if (index == 5) index = 6;
					else index = 5;
				}
				dinoPos.y = HEIGHT - t5.getSize().y;
			}
			else if (dinoPos.y < DINO_Y_BOTTOM) index = 3;
			else if (frame > changeCount)  // 공룡 발바꿔주는 작업.
			{
				frame -= changeCount;
				if (index == 1) index = 2;
				else index = 1;
			}

			//tree move.
			if (treePos.x <= -50)     // 나무가 안보이는 좌표에 도달 시, 랜덤으로 나무 선택해 다시 오른쪽 끝으로 이동시켜 나오도록 함.
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
				window.draw(GAMEOVER); // gameover text 표시
				window.display();

				treePos.x = WIDTH;
				while (1) {  // 스페이스바, 엔터, 또는 마우스 좌클릭 입력 받아 게임을 재시작할 때까지 멈춤.
					while (window.pollEvent(e))
					{
						if (e.type == Event::Closed)	// 혹은 윈도우창 닫기 버튼 클릭 이벤트로 게임을 종료한다.
						{
							window.close();
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Space) || (Keyboard::isKeyPressed(Keyboard::Return)) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) break;
				}
				break;
			}
			// game over 되지 않은 경우 게임을 그대로 진행함.
			window.draw(tree[tree_index]);
			window.draw(dinoArr[index]);
			window.draw(score_text);
			window.draw(score_high_text);
			window.display();
		}
	}
	return 0;
}