//to compile use this line: g++ main.cpp -o sfml-app -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -std=c++11
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "minesweeperMain.hpp"
using namespace std;

int playGame(vector<AI> & AIs);
void AIselectMenu();
string mainMenu();

int main()
{
	vector<AI> AIs;
	string playType = mainMenu();
	while (playType != "close")
	{
		if (playType == "solo")
		{
			playGame(AIs);
			playType = mainMenu();
		}
		else if (playType == "AI")
		{
			AIselectMenu();
			playType = mainMenu();
		}
	}
}

string mainMenu()
{
	// set the scale variables
	double scaleX = ((double)sf::VideoMode::getDesktopMode().width)/1622;
	double scaleY = ((double)sf::VideoMode::getDesktopMode().height)/975;

	// create the window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(950*scaleX, 950*scaleY), "Minekowsy Sweeper", sf::Style::Default, settings);
	window.setFramerateLimit(FRAME_RATE);

	// try to change the font
	sf::Font mainFont;
	if (!mainFont.loadFromFile("Icons/Anonymous.ttf"))
	{
	}

	// set game title text attributes
	sf::Text mainText;
	mainText.setColor(MAIN_MENU_FONT_COLOR);
	mainText.setFont(mainFont);
	mainText.setString("Minekowsy Sweeper");
	mainText.setCharacterSize(MAIN_MENU_FONT_SIZE);
	double mainTextWidth = mainText.getLocalBounds().width;

	// try to change the font
	sf::Font font;
	if (!font.loadFromFile("Icons/Ubuntu-L.ttf"))
	{
	}

	// set the play solo button text attributes
	sf::Text playText;
	playText.setColor(MAIN_MENU_FONT_COLOR);
	playText.setFont(font);
	playText.setString("Play solo");
	playText.setCharacterSize(MAIN_MENU_BUTTON_FONT_SIZE);
	double textWidth = playText.getLocalBounds().width;
	double textHeight = playText.getLocalBounds().height;

	// set the play solo button attributes
	sf::RectangleShape playButton(sf::Vector2f(MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT));
	unsigned int ButtonX = (window.getSize().x - MAIN_MENU_BUTTON_WIDTH) / 2; // center the button with a predetermined button size
	unsigned int ButtonY = (window.getSize().y - MAIN_MENU_BUTTON_HEIGHT - (MAIN_MENU_BUTTON_HEIGHT * 3 / 2)) / 2;
	playButton.setFillColor(MAIN_MENU_BUTTON_COLOR);

	// set the choose AI button text attributes
	sf::Text chooseAIText;
	chooseAIText.setColor(MAIN_MENU_FONT_COLOR);
	chooseAIText.setFont(font);
	chooseAIText.setString("Choose AI helper");
	chooseAIText.setCharacterSize(MAIN_MENU_BUTTON_FONT_SIZE);
	double AItextWidth = chooseAIText.getLocalBounds().width;
	double AItextHeight = chooseAIText.getLocalBounds().height;

	// set the choose AI button attributes
	sf::RectangleShape AIButton(sf::Vector2f(MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT));
	unsigned int AIButtonY = (window.getSize().y - MAIN_MENU_BUTTON_HEIGHT + (MAIN_MENU_BUTTON_HEIGHT * 3 / 2)) / 2; // uses same ButtonX
	AIButton.setFillColor(AI_SELECTED_COLOR);

	// set the position of each item
	mainText.setPosition(sf::Vector2f((window.getSize().x - mainTextWidth) / 2, 100));
	playButton.setPosition(sf::Vector2f(ButtonX, ButtonY));
	playText.setPosition(sf::Vector2f(ButtonX + (MAIN_MENU_BUTTON_WIDTH / 2) - (textWidth / 2), ButtonY + (MAIN_MENU_BUTTON_HEIGHT / 2) - (textHeight / 2)));
	AIButton.setPosition(sf::Vector2f(ButtonX, AIButtonY));
	chooseAIText.setPosition(sf::Vector2f(ButtonX + (MAIN_MENU_BUTTON_WIDTH / 2) - (AItextWidth / 2), AIButtonY + (MAIN_MENU_BUTTON_HEIGHT / 2) - (AItextHeight / 2)));

	// draw the necessary items to a blank screen
	window.clear(DARK_GREY);
	window.draw(mainText);
	window.draw(playButton);
	window.draw(playText);
	window.draw(AIButton);
	window.draw(chooseAIText);
	window.display();

	// an array to handle various button attributes easier
	sf::RectangleShape buttons[2];
	buttons[0] = playButton;
	buttons[1] = AIButton;
	bool selector = 0;

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				// toggle between the two buttons
				if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Up)
				{
					buttons[selector].setFillColor(AI_SELECTED_COLOR);
					selector = !selector;
					buttons[selector].setFillColor(MAIN_MENU_BUTTON_COLOR);
				}
				// select the highlighted button
				else if (event.key.code == sf::Keyboard::Return)
				{
					window.close();
					if (selector)
						return "AI";
					else
						return "solo";
				}
			}
		}

		// refresh the screen
		window.clear(DARK_GREY);
		window.draw(mainText);
		window.draw(buttons[0]); //draw playbutton
		window.draw(playText);
		window.draw(buttons[1]); //draw AIbutton
		window.draw(chooseAIText);
		window.display();
	}
	return "close";
}

