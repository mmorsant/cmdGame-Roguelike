#include <iostream>
#include <cstdlib>
#include <vector>

namespace GRID
{
	constexpr int width{ 20 };
	constexpr int height{ 10 };
}

namespace PLAYER
{
	int positionX{ 1 };
	int positionY{ 1 };
}

namespace TILE
{
	constexpr unsigned char player = 64;
	constexpr unsigned char floor = 176;
	constexpr unsigned char wall = 219;
}

namespace ENEMY
{
	struct enemy
	{
		int id;
		unsigned char tile;
		int posY;
		int posX;
	};

	enemy swordsman{ 501, 159, 0, 0 };
	enemy knight{ 502, 190, 0, 0 };
}

class GameSystem
{
private:
	unsigned char** m_grid;
	bool m_firstRun;
	bool m_secondRun;
	std::vector<ENEMY::enemy> m_enemies;

public:
	//Allocate memory actions for next functions, initialize all the resources.
	GameSystem() : m_firstRun{ true }, m_secondRun{ true }
	{
		m_grid = new unsigned char*[GRID::height];			//rounds
		for (int a{ 0 }; a < (GRID::height); a++)			//columns
		{
			m_grid[a] = new unsigned char [GRID::width];
		}

		m_enemies.reserve(2);
	}

	//Set tiles in the grid for floors, walls, players and enemies.
	void init()
	{
		//set floor all grid
		for (int a{ 0 }; a < (GRID::height); a++)			//rounds
		{
			for (int b{ 0 }; b < (GRID::width); b++)		//columns
			{
				m_grid[a][b] = TILE::floor;
			}
		}

		//make default walls
		for (int a{ 0 }; a < (GRID::width); a++)				//superior
		{
			m_grid[0][a] = TILE::wall;
		}
		for (int a{ 0 }; a < (GRID::height); a++)				//left
		{
			m_grid[a][0] = TILE::wall;
		}
		for (int a{ 0 }; a < (GRID::height); a++)				//right
		{
			m_grid[a][GRID::width - 1] = TILE::wall;
		}
		for (int a{ 0 }; a < (GRID::width); a++)				//inferior
		{
			m_grid[GRID::height - 1][a] = TILE::wall;
		}
		m_grid[4][4] = TILE::wall;

		//spawn enemies
		spawnEnemy(6, 3);
		spawnEnemy(6, 16);

		//set player position
		m_grid[PLAYER::positionY][PLAYER::positionX] = TILE::player;
	}

	//Update a frame in the game status, manage the player and enemies movement while draw in the console.
	bool state(unsigned short& input)
	{
		// > to second state put floor on the player was
		if (m_firstRun == true)
			m_firstRun = false;
		else
			m_grid[PLAYER::positionY][PLAYER::positionX] = TILE::floor;

		//movement system
		switch (input)
		{
		case 4:
			//move left
			if (!(collision(PLAYER::positionY, PLAYER::positionX, 4, TILE::wall)))
				PLAYER::positionX--;
			break;
		case 6:
			//move right
			if (!(collision(PLAYER::positionY, PLAYER::positionX, 6, TILE::wall)))
				PLAYER::positionX++;
			break;
		case 8:
			//move up
			if (!(collision(PLAYER::positionY, PLAYER::positionX, 8, TILE::wall)))
				PLAYER::positionY--;
			break;
		case 2:
			//move down
			if (!(collision(PLAYER::positionY, PLAYER::positionX, 2, TILE::wall)))
				PLAYER::positionY++;
			break;
		case 5:
			return false;
		}
		m_grid[PLAYER::positionY][PLAYER::positionX] = TILE::player;

		//moveEnemy();
		draw();

		if (!(damage(PLAYER::positionY, PLAYER::positionX)))
			return true;
		else
			std::cout << "You died!" << '\n';
			return false;
		return true;
	}

