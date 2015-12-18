//#include "CMatrix.h"

RowProxy::RowProxy(CMatrix& iData,unsigned int iRow){data=iData; row=iRow;}
	

double   CMatrix::RowProxy::operator[] (const unsigned int col) const
	{
		data.check(row,col);
		return data.data->data[row][col];
	}
double & CMatrix::RowProxy::operator[] (const unsigned int col )
	{
		data.check(row,col);
		data.data = data.data->detach();
		return data.data->data[row][col];
	}
	
