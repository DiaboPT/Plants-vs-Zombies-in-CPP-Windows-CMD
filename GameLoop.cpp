// GameLoop.cpp

#include "GameLoop.hpp"
#include "cppFrameWork/FrameWork.hpp"

#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

const int Res = 256;
const int Width = 4, Height = 3, Lenght = 5;
Coords size{ Width , Height , Lenght };

// Boards Size
static Coords
	plantsBoardSize{},
	plantsBoardSelection{},

	zombiesBoardSize{},
	zombiesBoardSelection{},

	gameBoardSize{},
	gameBoardSelection{}
;

static std::vector<CellContentClass> ChooseSeeds(std::vector<CellContentClass> Seeds) {
	std::vector<CellContentClass> vectorCellContent;

	Coords seedsTableSelection{}, seedsTableSize{ 8 , 6 };
	Coords plantsBoardSelection{};
	GameBoard seedsTable = GameBoard(seedsTableSize);
	GameBoard plantsBoard({ plantsBoardSize.x , 1 });

	int seedIndex = 0;
	for (int y = 0; y < seedsTableSize.y; y++) {
		for (int x = 0; x < seedsTableSize.x; x++) {
			if (seedIndex < Seeds.size()) {
				seedsTable.Cell({ x , y }, Seeds[seedIndex]);
				seedIndex++;
			}
		}
	}

	for (int y = 0; y < plantsBoardSize.y; y++) {
		for (int x = 0; x < plantsBoardSize.x; x++) {
			plantsBoard.Cell({ x , y }, Nothing);
		}
	}

	bool gameloop = true;
	std::string output = "";
	float fps = 60;

	Update(gameloop, output, fps, [&] {

		// Detects if key is pressed
		if (IsKeyPressed()) {
			char key = GetKeyPressed();

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
				CellContentClass selectedSeed = seedsTable.Cell(seedsTableSelection);
				// Check if there's already a plant in the selected position
				(plantsBoard.Cell(plantsBoardSelection).Name() != selectedSeed.Name()) ?
					// Place the plant on the plants board
					plantsBoard.Cell(plantsBoardSelection, selectedSeed) :
					// Remove the plant from the plants board
					plantsBoard.Cell(plantsBoardSelection, Nothing); // Empty cell
			}
			break;
			case 27:
				gameloop = false;
				break;
			}
		}

		output = RESET +
			std::string("World ") +
			std::to_string(level.GetLevel().y + 1) +
			"-" +
			std::to_string(level.GetLevel().x + 1) +
			"\n" + RESET +

			"Objective: Defeat " + FlagZombie.Name() + RESET +
			"\n" + RESET +

			"\n" +
			plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET) +
			"\n" + RESET +
			seedsTable.DrawBoard(seedsTableSelection, COLOR(220), GAMERESET) +
			"\n" + RESET +

			"Selected: " +
			"\n" + RESET +

			"- Name: " +
			std::string(seedsTable.Cell(seedsTableSelection).Name()) +
			RESET + std::string("                                    ") +
			"\n" + RESET +

			"- HP: " +
			((seedsTable.Cell(seedsTableSelection).HP() > 9) ?
				std::to_string(seedsTable.Cell(seedsTableSelection).HP()) + " " :
				"0" + std::to_string(seedsTable.Cell(seedsTableSelection).HP())) +
			RESET + std::string("                                    ") +
			"\n" + RESET
			;
		});

	// Resize vectorCellContent to match the board size
	auto gridSize = plantsBoard.Grid();
	vectorCellContent.resize(static_cast<std::vector<CellContentClass, std::allocator<CellContentClass>>::size_type>(gridSize.x) * gridSize.y); // Resize the vector to fit the board's cells

	int index = 0;
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			vectorCellContent[index++] = plantsBoard.Cell({ x, y }); // Populating the vector
		}
	}

	return vectorCellContent;
}

