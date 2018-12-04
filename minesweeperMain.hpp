/*
Team 17
Zach Bax
Edward Koch
Brandon McCurry
Alan Truong
Lucas Winkelman

Minesweeper Implementation
*/
#include <iostream>
#include <ctime>
#include <utility>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <map>
#include <chrono>
using namespace std;
using namespace chrono;

enum AIType
{
    BombSniffer=0,
    AIPlayer=1
};

enum AICustomizations
{
   Image,
   Type
};



//Board Size: Medium = 16x16
const int BSIZE = 16;  //board size plus two to account for board edges
const string PLAYER_IMAGE_PATH = "Icons/Gosnell.png";
//Number of Bombs: Meduim = 40
const int NBOMBS = 40;
const double SECS_PER_MOVE = 0.3;
const unsigned int FRAME_RATE = 30 ;

unsigned int playerX;
unsigned int playerY;

const unsigned int MAIN_MENU_BUTTON_WIDTH = 200;
const unsigned int MAIN_MENU_BUTTON_HEIGHT = 100;
const sf::Color MAIN_MENU_BUTTON_COLOR = sf::Color(255,0,0,255);
const sf::Color DARK_GREY = sf::Color(192,192,192);
const sf::Color MAIN_MENU_FONT_COLOR = sf::Color(0,0,0,255);
const unsigned int AI_CHOOSE_ROW_HEIGHT = 150;
const unsigned int AI_TOP_MARGIN = 200;
const sf::Color AI_SELECTED_COLOR = sf::Color(200,0,0,50);
const sf::Color AI_OVER_COLOR = sf::Color(0,0,200,50);
const unsigned int MAIN_MENU_FONT_SIZE = 30;
const unsigned int MAIN_MENU_BUTTON_FONT_SIZE = 24;
const unsigned int AI_PIXEL_SIZE = 100;
const unsigned int CHOOSE_AI_TOP_TEXT_Y = 50;
const unsigned int CHOOSE_AI_TOP_MARGIN_SIZE = 100;
const unsigned int MAX_AIS = 5;
const unsigned int ARROW_RADIUS = 15;
const unsigned int ARROW_DIST = 50;
const unsigned int AI_IMAGE_X = 100;
const unsigned int AI_TYPE_X = 500;

int tilesUncovered = 0;
int flagsPlaced = 0;

sf::Texture defaultTexture;
sf::Sprite b[BSIZE][BSIZE];
sf::Sprite LoadPlayerImage( );

// all images are defined here
sf::Image mineImage;
sf::Image flag0Image;
sf::Image flag1Image;
sf::Image flag2Image;
sf::Image flag3Image;
sf::Image flag4Image;
sf::Image flag5Image;
sf::Image flag6Image;
sf::Image flag7Image;
sf::Image flag8Image;
sf::Image ExplodedMineImage;
sf::Image flagFlagImage;
sf::Image CharacterImage;
sf::Image AIImage;
sf::Image defaultImage;
sf::Image leoFlagImage;

// all textures are defined here
sf::Texture mineTexture;
sf::Texture flag0Texture;
sf::Texture flag1Texture;
sf::Texture flag2Texture;
sf::Texture flag3Texture;
sf::Texture flag4Texture;
sf::Texture flag5Texture;
sf::Texture flag6Texture;
sf::Texture flag7Texture;
sf::Texture flag8Texture;
sf::Texture ExplodedMineTexture;
sf::Texture flagFlagTexture;
sf::Texture CharacterTexture;
sf::Texture AITexture;
sf::Texture leoFlagTexture;

