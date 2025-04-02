// GameLoop.cpp
#include "header.hpp"
#include "FrameWork.hpp"
using std::max;
using std::min;

#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

bool static Menu() {
#ifndef _WIN32
	if (!isSoxInstalled()) {
		installSox();
		getKeyPressed();
	}
#endif
	ClearScreen();
	bool returned = false;
	char key = 'A';
	int selection = 0;
	bool menuloop = true;
	while (menuloop) {
		ResetCursor();
		std::cout << COLOR(46) << "+-" << "------" << "---" << "---------" << "------" << "-----" << "------" << "------" << "------" << "---" << "-----" << "---" << "-----" << "-+" << RESET << "\n";
		std::cout << COLOR(46) << "| " << COLOR(46) << "PPPPP " << "   " << COLOR(46) << "VV     VV" << " SSSS " << "     " << COLOR(165) << "ZZZZZZ" << "      " << COLOR(46) << " CCCCC" << "   " << "  +  " << "   " << "  +  " << COLOR(46) << " |" << "\n";
		std::cout << COLOR(208) << "| " << COLOR(46) << "PP  PP" << "   " << COLOR(208) << " VV   VV " << "SS   S" << "     " << COLOR(165) << "   ZZ " << "      " << COLOR(208) << "CCC   " << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		std::cout << COLOR(208) << "| " << COLOR(46) << "PPPPP " << "   " << COLOR(208) << "  VV VV  " << "  SS  " << "     " << COLOR(165) << "  ZZ  " << "      " << COLOR(208) << "CC    " << "   " << "+++++" << "   " << "+++++" << COLOR(208) << " |" << "\n";
		std::cout << COLOR(208) << "| " << COLOR(46) << "PP    " << "   " << COLOR(208) << "   VVV   " << "S   SS" << "..   " << COLOR(165) << " ZZ   " << "      " << COLOR(208) << "CCC   " << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		std::cout << COLOR(208) << "| " << COLOR(46) << "PP    " << "   " << COLOR(208) << "    V    " << " SSSS " << "..   " << COLOR(165) << "ZZZZZZ" << "      " << COLOR(208) << " CCCCC" << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		std::cout << COLOR(208) << "+-" << "------" << "---" << "---------" << "------" << "-----" << "------" << "------" << "------" << "---" << "-----" << "---" << "-----" << "-+" << RESET << "\n";

		std::cout << RESET;
		std::cout << "| " << "        Menu Keys         " << " |" << "\n";
		std::cout << "+-" << "--------------------------" << "-+" << "\n";
		std::cout << "| " << "     W | Move up          " << " |" << "\n";
		std::cout << "| " << "     S | Move down        " << " |" << "\n";
		std::cout << "+-" << "--------------------------" << "-+" << "\n";
		std::cout << "| " << "      Overall Keys        " << " |" << "\n";
		std::cout << "+-" << "--------------------------" << "-+" << "\n";
		std::cout << "| " << " Space | Select           " << " |" << "\n";
		std::cout << "| " << "Escape | Exit             " << " |" << "\n";
		std::cout << "+-" << "--------------------------" << "-+" << "\n";

		std::cout << "| " << " Options " << " |" << "\n";
		std::cout << "+-----------+" << "\n";
		std::cout << "| " << ((selection == 0) ? COLOR(46) + string("> Start <") + RESET + string(" |") : string("  Start   |")) << "\n";
		std::cout << "| " << ((selection == 1) ? COLOR(46) + string("> Guide <") + RESET + string(" | <- Recommended") : string("  Guide   | <- Recommended")) + string("\n");
		std::cout << "| " << ((selection == 2) ? COLOR(46) + string("> Quit  <") + RESET + string(" |") : string("  Quit    |")) << "\n";
		std::cout << "+-----------+" << "\n";

		key = getKeyPressed();
		switch (key) {
		case 'w':
		case 'W':
			if (selection > 0) selection--;
			break;
		case 's':
		case 'S':
			if (selection < 2) selection++;
			break;

		case ' ':
			switch (selection) {
			case 0:
				menuloop = false;
				returned = true;
				break;
			case 1:
				for (int i = 1; i <= 4; i++) {
					switch (i) {
					case 1:
						ClearScreen();

						std::cout << "Plants Board is where the plants are and where you can select them" << "\n";
						std::cout << "\n";

						std::cout << "+---+---+---+-----+                                " << "\n";
						std::cout << "| C |   | P | ... | <- Plants (Colored if selected)" << "\n";
						std::cout << "+---+---+---+-----+                                " << "\n";
						std::cout << "| A |   | B | ... | <- Cost of the plant on top    " << "\n";
						std::cout << "+---+---+---+-----+                                " << "\n";
						std::cout << "\n";

						std::cout << "C - Currency / A - Your currency right now                  " << "\n";
						std::cout << "P - Plant    / B - Currency you need in A to place the plant" << "\n";
						std::cout << "\n";

						std::cout << "Keys for Plants Board:" << "\n";
						std::cout << "- Q - Move to the left in the Plant Board" << "\n";
						std::cout << "- E - Move to the right in the Plant Board" << "\n";
						std::cout << "\n";

						getKeyPressed();
						break;
					case 2:
						ClearScreen();

						std::cout << "Game Board is where you can place plants to actually affect the game" << "\n";
						std::cout << "\n";

						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "| S | P |   | ... |" << "\n";
						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "| S |   |   | ... |" << "\n";
						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "|   | W | A | ... |" << "\n";
						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "|   | W | B | ... |" << "\n";
						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "|   |   | C | ... |" << "\n";
						std::cout << "+---+---+---+-----+" << "\n";
						std::cout << "\n";

						std::cout << "S - Sunflower   (Plant  - gives more currency each time)" << "\n";
						std::cout << "P - Peashooter  (Plant  - kills zombies)" << "\n";
						std::cout << "W - Wall-Nut    (Plant  - tanky)" << "\n";
						std::cout << "\n";

						std::cout << "A - Basic       (Zombie - normal)" << "\n";
						std::cout << "B - Cone-Head   (Zombie - double stats from Basic)" << "\n";
						std::cout << "C - Bucket-Head (Zombie - triple stats from Basic)" << "\n";
						std::cout << "\n";

						std::cout << "Keys for Plants Board:" << "\n";
						std::cout << "- W - Move to the up in the Game Board" << "\n";
						std::cout << "- A - Move to the left in the Game Board" << "\n";
						std::cout << "- S - Move to the down in the Game Board" << "\n";
						std::cout << "- D - Move to the right in the Game Board" << "\n";
						std::cout << "\n";

						getKeyPressed();
						break;
					case 3:
						ClearScreen();

						std::cout << "Zombie Board is where the zombies are and where the pc can select them" << "\n";
						std::cout << "\n";

						std::cout << "+---+---+---+-----+                                 " << "\n";
						std::cout << "| C |   | Z | ... | <- Zombies (Colored if selected)" << "\n";
						std::cout << "+---+---+---+-----+                                 " << "\n";
						std::cout << "| A |   | B | ... | <- Cost of the zombie on top    " << "\n";
						std::cout << "+---+---+---+-----+                                 " << "\n";
						std::cout << "\n";

						std::cout << "C - Currency / A - Pc currency right now                   " << "\n";
						std::cout << "Z - Zombie   / B - Currency pc needs in A to place a zombie" << "\n";
						std::cout << "\n";

						getKeyPressed();
						break;
					case 4:
						ClearScreen();

						std::cout << "Objective: Survive";
						std::cout << "\n";

						getKeyPressed();
						break;
					}

					ClearScreen();

				}
				break;
			case 2:
				menuloop = false;
				break;
			}
			break;
		case 27:
			menuloop = false;
			break;
		}
	}
	return returned;
}

