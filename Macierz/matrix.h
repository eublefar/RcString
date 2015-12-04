#pragma once
#include<iostream>

class CMatrix 
{
struct Matrix;
	Matrix *data;
	
public:
	struct IndexOutOfRange{};
	struct WrongDim{};
	class Cref;
	CMatrix(int r,int c, double identityValue, double defaultValue=0.0);
	CMatrix(std::fstream file);	
	CMatrix(const &CMatrix cpy_obj);

	std::ostream& operator<<(std::ostream& os, const CMatrix& obj);
	CMatrix operator+(const CMatrix& rhs);
	CMatrix operator-(const CMatrix& rhs);
	CMatrix operator*(const CMatrix& rhs);
	
	CMatrix& invert();
	CMatrix& transpose();
	 
}