void createBoard(vector<vector<char>> & b);
void clearBoard(vector<vector<char>> & b);
void dfs(vector<vector<char>> & b, vector<pair<int,int> > v);
bool findPath(vector<vector<char>> & b);
void addBombs(vector<vector<char>> & b);
void addNumbers(vector<vector<char>> & b);
void print(vector<vector<char>> & b);
bool isInBounds(const unsigned int x, const unsigned int y, unsigned int size);
void RevealTile(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY);
void SetSize ( sf::Sprite & inSprite , double xSize, double ySize);
void placeFlagAI(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY);
void revealSurrondingTiles(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY);
void revealAllBomb(vector<vector<char>> mediumBoard, double scaleX, double scaleY);
void endGame(char wOL); //wOL - Win or Lose
int checkBoard(); //Counts number of Tiles Uncovered and Flags Placed
bool flagsCorrect(vector<vector<char>> mediumBoard); //Checks if each placed flag is correct
bool boardFull(vector<vector<char>> checkedBoard);
vector<sf::Texture> loadAITextures(const vector<string> & Paths);
vector<string> loadPresetAIImagePaths ( );
map<AIType, string> loadPresetAITypeStrings();
vector<AIType> loadPresetAITypes();




void createBoard(vector<vector<char>> & b)
{
  b.resize(BSIZE);
  for(int x = 0; x < BSIZE; x++)
  {
    b[x].resize(BSIZE);
  }
  addBombs(b);
  addNumbers(b);
}

//Function to clear the board
//Used to re-make the board if it is unsolvable
void clearBoard(vector<vector<char>> & b)
{
  for(int i = 1; i < BSIZE-1; i++)
  {
    for(int j = 1; j < BSIZE-1; j++)
    {
      b[i][j] = ' ';
    }
  }
}

//Depth First Search step through the board and mark all spot that is not block off by walls and bomb as '1'
void dfs(vector<vector<char>> & b, vector<pair<int,int> > v)
{
  //Pair of the current node
  pair<int,int> top = v.back();
  //Get first element of pair, X cordinate
  int x = get<0>(top);
  //Pet second element of pair, Y cordinate
  int y = get<1>(top);
  //Mark current node as reachable
  b[x][y] = '1';
  //Check if the area above current is reachable
  if(isInBounds(x, y-1, BSIZE))
    if(b[x][y-1] == ' ')
    {
      //If reachable push the cordinate into the stack
      v.push_back(make_pair(x,y-1));
      //Recursion to mark the most recent reachable spot as current node
      dfs(b,v);
    }
  //Check if the area to the right of current is reachable
  if(isInBounds(x+1, y, BSIZE))
    if(b[x+1][y] == ' ')
    {
      //If reachable push the cordinate into the stack
      v.push_back(make_pair(x+1,y));
      //Recursion to mark the most recent reachable spot as current node
      dfs(b,v);
    }
  //Check if the area below current is reachable
  if(isInBounds(x, y+1, BSIZE))
    if(b[x][y+1] ==' ')
    {
      //If reachable push the cordinate into the stack
      v.push_back(make_pair(x,y+1));
      //Recursion to mark the most recent reachable spot as current node
      dfs(b,v);
    }
  //Check if the area to the left of current is reachable
  if(isInBounds(x-1, y, BSIZE))
    if(b[x-1][y] ==' ')
    {
      //If reachable push the cordinate into the stack
      v.push_back(make_pair(x-1,y));
      //Recursion to mark the most recent reachable spot as current node
      dfs(b,v);
    }
  //No reachable spot beyond the current node, pop off current node from the stack
  v.pop_back();
}

//Create a copy of the board to check if there is a path to every non-bomb area
bool findPath(vector<vector<char>> & b)
{
  vector<vector<char>> copyBoard;
  int x,y;
  vector<pair<int,int> > cordStack;
  //Copy the board into the newboard
  copyBoard.resize(BSIZE);
  for(int i = 0; i < BSIZE; i++)
  {
    copyBoard[i].resize(BSIZE);
    for(int j = 0; j < BSIZE; j++)
    {
      copyBoard[i][j] = b[i][j];
    }
  }
  //Find first non bomb and wall space
  for(int i = 0; i < BSIZE; i++)
  {
    for(int j = 0; j <BSIZE; j++)
    {
      if(copyBoard[i][j] == ' ')
      {
        //Push the cordinate to the stack to mark as starting point for the search
        cordStack.push_back(make_pair(i,j));
        break;
      }
    }
  }
  //If stack isnt empty start the search
  if(!cordStack.empty())
  {
    dfs(copyBoard,cordStack);
  }
  //Check if any of the index of the board has not been reach by the search function
  //If so return false, else return true
  for(int i = 0; i < BSIZE; i++ )
  {
    for(int j = 0; j < BSIZE; j++)
    {
      if(copyBoard[i][j] == ' ')
      {
        return false;
      }
    }
  }
  return true;
}

