#pragma once

typedef struct D3DXFLOAT16
{
#ifdef __cplusplus
public:
	D3DXFLOAT16() {};
	D3DXFLOAT16(FLOAT);
	D3DXFLOAT16(CONST D3DXFLOAT16&);

	// casting
	operator FLOAT ();

	// binary operators
	BOOL operator == (CONST D3DXFLOAT16&) const;
	BOOL operator != (CONST D3DXFLOAT16&) const;

protected:
#endif //__cplusplus
	WORD value;
} D3DXFLOAT16, *LPD3DXFLOAT16;

typedef struct D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX() {};
	D3DXMATRIX(CONST FLOAT *);
	D3DXMATRIX(CONST D3DMATRIX&);
	D3DXMATRIX(CONST D3DXFLOAT16 *);
	D3DXMATRIX(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
		FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
		FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
		FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44);


	// access grants
	FLOAT& operator () (UINT Row, UINT Col);
	FLOAT  operator () (UINT Row, UINT Col) const;

	// casting operators
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXMATRIX& operator *= (CONST D3DXMATRIX&);
	D3DXMATRIX& operator += (CONST D3DXMATRIX&);
	D3DXMATRIX& operator -= (CONST D3DXMATRIX&);
	D3DXMATRIX& operator *= (FLOAT);
	D3DXMATRIX& operator /= (FLOAT);

	// unary operators
	D3DXMATRIX operator + () const;
	D3DXMATRIX operator - () const;

	// binary operators
	D3DXMATRIX operator * (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator + (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator - (CONST D3DXMATRIX&) const;
	D3DXMATRIX operator * (FLOAT) const;
	D3DXMATRIX operator / (FLOAT) const;

	friend D3DXMATRIX operator * (FLOAT, CONST D3DXMATRIX&);

	BOOL operator == (CONST D3DXMATRIX&) const;
	BOOL operator != (CONST D3DXMATRIX&) const;

} D3DXMATRIX, *LPD3DXMATRIX;



class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator=(Vector3 v)
	{
		return Vector3(v.x,v.y,v.z);
	}
};