// GameLoop.cpp

#include "GameLoop.hpp"
#include "cppFrameWork/FrameWork.hpp"

#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

const int res = 108;
const int width = 16, height = 9, distance = 5;
Coords size{ width , height , distance };

std::vector<CellContent> ChooseSeeds(std::vector<CellContent> Seeds, GameBoard plantsBoard, Coords plantsBoardSelection) {
	std::vector<CellContent> vectorCellContent;

	Coords seedsTableSelection{}, seedsTableSize{ 8 , 6 };
	GameBoard seedsTable = GameBoard(seedsTableSize);

	for (int y = 0; y < seedsTableSize.y; y++) {
		for (int x = 0; x < seedsTableSize.x; x++) {
			for (int i = 0; i < Seeds.size(); i++) {
				seedsTable.SetCell({ x , y }, Seeds[i]);
			}
		}
	}

	bool gameloop = true;
	std::string output = "";
	float fps = 60;

	Update(gameloop, output, fps, [&] {

		// Detects if key is press
		if (isKeyPressed()) {
			char key = getKeyPressed();

			switch (key) {
			case 'q': case 'Q': if (plantsBoardSelection.x > 0) plantsBoardSelection.x--; break;
			case 'e': case 'E': if (plantsBoardSelection.x < (plantsBoard.GetGrid().x) - 1) plantsBoardSelection.x++; break;

			case 'w': case 'W': if (seedsTableSelection.y > 0) seedsTableSelection.y--; break;
			case 'a': case 'A': if (seedsTableSelection.x > 0) seedsTableSelection.x--; break;
			case 's': case 'S': if (seedsTableSelection.y < seedsTableSize.y - 1) seedsTableSelection.y++; break;
			case 'd': case 'D': if (seedsTableSelection.x < seedsTableSize.x - 1) seedsTableSelection.x++; break;

			case ' ': plantsBoard.SetCell(seedsTableSelection, plantsBoard.GetCell(plantsBoardSelection));
				break;
			case 27:
				gameloop = false;
				break;
			}
		}

		output += RESET;
		output += "\nPlants Board:\n" + plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET);
		output += RESET;

		output += "\nSeeds Board:\n" + seedsTable.DrawBoard(seedsTableSelection, COLOR(220), GAMERESET);
		output += RESET;

		output += "Selected: ";
		output += "Name: ";
		output += std::string(seedsTable.GetCell(seedsTableSelection).Get_Name());
		output += RESET;
		output += " | HP: ";
		output +=
			seedsTable.GetCell(seedsTableSelection).Get_HP() > 9 ?
			std::to_string(seedsTable.GetCell(seedsTableSelection).Get_HP()) + " " :
			"0" + std::to_string(seedsTable.GetCell(seedsTableSelection).Get_HP());
		output += "\n";

		});

	for (int i = 0; i < vectorCellContent.size(); i++) {
		vectorCellContent[i] = plantsBoard.GetCell({ i });
	}

	return vectorCellContent;
}

bool AreAllZombiesGone(Coords gameBoardSize, Levels level, GameBoard gameBoard) {
	for (int y = 0; y < gameBoardSize.y; y++) {
		for (int x = 0; x < gameBoardSize.x; x++) {
			for (const auto& z : level.GetZombiesTypes()) {
				if (gameBoard.GetCell({ x, y }).Get_Name() == z.Get_Name()) {
					return false;
				}
			}
		}
	}
	return true;
}


