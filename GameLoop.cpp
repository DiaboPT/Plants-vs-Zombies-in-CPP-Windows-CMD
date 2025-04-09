// GameLoop.cpp

#include "GameLoop.hpp"
#include "cppFrameWork/FrameWork.hpp"

#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

const int Res = 108;
const int Width = 16, Height = 9, Lenght = 5;
Coords size{ Width , Height , Lenght };

static std::vector<CellContent> ChooseSeeds(std::vector<CellContent> Seeds, Coords plantsBoardSize, Coords plantsBoardSelection) {
	std::vector<CellContent> vectorCellContent;

	Coords seedsTableSelection{}, seedsTableSize{ 8 , 6 };
	GameBoard seedsTable = GameBoard(seedsTableSize);
	GameBoard plantsBoard({ plantsBoardSize.x , 1 });

	int seedIndex = 0;
	for (int y = 0; y < seedsTableSize.y; y++) {
		for (int x = 0; x < seedsTableSize.x; x++) {
			if (seedIndex < Seeds.size()) {
				seedsTable.SetCell({ x , y }, Seeds[seedIndex]);
				seedIndex++;
			}
		}
	}

	bool gameloop = true;
	std::string output = "";
	float fps = 60;

	Update(gameloop, output, fps, [&] {

		// Detects if key is pressed
		if (isKeyPressed()) {
			char key = getKeyPressed();

			switch (key) {
			case 'q': case 'Q': if (plantsBoardSelection.x > 0) plantsBoardSelection.x--; break;
			case 'e': case 'E': if (plantsBoardSelection.x < (plantsBoardSize.x) - 1) plantsBoardSelection.x++; break;

			case 'w': case 'W': if (seedsTableSelection.y > 0) seedsTableSelection.y--; break;
			case 'a': case 'A': if (seedsTableSelection.x > 0) seedsTableSelection.x--; break;
			case 's': case 'S': if (seedsTableSelection.y < seedsTableSize.y - 1) seedsTableSelection.y++; break;
			case 'd': case 'D': if (seedsTableSelection.x < seedsTableSize.x - 1) seedsTableSelection.x++; break;

			case ' ':
			{
				// Get the seed at the current position
				CellContent selectedSeed = seedsTable.GetCell(seedsTableSelection);
				// Check if there's already a plant in the selected position
				if (plantsBoard.GetCell(plantsBoardSelection).Get_Name() != "") {
					// Place the plant on the plants board
					plantsBoard.SetCell(plantsBoardSelection, selectedSeed);
					// Mark the seed as used (turn grey in the seeds table)
					seedsTable.SetCell(seedsTableSelection, CellContent(COLOR(20), selectedSeed.Get_Name(), selectedSeed.GetCost(), selectedSeed.GetHP(), selectedSeed.GetSpeed()));
				}
				else {
					// Remove the plant from the plants board
					plantsBoard.SetCell(plantsBoardSelection, CellContent()); // Empty cell
					// Restore the seed to its original color
					seedsTable.SetCell(seedsTableSelection, selectedSeed);
				}
			}
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

		output += "\n- Name: ";
		output += std::string(seedsTable.GetCell(seedsTableSelection).GetColor()) + std::string(seedsTable.GetCell(seedsTableSelection).Get_Name());
		output += RESET + std::string("                                    ");
		output += "\n";

		output += "- HP: ";
		output +=
			seedsTable.GetCell(seedsTableSelection).GetHP() > 9 ?
			std::to_string(seedsTable.GetCell(seedsTableSelection).GetHP()) + " " :
			"0" + std::to_string(seedsTable.GetCell(seedsTableSelection).GetHP());
		output += RESET + std::string("                                    ");
		output += "\n";
		});

	// Resize vectorCellContent to match the board size
	auto gridSize = plantsBoard.GetGrid();
	vectorCellContent.resize(gridSize.x * gridSize.y); // Resize the vector to fit the board's cells

	int index = 0;
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			vectorCellContent[index++] = plantsBoard.GetCell({ x, y }); // Populating the vector
		}
	}

	return vectorCellContent;
}