//Function to add bomb to the board
void addBombs(vector<vector<char>> & b)
{
  //Do while loop to add bomb to the board before checking if the board is unsolvable or not
  do
  {
    //Clear the board before placing any new bomb in case of second iteration of the loop
    clearBoard(b);
    //Place bombs onto the board
    for(int i = 0; i < NBOMBS; i++)
    {
      bool placedBomb = false;
      int x_cord = (rand() % BSIZE);
      int y_cord = (rand() % BSIZE);
      //While loop to keep trying to find a index that does not have a bomb currently on it
      while(!placedBomb)
      {
        //If current index is not a bomb place bomb and break out of the while loop
      	if (b[x_cord][y_cord] != 'B')
      	{
      	  b[x_cord][y_cord] = 'B';
      	  placedBomb = true;
      	}
        //If current index is a bomb then find new random x and y cordinate
      	else
      	{
      	  x_cord = (rand() % BSIZE);
      	  y_cord = (rand() % BSIZE);
        }
      }
    }
  //After placing the bomb check all non bomb space is reachable and not block off by walls and bomb
  }while(!findPath(b));
}

void addNumbers(vector<vector<char>> & b)
{
  int numBombs = 0;
  for(int x = 0; x < BSIZE; x++)
  {
    for(int y = 0; y < BSIZE; y++)
    {
      //sets the number of surrounding bombs to 0 for each square
      numBombs = 0;
      //If current square is a Bomb, Don't Do Anything
      if(b[x][y] == 'B');
      else
      {
        //Checks Each surrounding square for a bomb, if there is a bomb, add it to the counter
        if(x >= 1 )
        {
          if(y >= 1 && b[x - 1][y - 1] == 'B') numBombs ++; //Top Left Neighbor
          if(b[x - 1][y] == 'B') numBombs ++;//Left Neighbor
          if(y < BSIZE - 1 && b[x - 1][y + 1] == 'B') numBombs ++;//Bottom Left Neighbor
        }
        if(y >= 1 && b[x][y - 1] == 'B') numBombs ++;//Top Neighbor
        if(y < BSIZE - 1 && b[x][y + 1] == 'B') numBombs ++;//Bottom Neighbor
        if(x < BSIZE - 1 )
        {
          if(y >= 1 && b[x + 1][y - 1] == 'B') numBombs ++;//Top Right Neighbor
          if(b[x + 1][y] == 'B') numBombs ++;//Right Neighbor
          if(y < BSIZE - 1 && b[x + 1][y + 1] == 'B') numBombs ++;//Bottom Right Neighbor
        }
        //Sets the current Square Equal to the number of bombs around it
        switch(numBombs)
        {
          case 0: b[x][y] = 'O';
                  break;
          case 1: b[x][y] = '1';
                  break;
          case 2: b[x][y] = '2';
                  break;
          case 3: b[x][y] = '3';
                  break;
          case 4: b[x][y] = '4';
                  break;
          case 5: b[x][y] = '5';
                  break;
          case 6: b[x][y] = '6';
                  break;
          case 7: b[x][y] = '7';
                  break;
          case 8: b[x][y] = '8';
                  break;
          case 9: b[x][y] = '9';
                  break;
        }
      }
    }
  }
}