	//Output the grid values on Y and X.
	void draw()
	{
		for (int a{ 0 }; a < (GRID::height); a++)
		{
			for (int b{ 0 }; b < (GRID::width); b++)
			{
				std::cout << m_grid[a][b];
			}
			std::cout << '\n';
		}
	}

	//Generate a random number between 501 and 502 (pre, not finished)
	int random()
	{
		static short count{0};

		if (count == 0)
		{
			count++;
			return 501;
		}
		else if (count == 1)
		{
			count--;
			return 502;
		}
		return 501;
	}

	//Set the spawn position Y and X, the function will spawn a random enemy on it.
	void spawnEnemy(const int tempPosY, const int tempPosX, int tempId = 0, unsigned char tempTile = 0, unsigned int top = 0)
	{
		tempId = random();
		top = (m_enemies.size());

		if (tempId == ENEMY::swordsman.id)
		{
			tempTile = ENEMY::swordsman.tile;

			m_enemies.push_back({ tempId, tempTile, tempPosY, tempPosX });
			m_grid[tempPosY][tempPosX] = m_enemies[top].tile;
		}
		else if (tempId == ENEMY::knight.id)
		{
			tempTile = ENEMY::knight.tile;

			m_enemies.push_back({ tempId, tempTile, tempPosY, tempPosX });
			m_grid[tempPosY][tempPosX] = m_enemies[top].tile;
		}
	}

	//Usig a predefined path, moves the enemy position every time that the players moves.
	void moveEnemy()
	{
		//unable to move on the first run
		if (m_secondRun == true)
		{
			m_secondRun = false;
		}
		else
		{
			for (unsigned short a{ 0 }; a < (m_enemies.size()); a++)
			{
				if ((m_enemies.at(a).id) == (ENEMY::swordsman.id))
				{
					//replace the tile where it last be with floor
					m_grid[m_enemies[a].posY][m_enemies[a].posX] = TILE::floor;
					//move the coords
					m_enemies[a].posX++;
					//put the enemy tile in the new position
					m_grid[m_enemies[a].posY][m_enemies[a].posX] = m_enemies[a].tile;
				}
				else if ((m_enemies.at(a).id) == (ENEMY::knight.id))
				{
					//replace the tile where it last be with floor
					m_grid[m_enemies[a].posY][m_enemies[a].posX] = TILE::floor;
					//move the coords
					m_enemies[a].posX--;
					//put the enemy tile in the new position
					m_grid[m_enemies[a].posY][m_enemies[a].posX] = m_enemies[a].tile;
				}
			}
		}
	}

	//Take the coords of the player, the direction to compare and the tile that is compared.
	//direction index: 8 if up, 2 if down, 4 if left, 6 if right.
	bool collision(int tempPosY, int tempPosX, short direction, unsigned char tileColl, bool isColl = false)
	{
		if (direction == 8)
		{
			tempPosY--;
		}
		else if (direction == 2)
		{
			tempPosY++;
		}
		else if (direction == 4)
		{
			tempPosX--;
		}
		else if (direction == 6)
		{
			tempPosX++;
		}

		isColl = ((m_grid[tempPosY][tempPosX]) == tileColl);
		if (isColl == true)
			return true;
		else
			return false;
	}

	//If the player collide with a enemy, end the game.
	bool damage(int tempPosY, int tempPosX)
	{
		for (unsigned short a{0}; a < (m_enemies.size()); a++ )
		{
			if (((PLAYER::positionY) == (m_enemies[a].posY)) && ((PLAYER::positionX) == (m_enemies[a].posX)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	//Deallocate all memory that is setup at the end of the game.
	~GameSystem()
	{
		for (int a{ 0 }; a < (GRID::height); a++)
		{
			delete[] m_grid[a];
		}
		delete m_grid;
		m_grid = nullptr;
	}
};

int main()
{
	unsigned short key{ 0 };

	GameSystem run;
	run.init();
	while (run.state(key))
	{
		key = 0;
		std::cin >> key;
		system("CLS");
	}

	return 0;
}
