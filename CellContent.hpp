// CellContent.hpp
#ifndef CELLCONTENT_HPP
#define CELLCONTENT_HPP

#include <string>
#include "Transform.hpp"

using std::string;

class CellContent {
public:
	CellContent();
	CellContent(string name, int health, int cost);

	void Set_Name(string value);
	string Get_Name();

	void Set_Health(int value);
	void Add_Health(int value);
	int Get_Health() const;

	void Set_Cost(int value);
	int Get_Cost() const;

	void Set_Transform(int x, int y);
	Coordenates Get_Transform() const;

private:
	string name;
	int health;

	int cost;
	Coordenates transform = { 0 , 0 };
};

#endif // !CELLCONTENT_HPP