static bool AreAllZombiesGone(Coords gameBoardSize, Levels level, GameBoard gameBoard) {
	for (int y = 0; y < gameBoardSize.y; y++) {
		for (int x = 0; x < gameBoardSize.x; x++) {
			for (const auto& z : level.GetZombiesTypes()) {
				if (gameBoard.GetCell({ x, y }).Get_Char() == z.Get_Char()) {
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
	CellContent

		// Day Levels
		Peashooter = CellContent(COLOR(46), std::string("Peashooter"), 4, 6, 1.5f),
		Sunflower = CellContent(COLOR(220), std::string("Sunflower"), 2, 6, 24.0f / 2),
		CherryBomb = CellContent(COLOR(1), std::string("Cherry Bomb"), 6, 6, 1.2f),
		WallNut = CellContent(COLOR(130), std::string("Wall-Nut"), 2, 72),
		RollingWallNut = CellContent(COLOR(208), std::string("Rolling Wall-Nut"), 0, WallNut.GetHP(), Peashooter.GetSpeed()),
		PotatoMine = CellContent(COLOR(130), std::string("Potato Mine"), 1),
		SnowPea = CellContent(COLOR(74), std::string("Snow Peashooter"), Peashooter.GetCost() * 1.75f, Peashooter.GetHP(), Peashooter.GetSpeed()),
		Chomper = CellContent(COLOR(93), std::string("Chomper"), 6),
		Repeater = CellContent(COLOR(46), std::string("Repeater"), Peashooter.GetCost() * 2, 6, Peashooter.GetSpeed() * .5f),

		// Night Levels
		PuffShroom = CellContent(COLOR(93), std::string("Puff-Shroom"), 0),
		SunShroom = CellContent(COLOR(178), std::string("Sun-Shroom"), 1),
		FumeShroom = CellContent(PuffShroom.GetColor(), std::string("Fume-Shroom"), 3),
		GraveBuster = CellContent(COLOR(235), std::string("Grave Buster"), 3),
		HypnoShroom = CellContent(COLOR(218), std::string("Hypno-Shroom"), 3),
		ScaredyShroom = CellContent(COLOR(95), std::string("Scaredy-Shroom"), 1),
		IceShroom = CellContent(SnowPea.GetColor(), std::string("Ice-Shroom"), 3),
		DoomShroom = CellContent(GraveBuster.GetColor(), std::string("Doom-Shroom"), 5),

		// Pool Levels
		LilyPad = CellContent(COLOR(37), std::string("Lily Pad"), 1),
		Squash = CellContent(COLOR(120), std::string("Squash"), 2),
		Threepeater = CellContent(Peashooter.GetColor(), std::string("Threepeater"), Peashooter.GetCost() * 3.25f, Peashooter.GetHP(), Peashooter.GetSpeed()),
		TangleKelp = CellContent(COLOR(100), std::string("Tangle Kelp"), 1),
		Jalapeno = CellContent(CherryBomb.GetColor(), std::string("Jalapeno"), 5),
		SpikeWeed = CellContent(WallNut.GetColor(), std::string("SpikeWeed"), 4),
		TorchWood = CellContent(COLOR(202), std::string("TorchWood"), 7),
		TallNut = CellContent(WallNut.GetColor(), std::string("TallNut"), WallNut.GetCost() * 2.25f, WallNut.GetHP() * 2.25f),

		// Fog Levels
		SeaShroom = CellContent(COLOR(37), std::string("Sea-Shroom"), 0),
		Plantern = CellContent(COLOR(226), std::string("Plantern"), 1),
		Cactus = CellContent(COLOR(82), std::string("Cactus"), 5, Peashooter.GetHP(), Peashooter.GetSpeed()),
		Blover = CellContent(COLOR(22), std::string("Blover"), 4),
		SplitPea = CellContent(Peashooter.GetColor(), std::string("Split Peashooter"), Peashooter.GetCost() * 1.25f, Peashooter.GetHP(), Peashooter.GetSpeed()),
		StarFruit = CellContent(COLOR(184), std::string("Starfruit"), Peashooter.GetCost() * 1.25f, Peashooter.GetHP(), Peashooter.GetSpeed()),
		Pumpkin = CellContent(COLOR(172), std::string("Pumpkin"), WallNut.GetCost() * 2.25f, WallNut.GetHP()),
		MagnetShroom = CellContent(COLOR(127), std::string("Magnet-Shroom"), 4),

		// Roof Levels
		CabbagePult = CellContent(COLOR(34), std::string("Cabbage-Pult"), Peashooter.GetCost(), Peashooter.GetHP(), Peashooter.GetSpeed()),
		FlowerPot = CellContent(LilyPad.GetColor(), std::string("Flower Pot"), LilyPad.GetCost(), Peashooter.GetHP()),
		KernelPult = CellContent(Sunflower.GetColor(), std::string("Kernel-Pult"), Peashooter.GetCost(), Peashooter.GetHP(), Peashooter.GetSpeed()),
		CoffeBean = CellContent(WallNut.GetColor(), std::string("Coffe Bean"), 3),
		Garlic = CellContent(COLOR(255), std::string("Garlic"), 2),
		UmbrellaLeaf = CellContent(Peashooter.GetColor(), std::string("Umbrella Leaf"), 4),
		Marigold = CellContent(COLOR(220), std::string("Marigold"), Sunflower.GetCost(), Sunflower.GetHP(), Sunflower.GetSpeed()),
		MelonPult = CellContent(CabbagePult.GetColor(), std::string("Melon-Pult"), CabbagePult.GetCost() * 4, CabbagePult.GetHP(), CabbagePult.GetSpeed())

		//// Extra Plants
		//GatlingPea = CellContent(COLOR(46), std::string("GatlingPea"), Repeater.GetCost() * 2, 6, Repeater.GetSpeed() * .5f),
		//TwinSunFlower = CellContent(COLOR(220), std::string("TwinSunFlower"), Sunflower.GetCost() * 3, 6, Sunflower.GetSpeed() * .5f),
		//GloomShroom,
		//CatTail,
		//WinterPult,
		//GoldMagnet,
		//SpikeRock,
		//CobCannon,

		//// IMITATOR
		//Imitator
		;

	// Zombies Cells
	// Info: https://plantsvszombies.fandom.com/wiki/Zombies_(PvZ)
	const CellContent
		Zombie = CellContent(COLOR(165), std::string("Zombie"), 5, 10, 4.7f),
		FlagZombie = CellContent(COLOR(1), std::string("Flag Zombie"), 0, Zombie.GetHP() * 1.25f, Zombie.GetSpeed() * 1.25f),
		ConeHead = CellContent(COLOR(208), std::string("Cone Head"), Zombie.GetCost() * 2, Zombie.GetHP() * 2, Zombie.GetSpeed()),
		PoleVault = CellContent(COLOR(74), std::string("Pole Vault"), Zombie.GetCost() * 4, Zombie.GetHP() * 2, Zombie.GetSpeed() / 2),
		BucketHead = CellContent(COLOR(255), std::string("Bucket Head"), Zombie.GetCost() * 3, Zombie.GetHP() * 3, Zombie.GetSpeed()),
		NewsPaper,
		ScreenDoor,
		FootBall,
		DancingZombie,
		BackUpZombie,
		DuckyTube,
		SnorkelZombie,
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

	bool gameloop = false;
	std::function<void()> ReStart = [&](){

		// Console and console font size
		SetConsoleFontSize(Res * .25f);
		SetConsoleSize(Res, size);

		isLastFlag = false;
		winConditionReached = false;
		flagZombieSpawned = false;
		frameCount = 1;

		plantsCurrency = CellContent(COLOR(220), std::string("C"), 2, 0, 10 / 2);
		zombiesCurrency = CellContent(plantsCurrency);
		zombieTimeCount = 1;
		gameBoardSize.y = 1;

		level.ClearTypes();

		plantsBoardSize.y = 2, zombiesBoardSize.y = 2, gameBoardSize.x = 9;

		switch (level.GetLevel().x) {
		case 0:
			gameBoardSize.y = 1;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes({ Peashooter });
			level.SetZombiesTypes({ Zombie, FlagZombie });
			break;
		case 1:
			gameBoardSize.y = 3;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes({ Peashooter, Sunflower });
			level.SetZombiesTypes({ Zombie, FlagZombie });
			break;
		case 2:
			gameBoardSize.y = 3;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb });
			level.SetZombiesTypes({ Zombie, FlagZombie, ConeHead });
			break;
		case 3:
			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes({ Peashooter, Sunflower, CherryBomb, WallNut });
			level.SetZombiesTypes({ Zombie, FlagZombie, ConeHead });
			break;
		case 4:
			gameBoardSize.y = 5;
			plantsBoardSize.x = 1;

			level.SetPlantsTypes({ RollingWallNut });
			level.SetZombiesTypes({ Zombie, FlagZombie, ConeHead });

			zombieTimeCount = 5;
			zombiesCurrency.SetCost(4);
			break;
		case 5:
			gameloop = false; break;
		case 7:
			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes(ChooseSeeds({ Peashooter, Sunflower, CherryBomb, WallNut }, plantsBoardSize, plantsBoardSelection));
			level.SetZombiesTypes({ Zombie, FlagZombie, ConeHead, PoleVault, BucketHead });
			break;
		}

		plantsBoardSize.x += 2; // Goes up to 10 + 2
		zombiesBoardSize.x = 12 + 2;

		// Create the boards
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
							CellContent(std::string(level.GetPlantsTypes()[index].GetColor()), std::to_string(level.GetPlantsTypes()[index].GetCost()), level.GetPlantsTypes()[index].GetCost(), 0));
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
							CellContent(std::string(level.GetZombiesTypes()[index].GetColor()), std::to_string(level.GetZombiesTypes()[index].GetCost()), level.GetZombiesTypes()[index].GetCost(), 0));
						averageCost += zombiesBoard.GetCell({ x , y }).GetCost();
					}
				}
			}
		}

		level.SetWinCondiction(averageCost * 2);

		ClearScreen();

		};

	// Start
	Start(gameloop, ReStart);

	// Update
	const int fps = 60;
	std::string output = "";
	Update(gameloop, output, fps,
		[&] {

			// Detects if key is press
			if (isKeyPressed()) {
				char key = getKeyPressed();
				bool hasMoney = plantsCurrency.GetCost() >= plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).GetCost();

				bool canPlace = true;
				for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
					if (gameBoard.GetCell({ gameBoardSelection }).Get_Char() == level.GetZombiesTypes()[i].Get_Char()) {
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
						plantsCurrency.AddCost(gameBoard.GetCell(gameBoardSelection).GetCost() * .5f);
						gameBoard.SetCell(gameBoardSelection, plantsBoard.GetCell(plantsBoardSelection));
						plantsCurrency.AddCost(-plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).GetCost());
					}
					break;
				case 27:
					gameloop = false;
					break;
				}
			}

			// Currency Update
			if (fmod(frameCount, fps * plantsCurrency.GetSpeed()) < 1) {
				// PlayCollectSunSound();
				plantsCurrency.AddCost(1);

				if (!winConditionReached) zombiesCurrency.AddCost(zombieTimeCount);
			}

			for (int y = 0; y < gameBoardSize.y; y++) {
				for (int x = 0; x < gameBoardSize.x; x++) {

					// Sunflowers Currency Update
					int sunflowerCount = 0;
					bool hasSunflowerHere = gameBoard.GetCell({ x, y }).Get_Name() == Sunflower.Get_Name();
					if (hasSunflowerHere) {
						// PlayCollectSunSound();
						if (fmod(frameCount, fps * Sunflower.GetSpeed()) < 1) {
							sunflowerCount++;
						}
						plantsCurrency.AddCost(sunflowerCount);  // Increase cost based on Sunflowers
					}

					// Peashooter Shoots Update
					bool hasPeashooterHere = gameBoard.GetCell({ x, y }).Get_Name() == Peashooter.Get_Name();
					if (hasPeashooterHere) {

						if (fmod(frameCount, fps * Peashooter.GetSpeed()) < 1) {

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
									damagedZombie.AddHP(-1);
									if (damagedZombie.GetHP() > 0) {
										gameBoard.SetCell({ i, y }, damagedZombie);
									}
									else {
										gameBoard.SetCell({ i, y }, Nothing);
										if (!winConditionReached) zombiesCurrency.AddCost(damagedZombie.GetCost() / 2);
									}
									// PlayZombieHitSound();
									break;
								}
							}
						}
					}

					// Cherry Bomb Update
					bool hasCherryBombHere = gameBoard.GetCell({ x, y }).Get_Name() == CherryBomb.Get_Name();
					if (hasCherryBombHere) {

						if (fmod(frameCount, fps * CherryBomb.GetSpeed()) < 1) {
							// PlayCherryBombExplodeSound();

							for (int j = std::max(0, y - 1); j <= std::min(gameBoardSize.y - 1, y + 1); j++) {
								for (int i = std::max(0, x - 1); i <= std::min(gameBoardSize.x - 1, x + 1); i++) {

									for (int k = 0; k < level.GetZombiesTypes().size(); k++) {
										if (gameBoard.GetCell({ i, j }).Get_Name() == level.GetZombiesTypes()[k].Get_Name()) {

											CellContent damagedZombie = gameBoard.GetCell({ i, j });
											damagedZombie.AddHP(-90);
											if (damagedZombie.GetHP() > 0) {
												gameBoard.SetCell({ i, j }, damagedZombie);
											}
											else {
												gameBoard.SetCell({ i, j }, Nothing);
												if (!winConditionReached) zombiesCurrency.AddCost(damagedZombie.GetCost() / 2);
											}
											// PlayZombieHitSound();
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
									damagedPlant.AddHP(-1);
									if (damagedPlant.GetHP() != 0) {
										gameBoard.SetCell({ x - 1, y }, damagedPlant);
									}
									else {
										gameBoard.SetCell({ x - 1, y }, Nothing);
									}
									// PlayZombieBiteSound();
								}
							}
							else if (!hasPlantNext && !hasZombieNext) {
								if (fmod(frameCount, fps * gameBoard.GetCell({ x,y }).GetSpeed()) < 1) {

									// Moves a cell foward
									gameBoard.SetCell({ x - 1, y }, gameBoard.GetCell({ x,y }));
									gameBoard.SetCell({ x, y }, Nothing);
								}
							}
						}
					}

					// Buy zombies
					bool zombieHasMoney = zombiesCurrency.GetCost() >= Zombie.GetCost();
					if (zombieHasMoney) {

						if ((frameCount % fps) == 0) {
							CellContent zombie;

							for (int i = (int)level.GetZombiesTypes().size() - 1; i >= 0; i--) {
								if (isLastFlag && !flagZombieSpawned && level.GetZombiesTypes()[i].Get_Name() == FlagZombie.Get_Name()) {
									zombie = level.GetZombiesTypes()[i];
									flagZombieSpawned = true;
									break;
								}
								else if (zombiesCurrency.GetCost() >= level.GetZombiesTypes()[i].GetCost() && level.GetZombiesTypes()[i].Get_Name() != FlagZombie.Get_Name()) {
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
								zombiesCurrency.AddCost(-zombie.GetCost());
							}
						}

					}
				}

				for (int x = gameBoardSize.x - 1; x >= 0; x--) {

					// Rolling Nut
					bool hasRollingWallNut = gameBoard.GetCell({ x, y }).Get_Char() == RollingWallNut.Get_Char();
					if (hasRollingWallNut) {

						if (x == size.x - 1) {
							gameBoard.SetCell({ x, y }, Nothing);
						}
						else {
							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1, y }).Get_Char() == level.GetZombiesTypes()[i].Get_Char()) {
									hasZombieNext = true;
									break;
								}
							}

							if (fmod(frameCount, fps * gameBoard.GetCell({ x,y }).GetSpeed()) < 1) {
								if (hasZombieNext) {

									CellContent damagedZombie = gameBoard.GetCell({ x - 1, y });
									damagedZombie.AddHP(Zombie.GetHP());
									if (damagedZombie.GetHP() != 0) {
										gameBoard.SetCell({ x - 1, y }, damagedZombie);
									}
									else {
										if (!winConditionReached) zombiesCurrency.SetCost(gameBoard.GetCell({ x - 1, y }).GetCost() * .5f);
										gameBoard.SetCell({ x - 1, y }, Nothing);
									}

									gameBoard.SetCell({ x + 1 , y + 1 }, gameBoard.GetCell({ x, y }));
									gameBoard.SetCell({ x, y }, Nothing);

									// PlayZombieHitSound();
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

			plantsBoard.SetCell({ 0, 1 }, CellContent(std::string(plantsCurrency.GetColor()), std::to_string(plantsCurrency.GetCost()), plantsCurrency.GetCost(), 0));
			zombiesBoard.SetCell({ 0, 1 }, CellContent(std::string(zombiesCurrency.GetColor()), std::to_string(zombiesCurrency.GetCost()), zombiesCurrency.GetCost(), 0));

			output = RESET;
			output += "World " + std::to_string(level.GetLevel().y + 1) + "-" + std::to_string(level.GetLevel().x + 1);
			output += "\n";
			output += RESET;

			output += "Objective: Defeat " + FlagZombie.Get_Name() + RESET;
			output += "\n";
			output += RESET;

			output += "\n";
			output += "Plants Board:\n" + plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET);
			output += "\n";
			output += RESET;

			output += "Game Board:\n" + gameBoard.DrawBoard(gameBoardSelection, COLOR(220), GAMERESET);
			output += RESET;

			output += "Selected: ";
			output += "\n";

			output += "- Name: ";
			output += std::string(gameBoard.GetCell(gameBoardSelection).GetColor()) + std::string(gameBoard.GetCell(gameBoardSelection).Get_Name());
			output += RESET + std::string("                                    ");
			output += "\n";

			output += "- HP: ";
			output +=
				gameBoard.GetCell(gameBoardSelection).GetHP() > 9 ?
				std::to_string(gameBoard.GetCell(gameBoardSelection).GetHP()) + " " :
				"0" + std::to_string(gameBoard.GetCell(gameBoardSelection).GetHP());
			output += RESET + std::string("                                    ");
			output += "\n";

			output += "\n";
			output += "Zombies Board:\n" + std::string(PLANTRESET) + zombiesBoard.DrawBoard(zombiesBoardSelection, COLOR(165), PLANTRESET);
			output += RESET;

			// Last Wave
			if (!winConditionReached && zombiesCurrency.GetCost() >= level.GetWinCondiction()) {

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