//Check if the move the user is tying to make is not a wall
bool isValidMove(int x, int y)
{
  return!((b[x][y].getTexture() == &defaultTexture || b[x][y].getTexture() == &flagFlagTexture)&&
          (!isInBounds(x-1, y, BSIZE) || b[x-1][y].getTexture() == &defaultTexture || b[x-1][y].getTexture() == &flagFlagTexture)&&
          (!isInBounds(x+1, y, BSIZE) || b[x+1][y].getTexture() == &defaultTexture || b[x+1][y].getTexture() == &flagFlagTexture || x == BSIZE)&&
          (!isInBounds(x, y+1, BSIZE) || b[x][y+1].getTexture() == &defaultTexture || b[x][y+1].getTexture() == &flagFlagTexture || y == BSIZE)&&
          (!isInBounds(x, y-1, BSIZE) || b[x][y-1].getTexture() == &defaultTexture || b[x][y-1].getTexture() == &flagFlagTexture || y == 0)&&
          (!isInBounds(x+1, y+1, BSIZE) || b[x+1][y+1].getTexture() == &defaultTexture || b[x+1][y+1].getTexture() == &flagFlagTexture)&&
          (!isInBounds(x+1, y-1, BSIZE) || b[x+1][y-1].getTexture() == &defaultTexture || b[x+1][y-1].getTexture() == &flagFlagTexture)&&
          (!isInBounds(x-1, y+1, BSIZE) || b[x-1][y+1].getTexture() == &defaultTexture || b[x-1][y+1].getTexture() == &flagFlagTexture)&&
          (!isInBounds(x-1, y-1, BSIZE) || b[x-1][y-1].getTexture() == &defaultTexture || b[x-1][y-1].getTexture() == &flagFlagTexture));
}

void print(vector<vector<char>> & b)
{
  for(int y = 0; y < BSIZE; y++)
  {
    for(int x = 0; x < BSIZE; x++)
    {
      cout << b[x][y] << " ";
    }
    cout << endl;
  }
}

bool isInBounds(const unsigned int x, const unsigned int y, unsigned int size)
{
  bool output = true;
  if(x < 0 || x >= size || y < 0 || y >= size)
    output = false;
  return output;
}

void RevealTile(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY)
{
  tilesUncovered ++;
	// Draw a new outline color where the user selected
	sf::Texture * temp = &flag2Texture;
	if ( mediumBoard[boardX][boardY] == 'O')
	{
		temp = &flag0Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '1')
	{
		temp = &flag1Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '2')
	{
		temp = &flag2Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '3')
	{
		temp = &flag3Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '4')
	{
		temp = &flag4Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '5')
	{
		temp = &flag5Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '6')
	{
		temp = &flag6Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '7')
	{
		temp = &flag7Texture;
	}
	else if ( mediumBoard[boardX][boardY] == '8')
	{
		temp = &flag8Texture;
	}
	else if ( mediumBoard[boardX][boardY] == 'B')
	{
		temp = &mineTexture;
	}
	//cout << mediumBoard[boardX][boardY] << endl ;
	//cout << boardX << " " << boardY << endl ;
	sf::Sprite sprite;
	sprite.setTexture(*temp, true);
	sprite.setPosition(25*scaleX+50*(boardX)*scaleX, 25*scaleY+50*(boardY)*scaleY);
	SetSize(sprite,50*scaleX,50*scaleY);
	b[(boardX)][(boardY)] = sprite;
}

void SetSize ( sf::Sprite & inSprite , double xSize, double ySize)
{
	double factorX = xSize / inSprite.getTextureRect().width;
	double factorY = ySize / inSprite.getTextureRect().height;
	inSprite.setScale(factorX, factorY);
}

//Places a Flag for the Bomb Sniffing AI
void placeFlagAI(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY)
{
  sf::Texture * temp = &leoFlagTexture;
  if ( mediumBoard[boardX][boardY] == 'B')
	{
		temp = &flagFlagTexture;
		cout << mediumBoard[boardX][boardY] << ": " << boardX << " " << boardY << endl ;
		sf::Sprite sprite;
		sprite.setTexture(*temp, true);
		sprite.setPosition(25*scaleX+50*(boardX)*scaleX, 25*scaleY+50*(boardY)*scaleY);
		SetSize(sprite,50*scaleX,50*scaleY);
		b[(boardX)][(boardY)] = sprite;
	}
}

void revealSurrondingTiles(unsigned int boardX, unsigned int boardY, vector<vector<char>> mediumBoard, double scaleX, double scaleY)
{
	if(isInBounds(boardX, boardY, BSIZE))
	{
		if(b[boardX][boardY].getTexture() == &defaultTexture)
		{
			RevealTile(boardX,boardY,mediumBoard,scaleX, scaleY);
			if(mediumBoard[boardX][boardY] == 'O')
			{
				revealSurrondingTiles(boardX+1,boardY,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX-1,boardY,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX,boardY+1,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX,boardY-1,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX+1,boardY+1,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX-1,boardY+1,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX+1,boardY-1,mediumBoard,scaleX, scaleY);
				revealSurrondingTiles(boardX-1,boardY-1,mediumBoard,scaleX, scaleY);
			}
		}
	}
}