static bool AreAllZombiesGone(Coords gameBoardSize, Levels level, GameBoard gameBoard) {
	for (int y = 0; y < gameBoardSize.y; y++) {
		for (int x = 0; x < gameBoardSize.x; x++) {
			for (const auto& z : level.GetZombiesTypes()) {
				if (gameBoard.Cell({ x, y }).Char() == z.Char()) {
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
	bool isLastFlag = false;
	bool winConditionReached = false;
	bool flagZombieSpawned = false;
	int frameCount = 0;

	// Importante Cells
	int zombieTimeCount = 0;

	bool gameloop = false;
	std::function<void()> ReStart = [&]() {

		// Console and console font size
		ConsoleFontSize(Res * .1f);
		ConsoleSize(Res, size);

		isLastFlag = false;
		winConditionReached = false;
		flagZombieSpawned = false;
		frameCount = 1;

		plantsCurrency = CellContentClass(
			COLOR(220),
			std::string("Currency"),
			InicialPlantsCurrency,
			0,
			SpeedEnum::AutoProduction
		);
		zombiesCurrency = plantsCurrency;
		zombiesCurrency.Cost(InicialZombiesCurrency);

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

			zombieTimeCount = Zombie.Cost() * .5f;
			plantsCurrency.Cost(0);
			plantsCurrency.Speed(4000);
			break;
		case 5:
			gameloop = false; break;
		case 7:
			gameBoardSize.y = 5;
			plantsBoardSize.x = 6;

			level.SetPlantsTypes(ChooseSeeds({ Peashooter, Sunflower, CherryBomb, WallNut, PotatoMine }));
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
					plantsBoard.Cell({ x , y }, y == 0 ? plantsCurrency : CellContentClass());
				}
				else if (x == 1) {
					plantsBoard.Cell({ x , y }, CellContentClass()); // Empty cell
				}
				else {
					int index = x - 2;
					if (index < level.GetPlantsTypes().size()) {
						plantsBoard.Cell({ x, y }, y == 0 ?
							level.GetPlantsTypes()[index] :
							CellContentClass(std::string(level.GetPlantsTypes()[index].Color()), std::to_string(level.GetPlantsTypes()[index].Cost()), level.GetPlantsTypes()[index].Cost()));
					}
				}
			}
		}

		float averageCost = 0;
		// Populate the zombiesBoard
		for (int y = 0; y < zombiesBoardSize.y; y++) {
			for (int x = 0; x < zombiesBoardSize.x; x++) {

				if (x == 0) {
					zombiesBoard.Cell({ x, y }, y == 0 ? zombiesCurrency : CellContentClass());
				}
				else if (x == 1) {
					zombiesBoard.Cell({ x, y }, CellContentClass()); // Empty cell
				}
				else {
					int index = x - 2;
					if (index < level.GetZombiesTypes().size()) {
						zombiesBoard.Cell({ x, y }, y == 0 ? level.GetZombiesTypes()[index] :
							CellContentClass(std::string(level.GetZombiesTypes()[index].Color()), std::to_string(level.GetZombiesTypes()[index].Cost()), level.GetZombiesTypes()[index].Cost()));
						averageCost += zombiesBoard.Cell({ x , y }).Cost();
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
			if (IsKeyPressed()) {
				char key = GetKeyPressed();
				bool hasMoney = plantsCurrency.Cost() >= plantsBoard.Cell({ plantsBoardSelection.x, 1 }).Cost();

				bool canPlace = true;
				for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
					if (gameBoard.Cell({ gameBoardSelection }).Char() == level.GetZombiesTypes()[i].Char()) {
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

					case '+': plantsCurrency.AddCost(1); break;
					case '1': level.SetLevel({ 0, 0 }); ReStart(); break;
					case '2': level.SetLevel({ 1, 0 }); ReStart(); break;
					case '3': level.SetLevel({ 2, 0 }); ReStart(); break;
					case '4': level.SetLevel({ 3, 0 }); ReStart(); break;
					case '5': level.SetLevel({ 4, 0 }); ReStart(); break;
					// case '6': level.SetLevel({ 5, 0 }); ReStart(); break;
					case '7': level.SetLevel({ 6, 0 }); ReStart(); break;

					case 'r': case 'R': ReStart(); break;

					case ' ':
						if (hasMoney && canPlace) {
							plantsCurrency.AddCost(gameBoard.Cell(gameBoardSelection).Cost() * .5f);
							gameBoard.Cell(gameBoardSelection, plantsBoard.Cell(plantsBoardSelection));
							plantsCurrency.AddCost(-plantsBoard.Cell({ plantsBoardSelection.x, 1 }).Cost());
						}
						break;
					case 27:
						gameloop = false;
						break;
				}
			}

			// Currency Update
			if (fmod(frameCount, fps * plantsCurrency.Speed()) < 1) {
				// PlayCollectSunSound();
				plantsCurrency.AddCost(1);

				if (!winConditionReached) zombiesCurrency.AddCost(zombieTimeCount);
			}

			for (int y = 0; y < gameBoardSize.y; y++) {
				for (int x = 0; x < gameBoardSize.x; x++) {

					// Sunflowers Currency Update
					int sunflowerCount = 0;
					bool hasSunflowerHere = gameBoard.Cell({ x, y }).Name() == Sunflower.Name();
					if (hasSunflowerHere) {
						// PlayCollectSunSound();
						if (fmod(frameCount, fps * Sunflower.Speed()) < 1) {
							sunflowerCount++;
						}
						plantsCurrency.AddCost(sunflowerCount);  // Increase cost based on Sunflowers
					}

					// Peashooter Shoots Update
					bool hasPeashooterHere = gameBoard.Cell({ x, y }).Name() == Peashooter.Name();
					if (hasPeashooterHere) {

						if (fmod(frameCount, fps * Peashooter.Speed()) < 1) {

							for (int i = x + 1; i < gameBoardSize.x; i++) {

								bool hasZombieNext = false;
								for (int j = 0; j < level.GetZombiesTypes().size(); j++) {
									if (gameBoard.Cell({ i, y }).Name() == level.GetZombiesTypes()[j].Name()) {
										hasZombieNext = true;
										break;
									}
								}
								if (hasZombieNext) {

									// Damages next cell
									CellContentClass damagedZombie = gameBoard.Cell({ i, y });
									damagedZombie.AddHP(-1);

									if (damagedZombie.HP() <= Zombie.HP()) {
										damagedZombie.Color(Zombie.Color());
										damagedZombie.Name(Zombie.Name());
									}
									else if(damagedZombie.HP() <= 0) {
										damagedZombie = Nothing;
										if (!winConditionReached) zombiesCurrency.AddCost(damagedZombie.Cost() / 2);
									}
									// PlayZombieHitSound();
									gameBoard.Cell({ i, y }, damagedZombie);
									break;
								}
							}
						}
					}

					// Cherry Bomb Update
					bool hasCherryBombHere = gameBoard.Cell({ x, y }).Name() == CherryBomb.Name();
					if (hasCherryBombHere) {

						if (fmod(frameCount, fps * CherryBomb.Speed()) < 1) {
							// PlayCherryBombExplodeSound();

							for (int j = std::max(0, y - 1); j <= std::min(gameBoardSize.y - 1, y + 1); j++) {
								for (int i = std::max(0, x - 1); i <= std::min(gameBoardSize.x - 1, x + 1); i++) {

									for (int k = 0; k < level.GetZombiesTypes().size(); k++) {
										if (gameBoard.Cell({ i, j }).Name() == level.GetZombiesTypes()[k].Name()) {

											CellContentClass damagedZombie = gameBoard.Cell({ i, j });
											damagedZombie.AddHP(-90);
											if (damagedZombie.HP() > 0) {
												gameBoard.Cell({ i, j }, damagedZombie);
											}
											else {
												gameBoard.Cell({ i, j }, Nothing);
												if (!winConditionReached) zombiesCurrency.AddCost(damagedZombie.Cost() / 2);
											}
											// PlayZombieHitSound();
										}
									}
								}
							}
							gameBoard.Cell({ x, y }, Nothing);
						}
					}

					// Zombies movement + damages next Plant
					bool hasZombie = false;
					for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
						if (gameBoard.Cell({ x, y }).Name() == level.GetZombiesTypes()[i].Name()) {
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
								if (gameBoard.Cell({ x - 1, y }).Name() == level.GetPlantsTypes()[i].Name()) {
									hasPlantNext = true;
									break;
								}
							}

							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.Cell({ x - 1, y }).Name() == level.GetZombiesTypes()[i].Name()) {
									hasZombieNext = true;
									break;
								}
							}

							if (hasPlantNext) {

								// Damages next cell
								if (frameCount % fps == 0) {
									CellContentClass damagedPlant = gameBoard.Cell({ x - 1, y });
									damagedPlant.AddHP(-1);
									if (damagedPlant.HP() != 0) {
										gameBoard.Cell({ x - 1, y }, damagedPlant);
									}
									else {
										gameBoard.Cell({ x - 1, y }, Nothing);
									}
									// PlayZombieBiteSound();
								}
							}
							else if (!hasPlantNext && !hasZombieNext) {
								if (fmod(frameCount, fps * gameBoard.Cell({ x,y }).Speed()) < 1) {

									// Moves a cell foward
									gameBoard.Cell({ x - 1, y }, gameBoard.Cell({ x,y }));
									gameBoard.Cell({ x, y }, Nothing);
								}
							}
						}
					}

					// Buy zombies
					bool zombieHasMoney = zombiesCurrency.Cost() >= Zombie.Cost();
					if (zombieHasMoney) {

						if ((frameCount % fps) == 0) {
							CellContentClass zombie;

							for (int i = (int)level.GetZombiesTypes().size() - 1; i >= 0; i--) {
								if (isLastFlag && !flagZombieSpawned && level.GetZombiesTypes()[i].Name() == FlagZombie.Name()) {
									zombie = level.GetZombiesTypes()[i];
									flagZombieSpawned = true;
									break;
								}
								else if (zombiesCurrency.Cost() >= level.GetZombiesTypes()[i].Cost() && level.GetZombiesTypes()[i].Name() != FlagZombie.Name()) {
									zombie = level.GetZombiesTypes()[i];
									zombieTimeCount++;
									break;
								}
							}

							std::vector<int> possibleLines;
							for (int j = 0; j < gameBoardSize.y; j++) {
								if (gameBoard.Cell({ gameBoardSize.x - 1, j }).Name() == Nothing.Name()) {
									possibleLines.push_back(j);
								}
							}

							if (!possibleLines.empty()) {
								int randomLine = Random::FromList(possibleLines);
								gameBoard.Cell({ gameBoardSize.x - 1, randomLine }, zombie);
								zombiesCurrency.AddCost(-zombie.Cost());
							}
						}

					}
				}

				for (int x = gameBoardSize.x - 1; x >= 0; x--) {

					// Rolling Nut
					bool hasRollingWallNut = gameBoard.Cell({ x, y }).Char() == RollingWallNut.Char();
					if (hasRollingWallNut) {

						if (x == size.x - 1) {
							gameBoard.Cell({ x, y }, Nothing);
						}
						else {
							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.Cell({ x - 1, y }).Char() == level.GetZombiesTypes()[i].Char()) {
									hasZombieNext = true;
									break;
								}
							}

							if (fmod(frameCount, fps * gameBoard.Cell({ x,y }).Speed()) < 1) {
								if (hasZombieNext) {

									CellContentClass damagedZombie = gameBoard.Cell({ x - 1, y });
									damagedZombie.AddHP(Zombie.HP());
									if (damagedZombie.HP() != 0) {
										gameBoard.Cell({ x - 1, y }, damagedZombie);
									}
									else {
										if (!winConditionReached) zombiesCurrency.Cost(gameBoard.Cell({ x - 1, y }).Cost() * .5f);
										gameBoard.Cell({ x - 1, y }, Nothing);
									}

									gameBoard.Cell({ x + 1 , y + 1 }, gameBoard.Cell({ x, y }));
									gameBoard.Cell({ x, y }, Nothing);

									// PlayZombieHitSound();
								}
								else {
									// Moves a cell foward
									gameBoard.Cell({ x + 1 , y }, gameBoard.Cell({ x , y }));
									gameBoard.Cell({ x, y }, Nothing);
								}
							}
						}
					}
				}
			}

			plantsBoard.Cell({ 0, 1 }, CellContentClass(std::string(plantsCurrency.Color()), std::to_string(plantsCurrency.Cost()), plantsCurrency.Cost()));

			output = RESET + 
				std::string("World ") +
				std::to_string(level.GetLevel().y + 1) +
				"-" +
				std::to_string(level.GetLevel().x + 1) +
				"\n" + RESET +
				
				"Objective: Defeat " + FlagZombie.Name() + RESET +
				"\n" + RESET +
				
				"\n" +
				plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET) +
				"\n" + RESET +
				gameBoard.DrawBoard(gameBoardSelection, COLOR(220), GAMERESET) +
				"\n" + RESET +
				
				"Selected: " +
				"\n" + RESET +
					
				"- Name: " +
				std::string(gameBoard.Cell(gameBoardSelection).Color()) + std::string(gameBoard.Cell(gameBoardSelection).Name()) +
				RESET + std::string("                                    ") +
				"\n" + RESET +
				
				"- HP: " +
				std::string(gameBoard.Cell(gameBoardSelection).HP() > 9 ?
				std::to_string(gameBoard.Cell(gameBoardSelection).HP()) + " " :
				"0" + std::to_string(gameBoard.Cell(gameBoardSelection).HP())) +
				RESET + std::string("                                    ") +
				"\n" + RESET
				
				//+
				//"\n" +
				//"Zombies Board:\n" + std::string(PLANTRESET) + zombiesBoard.DrawBoard(zombiesBoardSelection, COLOR(165), PLANTRESET)+ 
				//"\n" + RESET
				;

			// Last Wave
			if (!winConditionReached && zombiesCurrency.Cost() >= level.GetWinCondiction()) {

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

	level.SetLevel({ level.GetLevel().x - 1 , level.GetLevel().y });
	std::cout << RESET;
	std::cout << "\n";
	std::cout << "+---------------------+\n";
	std::cout << "| Thanks for playing! |\n";
	std::cout << "| Level reached: " + std::to_string(level.GetLevel().y + 1) + "-" + std::to_string(level.GetLevel().x + 1) << "  |\n";
	std::cout << "+---------------------+\n";
	std::cout << RESET;
}
