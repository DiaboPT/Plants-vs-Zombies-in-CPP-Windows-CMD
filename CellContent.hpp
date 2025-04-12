#pragma once

#include <iostream>
#include <vector>

#include "cppFrameWork/FrameWork.hpp"

enum DamageEnum {
	Pea = 1,
	FirePea = Pea * 2,
	InstaKill = 90,
};

enum HPEnum {
	Standard = 6,
	ZombieHP = 10,
	Tanky = Standard * 3,
	Tankier = Tanky * 4,
};

enum SpeedEnum {
	Instant = 100,
	Normal = 1500,
	ZombieSpeed = 4700,
	AutoProduction = 5000,
	SunProduction = 12000,
};

// CellContentClass class
class CellContentClass {
private:
	std::string name = " ";
	std::string color = "";

	int cost = 0;
	int hp = 0;
	float speed = 0;

public:
	CellContentClass() {}

	CellContentClass(std::string new_color, std::string new_name, int new_cost) :
		color(new_color),
		name(new_name),
		cost(new_cost)
	{
	}

	CellContentClass(std::string new_color, std::string new_name, int new_cost, int new_hp, float new_speed) :
		color(new_color),
		name(new_name),
		cost(new_cost),
		hp(new_hp),
		speed(new_speed / 1000.f)
	{
	}

	void Name(std::string value);
	const std::string Name() const;

	void Color(std::string value);
	const std::string Color() const;

	void Cost(int value);
	const int Cost() const;

	void HP(int value);
	const int HP() const;

	void Speed(float value);
	const float Speed() const;

	const char Char() const;
	void AddCost(int value);
	void AddHP(int value);

};

static CellContentClass Nothing, plantsCurrency, zombiesCurrency;
static int InicialPlantsCurrency = 2, InicialZombiesCurrency = 8;