//Reveal all the tiles with bomb on the board
void revealAllBomb(vector<vector<char>> mediumBoard, double scaleX, double scaleY)
{
  //Run through the board
  for(int y = 0; y < BSIZE; y++)
  {
    for(int x = 0; x < BSIZE; x++)
    {
      //If any of the tile is a bomb call reveal function on the tile
      if(mediumBoard[x][y] == 'B')
      {
        RevealTile(x,y,mediumBoard,scaleX, scaleY);
      }
    }
  }
}

//Displays End of Game Animations in a New Window
//Based on Win or Lose
void endGame(char wOL, double scaleX, double scaleY)
{

  int timeLimit = 0;
  cout << "Game Over ";
  if (wOL == 'w')
  {
    sf::Music music;
    music.openFromFile("Icons/EndPraise.flac");
    music.play();
    cout << "YOU WON!!" << endl;
    sf::Texture texture;
  	if (!texture.loadFromFile("Icons/Price.png"))
  		cout << "ERROR" << endl;
  	sf::Sprite winnerSprite(texture);

    sf::Texture texture2;
  	if (!texture2.loadFromFile("Icons/PriceOpen.png"))
  		cout << "ERROR" << endl;
  	sf::Sprite winnerSprite2(texture2);

    sf::RenderWindow window(sf::VideoMode(500*scaleX, 500*scaleY), "YOU WON");
  	window.setFramerateLimit(FRAME_RATE);
    while(window.isOpen() && timeLimit < FRAME_RATE * 5)
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
      }
      window.clear();
      SetSize(winnerSprite, scaleX * 500, scaleY * 500 );
      SetSize(winnerSprite2, scaleX * 500, scaleY * 500 );
      //Switches between 2 frames of the "video"
      if (timeLimit % 5 == 0) window.draw(winnerSprite);
      else window.draw(winnerSprite2);

      window.display();
      timeLimit ++;
    }
  }

  else if (wOL == 'l')
  {
    sf::Music music;
    music.openFromFile("Icons/EndLaugh.flac");
    music.play();
    cout << "YOU LOSER!!" << endl;
    sf::Texture texture;
  	if (!texture.loadFromFile("Icons/mine.png"))
  		cout << "ERROR" << endl;
  	sf::Sprite loserSprite(texture);

    sf::Texture texture2;
  	if (!texture2.loadFromFile("Icons/mineOpen.png"))
  		cout << "ERROR" << endl;
  	sf::Sprite loserSprite2(texture2);

    sf::RenderWindow window(sf::VideoMode(500*scaleX, 500*scaleY), "YOU LOST");
  	window.setFramerateLimit(FRAME_RATE);
    while(window.isOpen() && timeLimit < FRAME_RATE * 5)
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
      }
      window.clear();
      SetSize(loserSprite, scaleX * 500, scaleY * 500 );
      SetSize(loserSprite2, scaleX * 500, scaleY * 500 );
      //Switches between 2 frames of the "video"
      if (timeLimit % 5 == 0) window.draw(loserSprite);
      else window.draw(loserSprite2);

      window.display();
      timeLimit ++;
    }
  }

  else cout << "INVALID CONDITION" << endl;
}

//Checks Board and counts remaining tiles and flags placed
int checkBoard()
{
  int coveredCount = 0;
  int flagCount = 0;
  for(int y = 0; y < BSIZE; y++)
  {
    for(int x = 0; x < BSIZE; x++)
    {
      if(b[x][y].getTexture() == &defaultTexture)
      {
        coveredCount ++;
      }
      if(b[x][y].getTexture() == &flagFlagTexture)
      {
        coveredCount++;
        flagCount++;
      }
    }
  }
  flagsPlaced = flagCount;
  return coveredCount;
}