void AIselectMenu()
{
	vector<AI> OutputAIs;
	vector<int> AIImages;
	vector<int> AITypes;
	vector<string> ImagePaths = loadPresetAIImagePaths();
	vector<sf::Texture> AITextures = loadAITextures(ImagePaths);
	map<AIType, string> AITypeStrings = loadPresetAITypeStrings();
	vector<AIType> AIPossibleTypes = loadPresetAITypes();

	bool isOnExit = false;	// is on the Play Game button in the AI selector menu
	bool isChangingAI = false;	// is editing a specific AI
	AICustomizations optionOver = AICustomizations::Image;
	double scaleX = ((double)sf::VideoMode::getDesktopMode().width)/1622;
	double scaleY = ((double)sf::VideoMode::getDesktopMode().height)/975;

	// create the window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(950*scaleX, 950*scaleY), "Minesweeper", sf::Style::Default, settings);
	window.setFramerateLimit(FRAME_RATE);

	// define the fonts
	sf::Font mainFont;
	if (!mainFont.loadFromFile("Icons/Anonymous.ttf"))
	{
	}
	sf::Font font;
	if (!font.loadFromFile("Icons/Ubuntu-L.ttf"))
	{
	}

	// set the attributes for the AI menu title
	sf::Text chooseAIText;
	chooseAIText.setColor(MAIN_MENU_FONT_COLOR);
	chooseAIText.setFont(mainFont);
	chooseAIText.setString("Choose AIs");
	chooseAIText.setCharacterSize(MAIN_MENU_FONT_SIZE);
	double chooseAITextWidth = chooseAIText.getLocalBounds().width;
	double chooseAITextHeight = chooseAIText.getLocalBounds().height;
	chooseAIText.setPosition(sf::Vector2f((window.getSize().x - chooseAITextWidth) / 2, CHOOSE_AI_TOP_TEXT_Y * scaleY));

	// set the attributes for the Play Game text
	sf::Text selectText;
	selectText.setColor(MAIN_MENU_FONT_COLOR);
	selectText.setFont(mainFont);
	selectText.setString("Play Game");
	selectText.setCharacterSize(MAIN_MENU_FONT_SIZE);
	selectText.setPosition(sf::Vector2f((window.getSize().x - selectText.getLocalBounds().width) / 2,
																			(window.getSize().y - selectText.getLocalBounds().height - 15 * scaleY) * 3 / 4));

	// set the attributes for the Play Game button
	sf::RectangleShape playButton(sf::Vector2f(MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT));
	unsigned int ButtonX = (window.getSize().x - MAIN_MENU_BUTTON_WIDTH) / 2; // center the button with a predetermined button size
	unsigned int ButtonY = (window.getSize().y - MAIN_MENU_BUTTON_HEIGHT - (MAIN_MENU_BUTTON_HEIGHT * 3 / 2)) / 2;
	playButton.setFillColor(MAIN_MENU_BUTTON_COLOR);
	playButton.setPosition(sf::Vector2f((window.getSize().x - playButton.getLocalBounds().width) / 2,
																			(window.getSize().y - playButton.getLocalBounds().height) * 3 / 4));

	// right arrow attributes
	sf::CircleShape rightArrow(ARROW_RADIUS, 3);
	rightArrow.setOrigin({ARROW_RADIUS, ARROW_RADIUS});
	rightArrow.setRotation(90);
	rightArrow.setFillColor(sf::Color(0, 0, 255));
	rightArrow.setPosition(200, 200);

	// left arrow attributes
	sf::CircleShape leftArrow(ARROW_RADIUS, 3);
	leftArrow.setOrigin({ARROW_RADIUS, ARROW_RADIUS});
	leftArrow.setRotation(-90);
	leftArrow.setFillColor(sf::Color(0, 0, 255));
	leftArrow.setPosition(200, 200);

	// up arrow attributes
	sf::CircleShape upArrow(ARROW_RADIUS, 3);
	upArrow.setOrigin({ARROW_RADIUS, ARROW_RADIUS});
	upArrow.setRotation(0);
	upArrow.setFillColor(sf::Color(0, 0, 255));
	upArrow.setPosition(200, 200);

	// down arrow attributes
	sf::CircleShape downArrow(ARROW_RADIUS, 3);
	downArrow.setOrigin({ARROW_RADIUS, ARROW_RADIUS});
	downArrow.setRotation(180);
	downArrow.setFillColor(sf::Color(0, 0, 255));
	downArrow.setPosition(200, 200);

	// default AIs
	unsigned int rowSelected = 3;
	AIImages.push_back(0);
	AITypes.push_back(0);
	AIImages.push_back(1);
	AITypes.push_back(1);
	AIImages.push_back(2);
	AITypes.push_back(1);
	AIImages.push_back(3);
	AITypes.push_back(1);

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right && !isOnExit)
				{
					if(isChangingAI)
					{
						if(optionOver == AICustomizations::Image)
						{
							AIImages[rowSelected] = (AIImages[rowSelected] + 1) % ImagePaths.size();
							cout << AIImages[rowSelected] << endl;
						}
						else if(optionOver == AICustomizations::Type)
							AITypes[rowSelected] = (AITypes[rowSelected] + 1) % AITypeStrings.size();
					}
					else if(rowSelected < MAX_AIS - 1)
					{
						rowSelected = rowSelected + 1;
						if(rowSelected >= AIImages.size())
						{
							AIImages.push_back(0);
							AITypes.push_back(AIType::AIPlayer);
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Left && !isOnExit)
				{
					if(isChangingAI)
					{
						if(optionOver == AICustomizations::Image)
						{
							if (AIImages[rowSelected] - 1 < 0)
								AIImages[rowSelected] = ImagePaths.size() - 1;
							else
								AIImages[rowSelected] = (AIImages[rowSelected] - 1) % ImagePaths.size();
						}
						else if(optionOver == AICustomizations::Type)
							AITypes[rowSelected] = (AITypes[rowSelected] - 1) % AITypeStrings.size();
					}
					else if(rowSelected > 0)
					{
						rowSelected = rowSelected - 1;
						if(rowSelected <= AIImages.size())
						{
							AIImages.pop_back();
							AITypes.pop_back();
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
				{
					if(isChangingAI)
					{
						if(optionOver == AICustomizations::Image)
						{
							optionOver = AICustomizations::Type;
						}
						else
						{
							optionOver = AICustomizations::Image;
						}
					}
					else
					{
						isOnExit = !isOnExit;
					}
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					if(isOnExit)
					{
						unsigned int AIIndex = 0;
						while(AIIndex < AIImages.size())
						{
							OutputAIs.push_back(AI());
							OutputAIs[AIIndex].loadImageFromPath(ImagePaths[AIImages[AIIndex]]);
							OutputAIs[AIIndex].name = AITypeStrings[AIPossibleTypes[AITypes[AIIndex]]];
							OutputAIs[AIIndex].createCheckBoard();
							OutputAIs[AIIndex].Speed = 2;
							OutputAIs[AIIndex].x = playerX;
							OutputAIs[AIIndex].y = playerY;
							AIIndex = AIIndex + 1;
						}
						window.close();
						playGame(OutputAIs);
					}
					else
					{
						isChangingAI = !isChangingAI;
					}
				}
			}
		}
		window.clear(DARK_GREY);
		unsigned int AIIndex = 0;
		while(AIIndex < AIImages.size())
		{
			// AI character image
			sf::Sprite sprite(AITextures[AIImages[AIIndex]]);
			unsigned int aiY = (((AI_CHOOSE_ROW_HEIGHT - AI_PIXEL_SIZE) / 2) + (AI_CHOOSE_ROW_HEIGHT * AIIndex) + CHOOSE_AI_TOP_MARGIN_SIZE) * scaleX;
			sprite.setPosition(sf::Vector2f(aiY, (AI_TOP_MARGIN + AI_IMAGE_X) * scaleX));
			SetSize(sprite, scaleY * AI_PIXEL_SIZE, scaleX * AI_PIXEL_SIZE );

			// AI type text attributes
			sf::Text tempAIText;
			tempAIText.setColor(MAIN_MENU_FONT_COLOR);
			tempAIText.setFont(font);
			tempAIText.setString(AITypeStrings[AIPossibleTypes[AITypes[AIIndex]]]);
			tempAIText.setCharacterSize(MAIN_MENU_BUTTON_FONT_SIZE);
			tempAIText.setPosition(sf::Vector2f(aiY + AI_PIXEL_SIZE / 2, AI_TYPE_X * scaleY));

			// AI background attributes
			sf::RectangleShape AIbg(sf::Vector2f(AI_PIXEL_SIZE * scaleY, AI_PIXEL_SIZE * scaleX));
			AIbg.setFillColor(MAIN_MENU_BUTTON_COLOR);
			AIbg.setPosition(sf::Vector2f(aiY, (AI_TOP_MARGIN + AI_IMAGE_X) * scaleX));

			window.draw(AIbg);
			window.draw(sprite);
			window.draw(tempAIText);
			if(isChangingAI == true)
			{
				if(AIIndex == rowSelected)
				{
					// highlight the AI character image
					if(optionOver == AICustomizations::Image)
					{
						unsigned int ArrowX = AIbg.getPosition().x;
						unsigned int ArrowY = AIbg.getPosition().y + AIbg.getLocalBounds().height / 2;
						leftArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
						ArrowX = AIbg.getPosition().x + AIbg.getLocalBounds().width;
						rightArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
						window.draw(rightArrow);
						window.draw(leftArrow);
					}
					// highlight the AI character attributes
					else
					{
						unsigned int ArrowX = tempAIText.getPosition().x - 20;
						unsigned int ArrowY = tempAIText.getPosition().y + tempAIText.getLocalBounds().height / 2;
						leftArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
						ArrowX = tempAIText.getPosition().x + tempAIText.getLocalBounds().width + 20;
						rightArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
						window.draw(rightArrow);
						window.draw(leftArrow);
					}
				}
			}
			// highlight the exit button
			else if(isOnExit)
			{
				unsigned int ArrowX = (((AI_CHOOSE_ROW_HEIGHT) / 2) + (AI_CHOOSE_ROW_HEIGHT * rowSelected) + CHOOSE_AI_TOP_MARGIN_SIZE) * scaleX;
				unsigned int ArrowY = 300 * scaleY;
				downArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
				ArrowY = 550 * scaleY;
				downArrow.setPosition(sf::Vector2f((playButton.getPosition().x + playButton.getLocalBounds().width / 2),
																						playButton.getPosition().y));
				upArrow.setPosition(sf::Vector2f((playButton.getPosition().x + playButton.getLocalBounds().width / 2),
																					playButton.getPosition().y + playButton.getLocalBounds().height));
			}
			// highlight the current AI
			else
			{
				unsigned int ArrowX = (((AI_CHOOSE_ROW_HEIGHT) / 2) + (AI_CHOOSE_ROW_HEIGHT * rowSelected) + CHOOSE_AI_TOP_MARGIN_SIZE) * scaleX;
				unsigned int ArrowY = 300 * scaleY;
				downArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
				ArrowY = 550 * scaleY;
				upArrow.setPosition(sf::Vector2f(ArrowX, ArrowY));
			}

			AIIndex = AIIndex + 1;
		}
		window.draw(chooseAIText);
		window.draw(playButton);
		window.draw(selectText);
		window.draw(upArrow);
		window.draw(downArrow);
		window.display();
	}
}

int playGame(vector<AI> & AIs)
{
  //sf::Sprite playerSprite = LoadPlayerImage();
	srand(time(NULL));
  vector<vector<char>> mediumBoard;
  createBoard(mediumBoard);
  print(mediumBoard);
	bool endOfGame = false;
	char winOrLose = 'w';
	int tileTMP = 0;

	sf::Texture texture;
	if (!texture.loadFromFile(PLAYER_IMAGE_PATH))
		cout << "ERROR" << endl;
	sf::Sprite playerSprite(texture);

	double scaleX = ((double)sf::VideoMode::getDesktopMode().width)/1622;
	double scaleY = ((double)sf::VideoMode::getDesktopMode().height)/975;
	double scale = 0.5; 	//change this later to adapt to screen size
	sf::RectangleShape a[16][16];
	sf::Color grey(128,128,128);
	sf::Color darkGrey(192,192,192);
	sf::Color darkestGrey(215,215,215);

	// all images are loaded here
	mineImage.loadFromFile("Icons/mine.png");
	flag0Image.loadFromFile("Icons/0flag.png");
	flag1Image.loadFromFile("Icons/1flag.png");
	flag2Image.loadFromFile("Icons/2flag.png");
	flag3Image.loadFromFile("Icons/3flag.png");
	flag4Image.loadFromFile("Icons/4flag.png");
	flag5Image.loadFromFile("Icons/5flag.png");
	flag6Image.loadFromFile("Icons/6flag.png");
	flag7Image.loadFromFile("Icons/7flag.png");
	flag8Image.loadFromFile("Icons/8flag.png");
	ExplodedMineImage.loadFromFile("Icons/ExplodedMine.png");
	flagFlagImage.loadFromFile("Icons/flag.png");
	CharacterImage.loadFromFile("Icons/Gosnell.png");
	AIImage.loadFromFile("Icons/drtAI.png");
	defaultImage.loadFromFile("Icons/default.png");

	// all textures are loaded here
	mineTexture.loadFromImage(mineImage);
	flag0Texture.loadFromImage(flag0Image);
	flag1Texture.loadFromImage(flag1Image);
	flag2Texture.loadFromImage(flag2Image);
	flag3Texture.loadFromImage(flag3Image);
	flag4Texture.loadFromImage(flag4Image);
	flag5Texture.loadFromImage(flag5Image);
	flag6Texture.loadFromImage(flag6Image);
	flag7Texture.loadFromImage(flag7Image);
	flag8Texture.loadFromImage(flag8Image);
	ExplodedMineTexture.loadFromImage(ExplodedMineImage);
	flagFlagTexture.loadFromImage(flagFlagImage);
	CharacterTexture.loadFromImage(CharacterImage);
	AITexture.loadFromImage(AIImage);
	defaultTexture.loadFromImage(defaultImage);

	int iniTargetX;
	int iniTargetY;

	do
	{
		iniTargetX = rand() % BSIZE;
		iniTargetY = rand() % BSIZE;
	}
	while(mediumBoard[iniTargetX][iniTargetY] != 'O');

	playerX = iniTargetX ;
	playerY = iniTargetY ;

	for(int i = 0; i < BSIZE; i++)
	{
		for(int j = 0; j < BSIZE; j++)
		{
			sf::Sprite sprite;
			sprite.setTexture(defaultTexture, true);
			sprite.setPosition(25*scaleX+50*i*scaleX, 25*scaleY+50*j*scaleY);
			SetSize(sprite,50*scaleX,50*scaleY);
			b[i][j] = sprite;
		}
	}
	//vector<AI> AIs = loadAIs(mediumBoard, b);
	for(unsigned int i = 0; i < AIs.size(); i = i + 1)
	{
		AIs[i].selectNewTarget(mediumBoard, b);
	}
	
	sf::RenderWindow window(sf::VideoMode(950*scaleX, 950*scaleY), "Minesweeper");
	window.setFramerateLimit(FRAME_RATE);
	revealSurrondingTiles(iniTargetX, iniTargetY, mediumBoard,scaleX, scaleY);
	while(window.isOpen() && !endOfGame)
	{
    for(unsigned int i = 0; i < AIs.size(); i = i + 1)
    {
			if(AIs[i].isTimeToMove())
			{
				 AIs[i].moveTowardTarget();
			}
			if(AIs[i].isOnTarget())
			{
				AIs[i].revealTileUnder(mediumBoard, scaleX, scaleY);
				AIs[i].selectNewTarget(mediumBoard, b);
			}
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right)
				{
					if(playerX < BSIZE-1)
					{
						if(isValidMove(playerX+1,playerY))
                        {
                          playerX = playerX + 1 ;
                        }
					}
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					if(playerX > 0)
					{
						if(isValidMove(playerX-1,playerY))
                        {
                          playerX = playerX - 1 ;
                        }
					}
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					if(playerY > 0)
					{
						if(isValidMove(playerX,playerY-1))
                        {
                          playerY = playerY - 1 ;
                        }
					}
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if(playerY < BSIZE-1)
					{
						if(isValidMove(playerX,playerY+1))
                        {
                          playerY = playerY + 1 ;
                        }
					}
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					if (b[playerX][playerY].getTexture() != &flagFlagTexture)
					{
						// Get the mouse position in tile coordinates
						unsigned int i = playerX;
						unsigned int j = playerY;

						unsigned int boardY = j;
						unsigned int boardX = i;
						revealSurrondingTiles (boardX, boardY, mediumBoard, scaleX, scaleY);
						if(b[playerX][playerY].getTexture() == &mineTexture)
						{
						    cout << "BOOM YOU FOUND MARKOWSKY" << endl;
						    revealAllBomb(mediumBoard, scaleX, scaleY);
								endOfGame = true;
								winOrLose = 'l';
						}
					}
				}
				else if (event.key.code == sf::Keyboard::F)
				{
					if (b[playerX][playerY].getTexture() == &flagFlagTexture)
					{
						flagsPlaced --;
						sf::Sprite sprite;
						sprite.setTexture(defaultTexture, true);
						sprite.setPosition(25*scaleX+50*playerX*scaleX, 25*scaleY+50*playerY*scaleY);
						SetSize(sprite,50*scaleX,50*scaleY);
						b[playerX][playerY] = sprite;
					}
					else if (b[playerX][playerY].getTexture() == &defaultTexture)
					{
						flagsPlaced++;
						sf::Sprite sprite;
						sprite.setTexture(flagFlagTexture, true);
						sprite.setPosition(25*scaleX+50*playerX*scaleX, 25*scaleY+50*playerY*scaleY);
						SetSize(sprite,50*scaleX,50*scaleY);
						b[playerX][playerY] = sprite;
					}
				}
			}
		}
		window.clear(DARK_GREY);
		for(int i = 0; i < BSIZE; i++)
		{
			for(int j = 0; j < BSIZE; j++)
			{
				window.draw(b[i][j]);
			}
		}
		for(unsigned int i = 0; i < AIs.size(); i = i + 1)
		{
			sf::Sprite sprite(AIs[i].texture);
			sprite.setPosition(sf::Vector2f((25+50*AIs[i].x)*scaleX, (25+50*AIs[i].y)*scaleY));
			SetSize(sprite, scaleX * 50, scaleY * 50 );
			window.draw(sprite);
		}
		playerSprite.setPosition(sf::Vector2f((25+50*playerX)*scaleX, (25+50*playerY)*scaleY));
		SetSize(playerSprite, scaleX * 50, scaleY * 50 );
		//playerSprite.setScale(0.05,0.05);
		window.draw(playerSprite);
		//window.draw(shape);
		window.display();

		if (tilesUncovered != tileTMP)
		{
			tileTMP = tilesUncovered;
			//cout << "\t\t\tFlag Placed:" << flagsPlaced << endl;
			//cout << "\t\t\tTiles Covered: " << checkBoard() << endl;
			//cout << "\t\t\tTiles Found: " << tilesUncovered << endl;
		}

		if (((BSIZE * BSIZE) - NBOMBS) == tilesUncovered || checkBoard() == NBOMBS || flagsCorrect(mediumBoard))
		{
			endOfGame = true;
			winOrLose = 'w';
		}
		if (endOfGame)
		{
			endGame(winOrLose, scaleX, scaleY);
		}
	}
	return 0;
}
