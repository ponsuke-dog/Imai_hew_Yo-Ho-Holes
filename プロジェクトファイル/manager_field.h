//	manager_field.h
//	
//
//	2024.12.20			‘åŽR—½‘¾˜N

#ifndef MANAGER_FIELD_H
#define MANAGER_FIELD_H

#include "directx.h"

class Ship;

class FieldManager
{
public:
	FieldManager() = delete;
	FieldManager(Ship* ship);
	~FieldManager();

};


#endif // MANAGER_FIELD_H