//Checks to see if each placed flag is correctly placed
bool flagsCorrect(vector<vector<char>> mediumBoard)
{
  int numCorrect = 0;
  for(int y = 0; y < BSIZE; y++)
  {
    for(int x = 0; x < BSIZE; x++)
    {
      if(b[x][y].getTexture() == &flagFlagTexture && mediumBoard[x][y] == 'B')
      {
        numCorrect ++;
      }
    }
  }
  if(numCorrect == NBOMBS)
  {
    return true;
  }
  else return false;
}

bool boardFull(vector<vector<char>> checkedBoard)
{
  int numCorrect = 0;
  for(int y = 0; y < BSIZE; y++)
  {
    for(int x = 0; x < BSIZE; x++)
    {
      if(checkedBoard[x][y] == 'O')
      {
        numCorrect ++;
      }
    }
  }
  if(numCorrect == BSIZE * BSIZE)
  {
    return true;
  }
  else return false;
}

sf::Sprite LoadPlayerImage ( )
{
	sf::Texture texture;
	if (!texture.loadFromFile(PLAYER_IMAGE_PATH))
		cout << "ERROR" << endl;
	sf::Sprite sprite(texture);
	return sprite;
}


class AI
{
  public:
    unsigned int x;
    unsigned int y;
    string name;
    unsigned int targetX;
    unsigned int targetY;
    vector<vector<char>> tilesVisited;
    sf::Texture texture;
    sf::Image image;
    sf::Sprite sprite;
    double Speed;
    high_resolution_clock::time_point lastMoveTime;

    void loadImageFromPath(string path)
    {
      image.loadFromFile(path);
      texture.loadFromImage(image);
    }

    void createCheckBoard()
    {
      createBoard(tilesVisited);
      clearBoard(tilesVisited);
    }

    void selectNewTarget(vector<vector<char>> & mediumBoard, sf::Sprite b[BSIZE][BSIZE])
    {
      vector<vector<char>> tilesChecked;
      createBoard(tilesChecked);
      clearBoard(tilesChecked);
      //makes sure target is a bomb for the bomb sniffer
      if (name == "BombSniffer")
      {
        do
        {
          targetX = rand() % BSIZE;
          targetY = rand() % BSIZE;
          tilesChecked[targetX][targetY] = 'O';
        }
        while((mediumBoard[targetX][targetY] != 'B' ||
              tilesVisited[targetX][targetY] == 'X' ||
              b[targetX][targetY].getTexture() != &defaultTexture) &&
              !boardFull(tilesChecked));
      }

      //Makes sure tile is not a bomb for regular AI
      else if(name == "AIPlayer")
      do
      {
        targetX = rand() % BSIZE;
        targetY = rand() % BSIZE;
        tilesChecked[targetX][targetY] = 'O';
      }
      while((mediumBoard[targetX][targetY] == 'B' ||
            tilesVisited[targetX][targetY] == 'X' ||
            b[targetX][targetY].getTexture() != &defaultTexture) &&
            !boardFull(tilesChecked));

      //If there are no more valid moves, stay put
      if(boardFull(tilesChecked))
      {
        targetX = x;
        targetY = y;
      }
      tilesVisited[targetX][targetY] = 'X';
      //cout << "found tile " << mediumBoard[targetX][targetY] << " " << targetX << " " << targetY << endl ;
    }

    void moveTowardTarget()
    {
      if(x < targetX)
      {
        x = x + 1;
      }
      else if(x > targetX)
      {
        x = x - 1;
      }
      else if(y < targetY)
      {
        y = y + 1;
      }
      else if(y > targetY)
      {
        y = y - 1;
      }
      lastMoveTime = high_resolution_clock::now();;
    }

    bool isOnTarget()
    {
      bool output = false;
      if(x == targetX && y == targetY)
      {
        output = true;
      }
      return output;
    }

    void revealTileUnder(vector<vector<char>> mediumBoard, double scaleX, double scaleY)
    {
      //Places a flag for the Bomb Sniffing AI
      if (name == "BombSniffer")
      {
        placeFlagAI(x, y, mediumBoard, scaleX, scaleY);
      }
      //Reveals the tile for regular AI Player
      else if(name == "AIPlayer")
      {
        revealSurrondingTiles(x, y, mediumBoard, scaleX, scaleY);
      }
    }