// GameLoop function with Linux compatibility
void GameLoop() {

	// Extra Inicializations for especific game
	Levels level = Levels();
	bool isLastFlag = false;
	bool winConditionReached = false;
	bool flagZombieSpawned = false;
	int frameCount = 0;

	// Importante Cells
	CellContent Nothing, plantsCurrency, zombiesCurrency;
	int zombieTimeCount = 0;

	// Plants Cells
	// Info : https://plantsvszombies.fandom.com/wiki/Plants_(PvZ)
	const CellContent
		Peashooter = CellContent(COLOR(46) + std::string("P"), 4, 6, 1.5f),
		Sunflower = CellContent(COLOR(220) + std::string("S"), 2, 6, 24.0f / 2),
		CherryBomb = CellContent(COLOR(1) + std::string("C"), 6, 6, 1.2f),
		WallNut = CellContent(COLOR(208) + std::string("W"), 2, 72),
		RollingWallNut = CellContent(COLOR(208) + std::string("R"), 0, WallNut.Get_HP(), Peashooter.Get_Speed()),
		PotatoMine,
		SnowPea,
		Chomper,
		Repeater = CellContent(COLOR(46) + std::string("R"), Peashooter.Get_Cost() * 2, 6, Peashooter.Get_Speed() * .5f),
		PuffShroom,
		SunShroom,
		FumeShroom,
		GraveBuster,
		HypnoShroom,
		ScaredyShroom,
		IceShroom,
		DoomShroom,
		LilyPad,
		Squash,
		Threepeater,
		TangleKelp,
		Jalapeno,
		SpikeWeed,
		TorchWood,
		TallNut = CellContent(COLOR(208) + std::string("T"), WallNut.Get_Cost() * 2.25f, WallNut.Get_HP() * 2.25f),
		SeaShroom,
		Plantern,
		Cactus,
		Blover,
		SplitPea,
		StarFruit,
		Pumpkin,
		MagnetShroom,
		CabbagePult,
		FlowerPot,
		KernelPult,
		CoffeBean,
		Garlic,
		UmbrellaLeaf,
		Marigold = CellContent(COLOR(0) + std::string("M"), Sunflower.Get_Cost(), Sunflower.Get_HP(), Sunflower.Get_Speed()),
		MelonPult,
		GatlingPea = CellContent(COLOR(46) + std::string("G"), Repeater.Get_Cost() * 2, 6, Repeater.Get_Speed() * .5f),
		TwinSunFlower = CellContent(COLOR(220) + std::string("2"), Sunflower.Get_Cost() * 3, 6, Sunflower.Get_Speed() * .5f),
		GloomShroom,
		CatTail,
		WinterPult,
		GoldMagnet,
		SpikeRock,
		CobCannon,
		Imitator
		;

	// Zombies Cells
	// Info: https://plantsvszombies.fandom.com/wiki/Zombies_(PvZ)
	const CellContent
		Basic = CellContent(COLOR(165) + std::string("Z"), 5, 10, 4.7f),
		FlagBasic = CellContent(COLOR(1) + std::string("F"), 0, Basic.Get_HP() * 1.25f, Basic.Get_Speed() * 1.25f),
		ConeHead = CellContent(COLOR(208) + std::string("C"), Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed()),
		PoleVault = CellContent(COLOR(74) + std::string("P"), Basic.Get_Cost() * 4, Basic.Get_HP() * 2, Basic.Get_Speed() / 2),
		BucketHead = CellContent("\033[97m" + std::string("B"), Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed()),
		NewsPaper,
		ScreenDoor,
		FootBall,
		Dancing,
		BackUp,
		DuckyTube,
		Snorkel,
		Zomboni,
		Bobsled,
		DolphinRider,
		JackInTheBox,
		Balloon,
		Digger,
		Pogo,
		Yeti,
		Bungee,
		Ladder,
		Catapult,
		Gargantuar,
		Imp,
		DrZomboss
		;

	// Boards Size
	Coords plantsBoardSize{}, zombiesBoardSize{}, gameBoardSize{};
	GameBoard plantsBoard, zombiesBoard, gameBoard;
	Coords plantsBoardSelection{}, zombiesBoardSelection{}, gameBoardSelection{};

	std::function<void()> ReStart = [&]() {

		// Console and console font size
		SetConsoleFontSize(res * .1f);
		SetConsoleSize(res, size);

		isLastFlag = false;
		winConditionReached = false;
		flagZombieSpawned = false;
		frameCount = 1;

		plantsCurrency = CellContent(COLOR(220) + std::string("C"), 2, 0, 10 / 2);
		zombiesCurrency = CellContent(plantsCurrency);
		zombieTimeCount = 1;
		gameBoardSize.y = 1;

		level.ClearTypes();

		switch (level.GetLevel().x) {
		case 0:
			level.SetPlantsTypes({ Peashooter });
			level.SetZombiesTypes({ Basic });

			gameBoardSize.y = 1;
			plantsBoardSize.x = 6;
			break;
		case 1:
			level.SetPlantsTypes({ Peashooter, Sunflower });
			level.SetZombiesTypes({ Basic, FlagBasic });

			gameBoardSize.y = 3;
			plantsBoardSize.x = 6;
			break;
		case 2:
			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb });
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead });

			gameBoardSize.y = 3;
			plantsBoardSize.x = 6;
			break;
		case 3:
			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead });

			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;
			break;
		case 4:
			level.SetPlantsTypes({ RollingWallNut });
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead });

			zombieTimeCount = 5;
			zombiesCurrency.Set_Cost(4);
			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;
			break;
		case 5:
			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead, PoleVault });

			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;
			break;
		case 6:
			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead, PoleVault });

			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;
			break;
		case 7:
			level.SetPlantsTypes(ChooseSeeds({ Peashooter, Sunflower, CherryBomb, WallNut }, plantsBoard, plantsBoardSelection));
			level.SetZombiesTypes({ Basic, FlagBasic, ConeHead, PoleVault, BucketHead });

			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;
			break;
		}

		plantsBoardSize.x += 2; // Goes up to 10 + 2
		zombiesBoardSize.x = 12 + 2;

		// Create the boards
		plantsBoardSize.y = 2, zombiesBoardSize.y = 2, gameBoardSize.x = 9;
		plantsBoard = GameBoard(plantsBoardSize);
		plantsBoardSelection = { 2 , 0 };
		zombiesBoard = GameBoard(zombiesBoardSize);
		zombiesBoardSelection = { 2 , 0 };

		gameBoard = GameBoard(gameBoardSize);
		gameBoardSelection = { 0 , 0 };

		// Populate the plantsBoard
		for (int y = 0; y < plantsBoardSize.y; y++) {
			for (int x = 0; x < plantsBoardSize.x; x++) {
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

		float averageCost = 0;
		// Populate the zombiesBoard
		for (int y = 0; y < zombiesBoardSize.y; y++) {
			for (int x = 0; x < zombiesBoardSize.x; x++) {

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
						averageCost += zombiesBoard.GetCell({ x , y }).Get_Cost();
					}
				}
			}
		}

		level.SetWinCondiction(averageCost * 2);

		ClearScreen();

		};

	// Start
	bool gameloop = false;
	Start(gameloop, ReStart);

	// Update
	const int fps = 60;
	std::string output = "";
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
				case 's': case 'S': if (gameBoardSelection.y < gameBoardSize.y - 1) gameBoardSelection.y++; break;
				case 'd': case 'D': if (gameBoardSelection.x < gameBoardSize.x - 1) gameBoardSelection.x++; break;

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

				if (!winConditionReached) zombiesCurrency.Add_Cost(zombieTimeCount);
			}

			for (int y = 0; y < gameBoardSize.y; y++) {
				for (int x = 0; x < gameBoardSize.x; x++) {

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

							for (int i = x + 1; i < gameBoardSize.x; i++) {

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
										if (!winConditionReached) zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() / 2);
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

							for (int j = max(0, y - 1); j <= min(gameBoardSize.y - 1, y + 1); j++) {
								for (int i = max(0, x - 1); i <= min(gameBoardSize.x - 1, x + 1); i++) {

									for (int k = 0; k < level.GetZombiesTypes().size(); k++) {
										if (gameBoard.GetCell({ i, j }).Get_Name() == level.GetZombiesTypes()[k].Get_Name()) {

											CellContent damagedZombie = gameBoard.GetCell({ i, j });
											damagedZombie.Add_HP(-90);
											if (damagedZombie.Get_HP() > 0) {
												gameBoard.SetCell({ i, j }, damagedZombie);
											}
											else {
												gameBoard.SetCell({ i, j }, Nothing);
												if (!winConditionReached) zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() / 2);
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
							ReStart();
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
								if (isLastFlag && !flagZombieSpawned && level.GetZombiesTypes()[i].Get_Name() == FlagBasic.Get_Name()) {
									zombie = level.GetZombiesTypes()[i];
									flagZombieSpawned = true;
									break;
								}
								else if (zombiesCurrency.Get_Cost() >= level.GetZombiesTypes()[i].Get_Cost() && level.GetZombiesTypes()[i].Get_Name() != FlagBasic.Get_Name()) {
									zombie = level.GetZombiesTypes()[i];
									zombieTimeCount++;
									break;
								}
							}

							std::vector<int> possibleLines;
							for (int j = 0; j < gameBoardSize.y; j++) {
								if (gameBoard.GetCell({ gameBoardSize.x - 1, j }).Get_Name() == Nothing.Get_Name()) {
									possibleLines.push_back(j);
								}
							}

							if (!possibleLines.empty()) {
								int randomLine = Random::FromList(possibleLines);
								gameBoard.SetCell({ gameBoardSize.x - 1, randomLine }, zombie);
								zombiesCurrency.Add_Cost(-zombie.Get_Cost());
							}
						}

					}
				}

				for (int x = gameBoardSize.x - 1; x >= 0; x--) {

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
										if (!winConditionReached) zombiesCurrency.Set_Cost(gameBoard.GetCell({ x - 1, y }).Get_Cost() * .5f);
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

			// Last Wave
			if (!winConditionReached && zombiesCurrency.Get_Cost() >= level.GetWinCondiction()) {

				winConditionReached = true;
				isLastFlag = true;

			}

			// Win
			if (winConditionReached && AreAllZombiesGone(gameBoardSize, level, gameBoard)) {
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
