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
    double **init_values;
    init_values = new double*[r];
    for(int i=0;i<r;++i)
            {
                init_values[i]=new double[c];
                for(int j=0;j<c;++j)
                {
                    if(i==j)
                        init_values[i][j]=identityValue;
                    else
                        init_values[i][j]=defaultValue;
                }
            }

	data = new Matrix(r, c, (double**)init_values);
	
}

CMatrix::CMatrix(std::ifstream &file)
	{
		if(!file)
			throw std::exception();
		unsigned int columns, rows;
		double** init_values;
		file>>rows;		
		file>>columns;
		
		init_values = new double*[rows];
    			for(int i=0;i<rows;++i)
            			{
                			init_values[i]=new double[columns];
				}
		data = new Matrix(rows, columns, (double**)init_values);

		for(int i=0;i<rows ;++i)
			for(int j=0;j<columns;++j)
				file>>(*this)[i][j];
	}


// Operators overloading
CMatrix CMatrix::operator*(const CMatrix& rhs)
{
	if(data->cols!=rhs.data->rows)
		throw WrongDim();
	CMatrix temp(data->rows,rhs.data->cols);
    CMatrix& t=*this;
	for(int i=0;i<data->rows;++i)
	{
		for(int j=0;j<rhs.data->cols; ++j)
		{
			for(int k=0;k<data->cols;++k)
            (temp[i][j]) +=  (t[i][k]) * (rhs[k][j]);
		}
	}
	return temp;
}

CMatrix CMatrix::operator+(const CMatrix& rhs) 
{
		if(data->cols!=rhs.data->cols || data->rows!=rhs.data->rows)
			throw WrongDim();
        CMatrix temp(data->rows,data->cols);
        CMatrix& t=*this;
		for(int i=0;i<data->rows;++i)
			for(int j=0;j<data->cols;++j)
                (temp[i][j])= (t[i][j]) + (rhs[i][j]);
			return temp;
}

CMatrix CMatrix::operator-(const CMatrix& rhs)
{
		if(data->cols!=rhs.data->cols || data->rows!=rhs.data->rows)
			throw WrongDim();
        CMatrix temp(data->rows,data->cols);
        CMatrix& t=*this;
		for(int i=0;i<data->rows;++i)
			for(int j=0;j<data->cols;++j)
                (temp[i][j])= (t[i][j]) - (rhs[i][j]);
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
    for(int i=0;i< (obj.data->rows);++i)
	{
        for(int j=0; j< (obj.data->cols);++j)
			os<<obj[i][j]<<' ';

			os<<std::endl;			
	}
	return os;
}

void CMatrix::check(unsigned int row,unsigned int col)
{
    if(col >= data->cols || row >= data->rows)
        throw(IndexOutOfRange());
}