// GameLoop std::function with Linux compatibility
void GameLoop() {

	SetConsoleFontSize(26);
	SetConsoleSize(1600, 900);

	int frameCount = 0;
	bool gameloop = false;
	const int fps = 60;
	string output = "", old_output = "";
	Levels level = Levels();

	CellContent Nothing, plantsCurrency, zombiesCurrency;
	int zombieTimeCount = 0;

	CellContent Peashooter, Sunflower, CherryBomb, WallNut;
	vector<CellContent> plantsTypes;

	CellContent Basic, ConeHead, BucketHead, PoleVault;
	vector<CellContent> zombiesTypes;

	int plantsBoardWidth = 0, zombiesBoardWidth = 0, plantsBoardHeight = 0, zombiesBoardHeight = 0, gameBoardWidth = 0, gameBoardHeight = 0;

	GameBoard plantsBoard = GameBoard(), zombiesBoard = GameBoard(), gameBoard = GameBoard();
	Coords plantsBoardSelection{ 0 , 0 }, zombiesBoardSelection{ 0 , 0 }, gameBoardSelection{ 0 , 0 };

	// Start
	Start(gameloop,
		[&] {

			plantsCurrency = CellContent(COLOR(220) + std::string("C"), 1, 0, 10 / 2);
			zombiesCurrency = CellContent(plantsCurrency);
			zombieTimeCount = 1;

			// Define plant and zombie objects manually
			Peashooter = CellContent(COLOR(46) + std::string("P"), 4, 6, 1.5f);
			Sunflower = CellContent(COLOR(220) + std::string("S"), 2, 6, 24.0f / 2);
			CherryBomb = CellContent(COLOR(1) + std::string("C"), 6, 6, 1.2f);
			WallNut = CellContent(COLOR(208) + std::string("W"), 2, 72);
			plantsTypes = {
				Peashooter,
				Sunflower,
				CherryBomb,
				WallNut
			};

			Basic = CellContent(COLOR(165) + std::string("Z"), 5, 10, 6.5f);
			ConeHead = CellContent(COLOR(208) + std::string("C"), Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed());
			BucketHead = CellContent("\033[97m" + std::string("B"), Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed());
			PoleVault = CellContent(COLOR(74) + std::string("P"), Basic.Get_Cost() * 4, Basic.Get_HP() * 2, Basic.Get_Speed() / 2);
			zombiesTypes = {
				Basic,
				ConeHead,
				BucketHead,
				PoleVault
			};

			int i = level.GetLevel().x - 1;
			float j = 5.0f;
			float k = 1.5f + (level.GetLevel().x * .5f);

			level.AddPlantsTypes(plantsTypes[i]);
			level.AddZombiesTypes(zombiesTypes[i]);

			level.SetWinCondiction(pow(j, k));

			// Calculate board width dynamically
			plantsBoardWidth = (int)level.GetPlantsTypes().size() + 2;  // +2 for currency and empty slot
			zombiesBoardWidth = (int)level.GetZombiesTypes().size() + 2;
			plantsBoardHeight = 2, zombiesBoardHeight = 2, gameBoardWidth = 9, gameBoardHeight = 1;

			// Create the boards
			plantsBoard = GameBoard(plantsBoardWidth, plantsBoardHeight);
			plantsBoardSelection = { 2,0 };
			zombiesBoard = GameBoard(zombiesBoardWidth, zombiesBoardHeight);
			zombiesBoardSelection = { 2,0 };

			gameBoard = GameBoard(gameBoardWidth, gameBoardHeight);
			gameBoardSelection = { 0,0 };

			// Populate the plantsBoard
			for (int y = 0; y < plantsBoardHeight; y++) {
				for (int x = 0; x < plantsBoardWidth; x++) {
					if (x == 0) {
						plantsBoard.SetCell({ x, y }, y == 0 ? plantsCurrency : CellContent());
					}
					else if (x == 1) {
						plantsBoard.SetCell({ x, y }, CellContent()); // Empty cell
					}
					else {
						int index = x - 2;
						if (index < level.GetPlantsTypes().size()) {
							plantsBoard.SetCell({ x, y }, y == 0 ? level.GetPlantsTypes()[index] :
								CellContent(std::to_string(level.GetPlantsTypes()[index].Get_Cost()), level.GetPlantsTypes()[index].Get_Cost(), 0));
						}
					}
				}
			}

			// Populate the zombiesBoard
			for (int y = 0; y < zombiesBoardHeight; y++) {
				for (int x = 0; x < zombiesBoardWidth; x++) {
					if (x == 0) {
						zombiesBoard.SetCell({ x, y }, y == 0 ? zombiesCurrency : CellContent());
					}
					else if (x == 1) {
						zombiesBoard.SetCell({ x, y }, CellContent()); // Empty cell
					}
					else {
						int index = x - 2;
						if (index < level.GetZombiesTypes().size()) {
							zombiesBoard.SetCell({ x, y }, y == 0 ? level.GetZombiesTypes()[index] :
								CellContent(std::to_string(level.GetZombiesTypes()[index].Get_Cost()), level.GetZombiesTypes()[index].Get_Cost(), 0));
						}
					}
				}
			}

		});

	ClearScreen();

	// Update
	Update(gameloop, output, fps,
		[&] {

			// Detects if key is press
			if (isKeyPressed()) {
				char key = getKeyPressed();
				bool hasMoney = plantsCurrency.Get_Cost() >= plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).Get_Cost();

				bool canPlace = true;
				for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
					if (gameBoard.GetCell({ gameBoardSelection }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
						canPlace = false;
						break;
					}
				}

				switch (key) {
				case 'q': case 'Q': if (plantsBoardSelection.x > 2) plantsBoardSelection.x--; break;
				case 'e': case 'E': if (plantsBoardSelection.x < (level.GetPlantsTypes().size() + 2) - 1) plantsBoardSelection.x++; break;

				case 'w': case 'W': if (gameBoardSelection.y > 0) gameBoardSelection.y--; break;
				case 'a': case 'A': if (gameBoardSelection.x > 0) gameBoardSelection.x--; break;
				case 's': case 'S': if (gameBoardSelection.y < gameBoardHeight - 1) gameBoardSelection.y++; break;
				case 'd': case 'D': if (gameBoardSelection.x < gameBoardWidth - 1) gameBoardSelection.x++; break;

				case ' ':
					if (hasMoney && canPlace) {
						gameBoard.SetCell(gameBoardSelection, plantsBoard.GetCell(plantsBoardSelection));
						plantsCurrency.Add_Cost(-plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).Get_Cost());
					}
					break;
				case 27:
					gameloop = false;
					break;
				}
			}

			// Currency Update
			if (fmod(frameCount, fps * plantsCurrency.Get_Speed()) < 1) {
				plantsCurrency.Add_Cost(1);
				zombiesCurrency.Add_Cost(zombieTimeCount);
			}

			for (int y = 0; y < gameBoardHeight; y++) {
				for (int x = 0; x < gameBoardWidth; x++) {

					// Sunflowers Currency Update
					int sunflowerCount = 0;
					bool hasSunflowerHere = gameBoard.GetCell({ x, y }).Get_Name() == Sunflower.Get_Name();
					if (hasSunflowerHere) {
						if (fmod(frameCount, fps * Sunflower.Get_Speed()) < 1) {
							sunflowerCount++;
						}
						plantsCurrency.Add_Cost(sunflowerCount);  // Increase cost based on Sunflowers
					}

					// Peashooter Shoots Update
					bool hasPeashooterHere = gameBoard.GetCell({ x, y }).Get_Name() == Peashooter.Get_Name();
					if (hasPeashooterHere) {

						if (fmod(frameCount, fps * Peashooter.Get_Speed()) < 1) {

							for (int i = x + 1; i < gameBoardWidth; i++) {

								bool hasZombieNext = false;
								for (int j = 0; j < level.GetZombiesTypes().size(); j++) {
									if (gameBoard.GetCell({ i, y }).Get_Name() == level.GetZombiesTypes()[j].Get_Name()) {
										hasZombieNext = true;
										break;
									}
								}
								if (hasZombieNext) {

									// Damages next cell
									CellContent damagedZombie = gameBoard.GetCell({ i, y });
									damagedZombie.Add_HP(-1);
									if (damagedZombie.Get_HP() > 0) {
										gameBoard.SetCell({ i, y }, damagedZombie);
									}
									else {
										gameBoard.SetCell({ i, y }, Nothing);
										zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() / 2);
									}
									PlayZombieHitSound();
									break;
								}
							}
						}
					}

					// Cherry Bomb Update
					bool hasCherryBombHere = gameBoard.GetCell({ x, y }).Get_Name() == CherryBomb.Get_Name();
					if (hasCherryBombHere) {

						if (fmod(frameCount, fps * CherryBomb.Get_Speed()) < 1) {

							for (int j = max(0, y - 1); j <= min(gameBoardHeight - 1, y + 1); j++) {
								for (int i = max(0, x - 1); i <= min(gameBoardWidth - 1, x + 1); i++) {

									for (int k = 0; k < level.GetZombiesTypes().size(); k++) {
										if (gameBoard.GetCell({ i, j }).Get_Name() == level.GetZombiesTypes()[k].Get_Name()) {

											CellContent damagedZombie = gameBoard.GetCell({ i, j });
											damagedZombie.Add_HP(-90);
											if (damagedZombie.Get_HP() > 0) {
												gameBoard.SetCell({ i, j }, damagedZombie);
											}
											else {
												gameBoard.SetCell({ i, y }, Nothing);
												zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() / 2);
											}
											PlayZombieHitSound();
										}
									}
								}
							}
							gameBoard.SetCell({ x, y }, Nothing);
						}
					}

					// Zombies movement + damages next Plant
					bool hasZombie = false;
					for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
						if (gameBoard.GetCell({ x, y }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
							hasZombie = true;
							break;
						}
					}
					if (hasZombie) {

						if (x == 0) {
							// Finishs the game
							gameloop = false;
						}
						else {
							bool hasPlantNext = false;
							for (int i = 0; i < level.GetPlantsTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1, y }).Get_Name() == level.GetPlantsTypes()[i].Get_Name()) {
									hasPlantNext = true;
									break;
								}
							}

							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1, y }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
									hasZombieNext = true;
									break;
								}
							}

							if (hasPlantNext) {

								// Damages next cell
								if (frameCount % fps == 0) {
									CellContent damagedPlant = gameBoard.GetCell({ x - 1, y });
									damagedPlant.Add_HP(-1);
									if (damagedPlant.Get_HP() != 0) {
										gameBoard.SetCell({ x - 1, y }, damagedPlant);
									}
									else {
										gameBoard.SetCell({ x - 1, y }, Nothing);
									}
									PlayZombieBiteSound();
								}
							}
							else if (!hasPlantNext && !hasZombieNext) {
								if (fmod(frameCount, fps * gameBoard.GetCell({ x,y }).Get_Speed()) < 1) {

									// Moves a cell foward
									gameBoard.SetCell({ x - 1, y }, gameBoard.GetCell({ x,y }));
									gameBoard.SetCell({ x, y }, Nothing);
								}
							}
						}
					}

					// Buy zombies
					bool zombieHasMoney = zombiesCurrency.Get_Cost() >= Basic.Get_Cost();
					if (zombieHasMoney) {

						if ((frameCount % fps) == 0) {

							CellContent zombie;
							for (int i = (int)level.GetZombiesTypes().size() - 1; i >= 0; i--) {
								if (zombiesCurrency.Get_Cost() >= level.GetZombiesTypes()[i].Get_Cost()) {
									zombie = level.GetZombiesTypes()[i];
									zombieTimeCount++;
									break;
								}
							}

							vector<int> possibleLines(0);
							if (true) {
								int k = 0;
								for (int j = 0; j < gameBoardHeight; j++) {
									if (gameBoard.GetCell({ gameBoardWidth - 1, j }).Get_Name() == Nothing.Get_Name()) {
										possibleLines.resize(static_cast<std::vector<int, std::allocator<int>>::size_type>(k) + 1);
										possibleLines[k] = j;
										k++;
									}
								}
							}

							if (possibleLines.size() > 0) {
								int randomLine = Random::FromList(possibleLines);
								gameBoard.SetCell({ gameBoardWidth - 1, randomLine }, zombie);
								zombiesCurrency.Add_Cost(-zombie.Get_Cost());
							}
						}
					}
				}
			}

			plantsBoard.SetCell({ 0, 1 }, CellContent(std::to_string(plantsCurrency.Get_Cost()), plantsCurrency.Get_Cost(), 0));
			zombiesBoard.SetCell({ 0, 1 }, CellContent(std::to_string(zombiesCurrency.Get_Cost()), zombiesCurrency.Get_Cost(), 0));

			output = RESET;
			output += "World " + std::to_string(level.GetLevel().y) + "-" + std::to_string(level.GetLevel().x);
			output += RESET;

			output += RESET;
			output += "\nObjective: " + std::to_string(int(level.GetWinCondiction())) + " zombies currency\n";
			output += RESET;

			output += RESET;
			output += "\nPlants Board:\n" + plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET);
			output += RESET;

			output += "\nGame Board:\n" + gameBoard.DrawBoard(gameBoardSelection, COLOR(220), GAMERESET);
			output += RESET;

			output += "Selected: ";
			output += "Name: ";
			output += std::string(gameBoard.GetCell(gameBoardSelection).Get_Name());
			output += RESET;
			output += " | HP: ";
			output += gameBoard.GetCell(gameBoardSelection).Get_HP() > 9 ? std::to_string(gameBoard.GetCell(gameBoardSelection).Get_HP()) + " " : "0" + std::to_string(gameBoard.GetCell(gameBoardSelection).Get_HP()) + " ";
			output += "\n";

			output += "\nZombies Board:\n" + std::string(PLANTRESET) + zombiesBoard.DrawBoard(zombiesBoardSelection, COLOR(165), PLANTRESET) + '\n';
			output += RESET;

			// Draw on screen
			if (output != old_output) {
				ResetCursor();
				std::cout << output;
				old_output = output;
			}

			// Win
			if (zombiesCurrency.Get_Cost() >= level.GetWinCondiction()) {
				frameCount = 0;
				plantsCurrency = CellContent(COLOR(220) + std::string("C"), 1, 0, 10);
				zombiesCurrency = plantsCurrency;
				zombieTimeCount = 1;

				level.AddLevel();

				if (true) {
					int i = level.GetLevel().x - 1;
					float j = 5.0f;
					float k = 1.5f + (level.GetLevel().x * .5f);

					level.AddPlantsTypes(plantsTypes[i]);
					level.AddZombiesTypes(zombiesTypes[i]);

					level.SetWinCondiction(pow(j, k));

				}

				plantsBoardWidth = (int)level.GetPlantsTypes().size() + 2;  // +2 for currency and empty slot
				zombiesBoardWidth = (int)level.GetZombiesTypes().size() + 2;

				switch (gameBoardHeight) {
				case 1:
				case 3:
					gameBoardHeight += 2;
					break;
				}

				plantsBoard = GameBoard(plantsBoardWidth, plantsBoardHeight);
				plantsBoardSelection = { 2,0 };
				zombiesBoard = GameBoard(zombiesBoardWidth, zombiesBoardHeight);
				zombiesBoardSelection = { 2,0 };

				gameBoard = GameBoard(gameBoardWidth, gameBoardHeight);
				gameBoardSelection = { 0,0 };

				// Populate the plantsBoard
				for (int y = 0; y < plantsBoardHeight; y++) {
					for (int x = 0; x < plantsBoardWidth; x++) {
						if (x == 0) {
							plantsBoard.SetCell({ x, y }, y == 0 ? plantsCurrency : CellContent());
						}
						else if (x == 1) {
							plantsBoard.SetCell({ x, y }, CellContent()); // Empty cell
						}
						else {
							int index = x - 2;
							if (index < level.GetPlantsTypes().size()) {
								plantsBoard.SetCell({ x, y }, y == 0 ? level.GetPlantsTypes()[index] :
									CellContent(std::to_string(level.GetPlantsTypes()[index].Get_Cost()), level.GetPlantsTypes()[index].Get_Cost(), 0));
							}
						}
					}
				}

				// Populate the zombiesBoard
				for (int y = 0; y < zombiesBoardHeight; y++) {
					for (int x = 0; x < zombiesBoardWidth; x++) {
						if (x == 0) {
							zombiesBoard.SetCell({ x, y }, y == 0 ? zombiesCurrency : CellContent());
						}
						else if (x == 1) {
							zombiesBoard.SetCell({ x, y }, CellContent()); // Empty cell
						}
						else {
							int index = x - 2;
							if (index < level.GetZombiesTypes().size()) {
								zombiesBoard.SetCell({ x, y }, y == 0 ? level.GetZombiesTypes()[index] :
									CellContent(std::to_string(level.GetZombiesTypes()[index].Get_Cost()), level.GetZombiesTypes()[index].Get_Cost(), 0));
							}
						}
					}
				}

				ClearScreen();
				std::cout << RESET;
				std::cout << "+----------+\n";
				std::cout << "| You Win! |\n";
				std::cout << "+----------+\n";
				std::cout << RESET;
				rest(3000);
				ClearScreen();
			}
			frameCount++;

		});

	std::cout << RESET;
	std::cout << "\n";
	std::cout << "+---------------------+\n";
	std::cout << "| Thanks for playing! |\n";
	std::cout << "| Time: " << std::to_string(frameCount / fps) << (" seconds!  ") << " |\n";
	std::cout << "+---------------------+\n";
	std::cout << RESET;
}
