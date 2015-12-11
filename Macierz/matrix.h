#pragma once
#include<iostream>
/* TODO:
*	Constuctor from file

*	function invert
*	function transpose
*	ask Prof. about more stuff to write in C++
*/


namespace my_tools{
class CMatrix 
{
struct Matrix;
	Matrix *data;
	void check(unsigned int col,unsigned int row)
public:
	struct null_init_value : std::exception {
		const char* what() const noexcept {return "null initial value!\n";}
	};
	struct IndexOutOfRange{};
	struct WrongDim{};
	class RowProxy;
	CMatrix(){new Matrix(0,0,nullptr);}
	CMatrix(int r,int c, double identityValue=0.0, double defaultValue=0.0);
	CMatrix(std::fstream file);	
	CMatrix(const &CMatrix cpy_obj);

	std::ostream& operator<<(std::ostream& os, const CMatrix& obj);
	CMatrix operator+(const CMatrix& rhs);
	CMatrix operator-(const CMatrix& rhs);
	CMatrix operator*(const CMatrix& rhs);
	CMatrix& operator= (const CMatrix& rhs);

	RowProxy operator[](unsigned int x){
		return RowProxy(*data,x);
	}
	
	CMatrix& invert();
	CMatrix& transpose();
	 
}



CMatrix::CMatrix(const &CMatrix cpy_obj)
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
	data = new Matrix(r, c, init_values);
	
}

CMatrix::CMatrix(std::fstream file)
	{
		
	}


// Operators overloading
CMatrix::operator*(const CMatrix& rhs)
{
	if(cols!=rhs.rows)
		throw WrongDim;
	CMatrix temp(rows,rhs.cols);
	for(int i=0;i<rows;++i)
	{
		for(int j=0;j<cols; ++j)
		{
			for(int k=0;k<cols;++k)
			temp[i][j] +=  (*this)[i][k] * rhs[k][j];
		}
	}
	return temp;
}

CMatrix::operator+(const CMatrix& rhs)
{
		if(cols!=rhs.cols || rows!=rhs.rows)
			throw WrongDim;
		CMatrix temp();
		for(int i=0;i<rows;++i)
			for(int j=0;j<cols;++j)
				temp[i][j]= (*this)[i][j] + rhs[i][j];
			return temp;
}

CMatrix::operator-(const CMatrix& rhs)
{
		if(cols!=rhs.cols || rows!=rhs.rows)
			throw WrongDim;
		CMatrix temp();
		for(int i=0;i<rows;++i)
			for(int j=0;j<cols;++j)
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

std::ostream& CMatrix::operator<<(std::ostream& os, const CMatrix& obj)
{
	for(int i=0;i<obj.data->rows;++i)
	{
		for(int j=0; j<obj.data->cols;++i)
			os<<obj[i][j]<<' ';
			os<<std::endl;			
	}
	return os;
}

//Proxy class for double indexation
class Matrix::RowProxy
{
	Matrix& data;
	unsigned int row;
	RowProxy(Matrix& iData,unsigned int iRow){data=iData; row=iRow;}
	double const & operator[] (unsigned int col)
	{
		check(row,col);
		return data.data[row][col];
	}
	double & operator[] (unsigned int col )
	{
		check(row,col);
		data = data.detach();
		return data.data[row][col];
	}
	
}

// structure for physical data representation
struct CMatrix::Matrix
{
	int cols,rows;
	int ref_count;
	double **data;
	
	Matrix( int row, int col, double **initV)
	{
		ref_count =1;
		if(initV==nullptr)
			throw null_init_value;
		
		data=new double*[row];
		rows=row;
		cols=col;
		
		//copy stuff
		for(int i=0;i<row;++i)
		{
			data[i]=new double[col];
			for(int j=0;j<col;++j)
				data[i][j]=initV[i][j];
		}
		
		
	}

	
	~Matrix()
	{
		for(int i=0; i<rows; ++i)
			delete []data[i];
		delete []data;
	}
	
	
	Matrix* detach()
  {
    if(ref_count==1)
      return this;
    Matrix* t=new Matrix(rows, cols, data);
    --ref_count;
    return t;
  };
  
  Matrix& operator=(const Matrix& x)
  {
	  if(cols!=x.cols || rows!=x.rows)
	  {
		  double ** temp= new double*[x.rows];
		  for(int i=0;i<x.rows;++i)
		  {
			temp[i]=new double[x.cols];
			for(int j=0; j<x.cols;++j)
				temp[i][j]=x.data[i][j];
		  }
		  for(int i=0;i<rows;++i)
			  delete []data[i];
		  delete []data;
		  cols=x.cols;
		  rows = x.rows;
		  data = temp;
	  }else 
	  {
		  for(int i=0;i<x.rows;++i)
		  {
			for(int j=0; j<x.cols;++j)
				data[i][j]=x.data[i][j];
		  }
	  }
	  return *this;
  }
  
  
  private:
  Matrix(const Matrix&){}
	
}
}