    bool isTimeToMove() const
    {
      bool output = false;
      high_resolution_clock::time_point currentTime = high_resolution_clock::now();
      duration<double> time_span = duration_cast<duration<double>>(currentTime - lastMoveTime);
      double seconds = time_span.count() * Speed;
      //cout << seconds << " " << clicks << " " << currentTime << " " << lastMoveTime << " " << CLOCKS_PER_SEC << endl ;
      if(seconds > SECS_PER_MOVE)
      {
        output = true;
      }
      return output;
    }
};

vector < AI > loadAIs(vector<vector<char>> mediumBoard, sf::Sprite b[BSIZE][BSIZE])
{
  vector<AI> AIs;

  AIs.push_back(AI());
  AIs[0].loadImageFromPath("Icons/bombSniffer.png");
  AIs[0].name = "BombSniffer";
  AIs[0].createCheckBoard();
  AIs[0].Speed = 0.50;
  AIs[0].x = playerX;
  AIs[0].y = playerY;
  AIs[0].selectNewTarget(mediumBoard, b);

  for(int i = 0; i < 1; i++)
  {
    AIs.push_back(AI());
    AIs[i+1].loadImageFromPath("Icons/drtAI.png");
    AIs[i+1].name = "AIPlayer";
    AIs[i+1].createCheckBoard();
    AIs[i+1].Speed = 1.25;
    AIs[i+1].x = playerX;
    AIs[i+1].y = playerY;
    AIs[i+1].selectNewTarget(mediumBoard, b);
  }

/*
  AIs.push_back(AI());
  AIs[0].loadImageFromPath("Icons/drtAI.png");
  AIs[0].x = 0;
  AIs[0].y = 0;
  AIs[0].selectNewTarget(mediumBoard, b);

  AIs.push_back(AI());
  AIs[1].loadImageFromPath("Icons/drtAI.png");
  AIs[1].x = 0;
  AIs[1].y = 0;
  AIs[1].selectNewTarget(mediumBoard, b);

  AIs.push_back(AI());
  AIs[2].loadImageFromPath("Icons/drtAI.png");
  AIs[2].x = 0;
  AIs[2].y = 0;
  AIs[2].selectNewTarget(mediumBoard, b);

  AIs.push_back(AI());
  AIs[3].loadImageFromPath("Icons/drtAI.png");
  AIs[3].x = 0;
  AIs[3].y = 0;
  AIs[3].selectNewTarget(mediumBoard, b);
*/
  return AIs;
}

map<AIType, string> loadPresetAITypeStrings()
{
   map<AIType, string> AITypeMap;
   AITypeMap[AIType::BombSniffer] = "BombSniffer";
   AITypeMap[AIType::AIPlayer] = "AIPlayer";
   return AITypeMap;
}
;

vector<string> loadPresetAIImagePaths ( )
{
   vector<string> AIImagePaths;
   AIImagePaths.push_back("Icons/bombSniffer.png");
   AIImagePaths.push_back("Icons/drtAI.png");
   AIImagePaths.push_back("Icons/Price.png");
   AIImagePaths.push_back("Icons/Morales.png");
   AIImagePaths.push_back("Icons/Zhu.png");
   AIImagePaths.push_back("Icons/Sabharwal.png");
   return AIImagePaths;
}

vector<sf::Texture> loadAITextures(const vector<string> & Paths)
{
    vector<sf::Texture> Textures;
    unsigned int PathIndex = 0;
    Textures.resize(Paths.size());
    while(PathIndex < Paths.size())
    {
        sf::Image TempImage;
        TempImage.loadFromFile(Paths[PathIndex]);
        Textures[PathIndex].loadFromImage(TempImage);
        PathIndex = PathIndex + 1;
    }
    return Textures;
}

vector<AIType> loadPresetAITypes()
{
   vector<AIType> AIPossibleTypes;
   AIPossibleTypes.push_back(AIType::BombSniffer);
   AIPossibleTypes.push_back(AIType::AIPlayer);
   return AIPossibleTypes;
}
