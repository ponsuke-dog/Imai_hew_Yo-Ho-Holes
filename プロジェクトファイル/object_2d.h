//	object_2d.h
//	
//
//	2024.12.22			‘åŽR—½‘¾˜N

#ifndef OBJECT_2D_Hs
#define OBJECT_2D_H

#include "directx.h"

class Object2d
{
private:
	XMFLOAT2 m_Position = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_Size = XMFLOAT2(0.0f, 0.0f);
	float m_Rotation = 0.0f;
	
public:
	Object2d() = default;
	Object2d(XMFLOAT2 position, XMFLOAT2 size, float rotation = 0.0f) :m_Position(position), m_Size(size), m_Rotation(rotation){}
	virtual ~Object2d() {};

	virtual void Update() = 0;
	virtual void Draw()const = 0;

	void SetPosition(XMFLOAT2 position) { m_Position = position; }
	const XMFLOAT2 GetPosition() const  { return m_Position; }

	void SetSize(XMFLOAT2 size) { m_Size = size; }
	const XMFLOAT2 GetSize() const  { return m_Size; }

	void SetRotation(float rotation) { m_Rotation = rotation; }
	const float GetRotation() const { return m_Rotation; }

};

#endif // OBJECT_2D_H
