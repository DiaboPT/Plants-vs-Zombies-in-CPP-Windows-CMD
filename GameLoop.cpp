// GameLoop.cpp

#include "GameLoop.hpp"
#include "FrameWork/FrameWork.hpp"

#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

const int res = 108;
const int width = 16, height = 9, distance = 5;
Coords size { width , height , distance };

// GameLoop function with Linux compatibility
void GameLoop() {

	// Game Start & Update Requirements
	bool gameloop = false;
	const int fps = 60;
	std::string output = "";

	// Extra Inicializations for especific game
	Levels level = Levels();
	int frameCount = 0;

	// Importante Cells
	CellContent Nothing, plantsCurrency, zombiesCurrency;
	int zombieTimeCount = 0;

	// Plants Cells
	const CellContent
		Peashooter = CellContent(COLOR(46) + std::string("P"), 4, 6, 1.5f), 
		Sunflower = CellContent(COLOR(220) + std::string("S"), 2, 6, 24.0f / 2), 
		CherryBomb = CellContent(COLOR(1) + std::string("C"), 6, 6, 1.2f),
		WallNut = CellContent(COLOR(208) + std::string("W"), 2, 72),
		RollingWallNut = CellContent(COLOR(208) + std::string("R"), 0, WallNut.Get_HP(), Peashooter.Get_Speed());

	// Zombies Cells
	const CellContent
		Basic = CellContent(COLOR(165) + std::string("Z"), 5, 10, 4.7f),
		ConeHead = CellContent(COLOR(208) + std::string("C"), Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed()),
		PoleVault = CellContent(COLOR(74) + std::string("P"), Basic.Get_Cost() * 4, Basic.Get_HP() * 2, Basic.Get_Speed() / 2),
		BucketHead = CellContent("\033[97m" + std::string("B"), Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed());

	// Boards Size
	int plantsBoardWidth = 0, plantsBoardHeight = 0, zombiesBoardWidth = 0, zombiesBoardHeight = 0, gameBoardWidth = 0, gameBoardHeight = 0;
	GameBoard plantsBoard, zombiesBoard, gameBoard;
	Coords plantsBoardSelection{}, zombiesBoardSelection{}, gameBoardSelection{};

	std::function<void()> ReStart = [&]() {

		// Console and console font size
		SetConsoleFontSize(res * .1f);
		SetConsoleSize(res, size);

		frameCount = 0;
		plantsCurrency = CellContent(COLOR(220) + std::string("C"), 1, 0, 10 / 2);
		zombiesCurrency = CellContent(plantsCurrency);
		zombieTimeCount = 1;
		gameBoardHeight = 1;

		float j = 5.0f;
		float k = 1.5f + (level.GetLevel().x * .5f);

		level.SetWinCondiction(pow(j, k));

		level.ClearTypes();

		switch (level.GetLevel().x) {
			case 0:
				level.SetPlantsTypes({ Peashooter });
				level.SetZombiesTypes({ Basic });

				gameBoardHeight = 1;
				break;
			case 1:
				level.SetPlantsTypes({ Peashooter, Sunflower });
				level.SetZombiesTypes({ Basic });

				gameBoardHeight = 3;
				break;
			case 2:
				level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb });
				level.SetZombiesTypes({ Basic, ConeHead });

				gameBoardHeight = 3;
				break;
			case 3:
				level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
				level.SetZombiesTypes({ Basic, ConeHead });

				gameBoardHeight = 5;
				break;
			case 4:
				level.SetPlantsTypes({ RollingWallNut });
				level.SetZombiesTypes({ Basic, ConeHead });

				zombieTimeCount = 5;
				zombiesCurrency.Set_Cost(4);
				gameBoardHeight = 5;
				break;
			case 5:
				level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
				level.SetZombiesTypes({ Basic, ConeHead, PoleVault });

				gameBoardHeight = 5;
				break;
			case 6:
				level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
				level.SetZombiesTypes({ Basic, ConeHead, PoleVault });

				gameBoardHeight = 5;
				break;
			case 7:
				level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
				level.SetZombiesTypes({ Basic, ConeHead, PoleVault, BucketHead });

				gameBoardHeight = 5;
				break;
		}

		plantsBoardWidth = (int)level.GetPlantsTypes().size() + 2;
		zombiesBoardWidth = (int)level.GetZombiesTypes().size() + 2;

		// Create the boards
		plantsBoardHeight = 2, zombiesBoardHeight = 2, gameBoardWidth = 9;
		plantsBoard = GameBoard(plantsBoardWidth, plantsBoardHeight);
		plantsBoardSelection = { 2 , 0 };
		zombiesBoard = GameBoard(zombiesBoardWidth, zombiesBoardHeight);
		zombiesBoardSelection = { 2 , 0 };

		gameBoard = GameBoard(gameBoardWidth, gameBoardHeight);
		gameBoardSelection = { 0 , 0 };

		// Populate the plantsBoard
		for (int y = 0; y < plantsBoardHeight; y++) {
			for (int x = 0; x < plantsBoardWidth; x++) {
				if (x == 0) {
					plantsBoard.SetCell({ x , y }, y == 0 ? plantsCurrency : CellContent());
				}
				else if (x == 1) {
					plantsBoard.SetCell({ x , y }, CellContent()); // Empty cell
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

		};

	// Start
	level.SetLevel({ 4 , 0 });
	Start(gameloop, ReStart);

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

				case 'r': case 'R': ReStart();

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
				PlayCollectSunSound();
				plantsCurrency.Add_Cost(1);
				zombiesCurrency.Add_Cost(zombieTimeCount);
			}

			for (int y = 0; y < gameBoardHeight; y++) {
				for (int x = 0; x < gameBoardWidth; x++) {

					// Sunflowers Currency Update
					int sunflowerCount = 0;
					bool hasSunflowerHere = gameBoard.GetCell({ x, y }).Get_Name() == Sunflower.Get_Name();
					if (hasSunflowerHere) {
						PlayCollectSunSound();
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
							PlayCherryBombExplodeSound();

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
												gameBoard.SetCell({ i, j }, Nothing);
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

							std::vector<int> possibleLines(0);
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

				for (int x = gameBoardWidth -1; x >= 0; x--) {

					// Rolling Nut
					bool hasRollingWallNut = gameBoard.GetCell({ x, y }).Get_Name() == RollingWallNut.Get_Name();
					if (hasRollingWallNut) {

						if (x == size.x - 1) {
							gameBoard.SetCell({ x, y }, Nothing);
						}
						else {
							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1, y }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
									hasZombieNext = true;
									break;
								}
							}

							if (fmod(frameCount, fps * gameBoard.GetCell({ x,y }).Get_Speed()) < 1) {
								if (hasZombieNext) {

									CellContent damagedZombie = gameBoard.GetCell({ x - 1, y });
									damagedZombie.Add_HP(Basic.Get_HP());
									if (damagedZombie.Get_HP() != 0) {
										gameBoard.SetCell({ x - 1, y }, damagedZombie);
									}
									else {
										gameBoard.SetCell({ x - 1, y }, Nothing);
									}

									gameBoard.SetCell({ x + 1 , y + 1 }, gameBoard.GetCell({ x, y }));
									gameBoard.SetCell({ x, y }, Nothing);

									PlayZombieHitSound();
								}
								else {
									// Moves a cell foward
									gameBoard.SetCell({ x + 1 , y }, gameBoard.GetCell({ x , y }));
									gameBoard.SetCell({ x, y }, Nothing);
								}
							}
						}
					}
				}
			}

			plantsBoard.SetCell({ 0, 1 }, CellContent(std::to_string(plantsCurrency.Get_Cost()), plantsCurrency.Get_Cost(), 0));
			zombiesBoard.SetCell({ 0, 1 }, CellContent(std::to_string(zombiesCurrency.Get_Cost()), zombiesCurrency.Get_Cost(), 0));

			output = RESET;
			output += "World " + std::to_string(level.GetLevel().y + 1) + "-" + std::to_string(level.GetLevel().x + 1);
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
			output += 
				gameBoard.GetCell(gameBoardSelection).Get_HP() > 9 ? 
				std::to_string(gameBoard.GetCell(gameBoardSelection).Get_HP()) + " " : 
				"0" + std::to_string(gameBoard.GetCell(gameBoardSelection).Get_HP());
			output += "\n";

			output += "\nZombies Board:\n" + std::string(PLANTRESET) + zombiesBoard.DrawBoard(zombiesBoardSelection, COLOR(165), PLANTRESET) + '\n';
			output += RESET;

			// Win
			if (zombiesCurrency.Get_Cost() >= level.GetWinCondiction()) {

				ClearScreen();
				std::cout << RESET;
				std::cout << "+----------+\n";
				std::cout << "| You Win! |\n";
				std::cout << "+----------+\n";
				std::cout << RESET;
				rest(3000);
				ClearScreen();

				level.AddLevel();
				output = "";
				ReStart();
			}
			frameCount++;

		});

	ClearScreen();

	std::cout << RESET;
	std::cout << "\n";
	std::cout << "+---------------------+\n";
	std::cout << "| Thanks for playing! |\n";
	std::cout << "| Time: " << std::to_string(frameCount / fps) << (" seconds!  ") << " |\n";
	std::cout << "+---------------------+\n";
	std::cout << RESET;
}
