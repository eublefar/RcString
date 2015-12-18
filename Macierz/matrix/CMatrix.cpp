#include "CMatrix.h"


CMatrix::RowProxy::RowProxy( CMatrix * iData,unsigned int iRow){data=iData; row=iRow;}
	

const double &  CMatrix::RowProxy::operator[] (const unsigned int col) const
	{
		data->check(row,col);
		return data->data->data[row][col];
	}
double & CMatrix::RowProxy::operator[] (const unsigned int col )
	{
		data->check(row,col);
		data->data = data->data->detach();
		return data->data->data[row][col];
	}


CMatrix::CMatrix(){data = new Matrix(0, 0, nullptr);}


CMatrix::RowProxy CMatrix::operator[](const int x){
		return RowProxy(this,x);
	}



CMatrix::CMatrix(const CMatrix &cpy_obj)
{
	data = cpy_obj.data;
	data->ref_count++;
}





CMatrix::CMatrix(int r,int c, double identityValue, double defaultValue)
{
	double init_values[r][c];
	for(int i=0;i<r;++i)
		for(int j=0;j<c;++j)
		{
			if(i==j)
				init_values[i][j]=identityValue;
			else
				init_values[i][j]=defaultValue;
		}
	data = new Matrix(r, c, (double**)init_values);
	
}

CMatrix::CMatrix(std::ifstream &file)
	{
		if(!file)
			throw std::exception();
		unsigned int columns, rows;
		file>>columns;
		file>>rows;
		CMatrix(columns,rows);
		for(int i=0;i<columns;++i)
			for(int j=0;j<rows;++j)
				file>>(*this)[i][j];
	}


// Operators overloading
CMatrix CMatrix::operator*(const CMatrix& rhs)
{
	if(data->cols!=rhs.data->rows)
		throw WrongDim();
	CMatrix temp(data->rows,rhs.data->cols);
	for(int i=0;i<data->rows;++i)
	{
		for(int j=0;j<data->cols; ++j)
		{
			for(int k=0;k<data->cols;++k)
			temp[i][j] +=  ((*this)[i][k]) * (rhs[k][j]);
		}
	}
	return temp;
}

CMatrix CMatrix::operator+(const CMatrix& rhs) 
{
		if(data->cols!=rhs.data->cols || data->rows!=rhs.data->rows)
			throw WrongDim();
		CMatrix temp();
		for(int i=0;i<data->rows;++i)
			for(int j=0;j<data->cols;++j)
				temp[i][j]= (*this)[i][j] + rhs[i][j];
			return temp;
}

CMatrix CMatrix::operator-(const CMatrix& rhs)
{
		if(data->cols!=rhs.data->cols || data->rows!=rhs.data->rows)
			throw WrongDim();
		CMatrix temp();
		for(int i=0;i<data->rows;++i)
			for(int j=0;j<data->cols;++j)
				temp[i][j]= (*this)[i][j] - rhs[i][j];
			return temp;
}

CMatrix& CMatrix::operator=(const CMatrix& rhs)
{
	if(data->ref_count==1){
    *data = *(rhs.data);
	rhs.data->ref_count++;
	}
  else
  {
    Matrix* t = new Matrix(rhs.data->rows,rhs.data->cols, rhs.data->data );
    data->ref_count--;
    data= t;
  };
  return *this;
}

std::ostream& operator<<(std::ostream& os, const CMatrix& obj)
{
	for(int i=0;i<obj.data->rows;++i)
	{
		for(int j=0; j<obj.data->cols;++i)
			os<<obj[i][j]<<' ';

			os<<std::endl;			
	}
	return os;
}



