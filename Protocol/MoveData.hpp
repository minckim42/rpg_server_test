#pragma once
#pragma pack(1)
struct MoveData
{
	float	position[2];
	float	direction[2];
	float	speed;			// unit length per ms
};
#pragma pack()