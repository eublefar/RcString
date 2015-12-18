#pragma once
#include <fstream>
//
//
//

struct null_init_value : public std::exception {
	const char* what() const noexcept {return "null initial value!\n";}
};

struct IndexOutOfRange: public std::exception {
const char* what() const noexcept {return "index out of range!\n";}
};

struct WrongDim: public std::exception {
const char* what() const noexcept {return "Wrong Matrix Dimensions";}
};


//
//
//



struct Matrix 
{
	int cols,rows;
	int ref_count;
	double **data;
	
	Matrix( int row, int col, double **initV) throw(null_init_value)
	{
		ref_count =1;
		if(initV==nullptr)
			throw null_init_value();
		
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
	
};





class CMatrix {


	friend class Matrix;
	Matrix *data;
	void check(unsigned int col,unsigned int row);
public:
	class RowProxy
	{
	CMatrix *data;
	unsigned int row;
public:
	RowProxy( CMatrix* iData,unsigned int iRow);
	const double  & operator[] (const unsigned int col) const;
	double & operator[] (const unsigned int col );
	
	};

	CMatrix();
	CMatrix(int r,int c, double identityValue=0.0, double defaultValue=0.0);
	CMatrix(std::ifstream &file);	
	CMatrix(const CMatrix &cpy_obj);

	friend std::ostream& operator<<(std::ostream& os, const CMatrix& obj);
	CMatrix operator+(const CMatrix& rhs);
	CMatrix operator-(const CMatrix& rhs);
	CMatrix operator*(const CMatrix& rhs) ;
	CMatrix& operator= (const CMatrix& rhs);

	RowProxy operator[](const int x);
	
	//CMatrix& invert();
	//CMatrix& transpose();
	 
};