// Plants Cells
// Info : https://plantsvszombies.fandom.com/wiki/Plants_(PvZ)
const CellContentClass

	// Day Levels
	Peashooter = CellContentClass(
		COLOR(46),
		std::string("Peashooter"),
		4,
		HPEnum::Standard,
		SpeedEnum::Normal)
	,
	Sunflower = CellContentClass(
		COLOR(220),
		std::string("Sunflower"),
		2,
		HPEnum::Standard,
		SpeedEnum::SunProduction)
	,
	CherryBomb = CellContentClass(
		COLOR(1),
		std::string("Cherry Bomb"),
		6,
		HPEnum::Standard,
		SpeedEnum::Instant)
	,
	WallNut = CellContentClass(
		COLOR(130),
		std::string("Wall-Nut"),
		2,
		HPEnum::Tankier,
		SpeedEnum::Normal)
	,
	RollingWallNut = CellContentClass(
		COLOR(208),
		std::string("Rolling Wall-Nut"),
		1,
		HPEnum::Tankier,
		SpeedEnum::Normal / 2)
	,
	PotatoMine = CellContentClass(
		COLOR(130),
		std::string("Potato Mine"),
		1,
		HPEnum::Standard,
		SpeedEnum::Instant)
	,
	SnowPea = CellContentClass(
		COLOR(74),
		std::string("Snow Peashooter"),
		Peashooter.Cost() * 1.75f,
		HPEnum::Standard,
		SpeedEnum::Normal)
	,
	Chomper = CellContentClass(
		COLOR(93),
		std::string("Chomper"),
		6,
		HPEnum::Tanky,
		SpeedEnum::Normal)
	,
	Repeater = CellContentClass(
		COLOR(46),
		std::string("Repeater"),
		Peashooter.Cost() * 2,
		HPEnum::Standard,
		SpeedEnum::Normal)

	//// Night Levels
	//PuffShroom = CellContentClass(COLOR(93), std::string("Puff-Shroom"), 0),
	//SunShroom = CellContentClass(COLOR(178), std::string("Sun-Shroom"), 1),
	//FumeShroom = CellContentClass(PuffShroom.Color(), std::string("Fume-Shroom"), 3),
	//GraveBuster = CellContentClass(COLOR(235), std::string("Grave Buster"), 3),
	//HypnoShroom = CellContentClass(COLOR(218), std::string("Hypno-Shroom"), 3),
	//ScaredyShroom = CellContentClass(COLOR(95), std::string("Scaredy-Shroom"), 1),
	//IceShroom = CellContentClass(SnowPea.Color(), std::string("Ice-Shroom"), 3),
	//DoomShroom = CellContentClass(GraveBuster.Color(), std::string("Doom-Shroom"), 5),

	//// Pool Levels
	//LilyPad = CellContentClass(COLOR(37), std::string("Lily Pad"), 1),
	//Squash = CellContentClass(COLOR(120), std::string("Squash"), 2),
	//Threepeater = CellContentClass(Peashooter.Color(), std::string("Threepeater"), Peashooter.Cost() * 3.25f, Peashooter.HP(), Peashooter.Speed()),
	//TangleKelp = CellContentClass(COLOR(100), std::string("Tangle Kelp"), 1),
	//Jalapeno = CellContentClass(CherryBomb.Color(), std::string("Jalapeno"), 5),
	//SpikeWeed = CellContentClass(WallNut.Color(), std::string("SpikeWeed"), 4),
	//TorchWood = CellContentClass(COLOR(202), std::string("TorchWood"), 7),
	//TallNut = CellContentClass(WallNut.Color(), std::string("TallNut"), WallNut.Cost() * 2.25f, WallNut.HP() * 2.25f),

	//// Fog Levels
	//SeaShroom = CellContentClass(COLOR(37), std::string("Sea-Shroom"), 0),
	//Plantern = CellContentClass(COLOR(226), std::string("Plantern"), 1),
	//Cactus = CellContentClass(COLOR(82), std::string("Cactus"), 5, Peashooter.HP(), Peashooter.Speed()),
	//Blover = CellContentClass(COLOR(22), std::string("Blover"), 4),
	//SplitPea = CellContentClass(Peashooter.Color(), std::string("Split Peashooter"), Peashooter.Cost() * 1.25f, Peashooter.HP(), Peashooter.Speed()),
	//StarFruit = CellContentClass(COLOR(184), std::string("Starfruit"), Peashooter.Cost() * 1.25f, Peashooter.HP(), Peashooter.Speed()),
	//Pumpkin = CellContentClass(COLOR(172), std::string("Pumpkin"), WallNut.Cost() * 2.25f, WallNut.HP()),
	//MagnetShroom = CellContentClass(COLOR(127), std::string("Magnet-Shroom"), 4),

	//// Roof Levels
	//CabbagePult = CellContentClass(COLOR(34), std::string("Cabbage-Pult"), Peashooter.Cost(), Peashooter.HP(), Peashooter.Speed()),
	//FlowerPot = CellContentClass(LilyPad.Color(), std::string("Flower Pot"), LilyPad.Cost(), Peashooter.HP()),
	//KernelPult = CellContentClass(Sunflower.Color(), std::string("Kernel-Pult"), Peashooter.Cost(), Peashooter.HP(), Peashooter.Speed()),
	//CoffeBean = CellContentClass(WallNut.Color(), std::string("Coffe Bean"), 3),
	//Garlic = CellContentClass(COLOR(255), std::string("Garlic"), 2),
	//UmbrellaLeaf = CellContentClass(Peashooter.Color(), std::string("Umbrella Leaf"), 4),
	//Marigold = CellContentClass(COLOR(220), std::string("Marigold"), Sunflower.Cost(), Sunflower.HP(), Sunflower.Speed()),
	//MelonPult = CellContentClass(CabbagePult.Color(), std::string("Melon-Pult"), CabbagePult.Cost() * 4, CabbagePult.HP(), CabbagePult.Speed())

	//// Extra Plants
	//GatlingPea = CellContentClass(COLOR(46), std::string("GatlingPea"), Repeater.Cost() * 2, 6, Repeater.Speed() * .5f),
	//TwinSunFlower = CellContentClass(COLOR(220), std::string("TwinSunFlower"), Sunflower.Cost() * 3, 6, Sunflower.Speed() * .5f),
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
const CellContentClass
	
	// Day Zombies
	Zombie = CellContentClass(
		COLOR(165),
		std::string("Zombie"),
		2 + InicialZombiesCurrency,
		HPEnum::ZombieHP,
		SpeedEnum::ZombieSpeed
	),
	FlagZombie = CellContentClass(
		COLOR(1),
		std::string("Flag Zombie"),
		0,
		HPEnum::ZombieHP * 1.25f,
		SpeedEnum::ZombieSpeed * 1.25f
	),
	ConeHead = CellContentClass(
		COLOR(208),
		std::string("Cone Head"),
		Zombie.Cost() * 2,
		HPEnum::ZombieHP * 2,
		SpeedEnum::ZombieSpeed
	),
	PoleVault = CellContentClass(
		COLOR(74),
		std::string("Pole Vault"),
		Zombie.Cost() * 4,
		HPEnum::ZombieHP * 2,
		SpeedEnum::ZombieSpeed / 2
	),
	BucketHead = CellContentClass(
		COLOR(255),
		std::string("Bucket Head"),
		Zombie.Cost() * 3,
		HPEnum::ZombieHP * 3,
		SpeedEnum::ZombieSpeed
	)

	//// Night Zombies
	//NewsPaper,
	//ScreenDoor,
	//FootBall,
	//DancingZombie,
	//BackUpZombie,

	//// Pool Zombies
	//DuckyTube,
	//SnorkelZombie,
	//Zomboni,
	//Bobsled,
	//DolphinRider,

	//// Fog Zombies
	//JackInTheBox,
	//Balloon,
	//Digger,
	//Pogo,
	//Yeti,

	//// Roof Zombies
	//Bungee,
	//Ladder,
	//Catapult,
	//Gargantuar,
	//Imp,
	//DrZomboss
;
