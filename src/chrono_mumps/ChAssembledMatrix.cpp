#include "chrono_mumps/ChAssembledMatrix.h"
#include <algorithm>

namespace chrono
{

	bool ChAssembledMatrix::Resize(int row, int col, int nonzeros)
	{
		mat_rows = row;
		mat_cols = col;
		if (nonzeros == 0)
			return true;

		if (nonzeros>array_memory_occupancy)
		{
			nonzeros = std::max(array_memory_occupancy + memory_augmentation, nonzeros);
			values.resize(nonzeros);
			rowIndex.resize(nonzeros);
			colIndex.resize(nonzeros);
			array_memory_occupancy = nonzeros;
		}

		return true;
	}

 	void ChAssembledMatrix::Reset(int row, int col, int nonzeros)
 	{
		mat_rows = row;
		mat_cols = col;
		array_size = 0;
		SetOneIndexedArray(false);
	}

	void ChAssembledMatrix::Trim()
	{
		values.resize(array_size);
		colIndex.resize(array_size);
		rowIndex.resize(array_size);
		array_memory_occupancy = array_size;
 	}

 	void ChAssembledMatrix::SetElement(int insrow, int inscol, double insval, bool overwrite)
	{
		if (oneIndexed_format)
		{
			insrow--;
			inscol--;
		}

		// search if the same element already exists;
		// the Mumps library accepts also duplicate entries (that will be summed),
		// but in this case, when we wanted to overwrite (most common) we should heve checked ALL the elements in the array
		// to avoid that some duplicated entries had left... this could increase time spent at every set element
		int el_sel;
		for (el_sel = 0; el_sel < array_size; el_sel++)
		{
			if (rowIndex[el_sel] == insrow && colIndex[el_sel] == inscol) // element found
			{
				if (overwrite)
					values[el_sel] = insval;
				else
					values[el_sel] += insval;
				break;
			}
		}

		// element not found
		if (el_sel == array_size)
		{
			Resize(mat_rows, mat_cols, array_size + 1);

			values[el_sel] = insval;
			rowIndex[el_sel] = insrow;
			colIndex[el_sel] = inscol;
			array_size++;
		}

	}

	double ChAssembledMatrix::GetElement(int row, int col)
	{
		if (oneIndexed_format)
		{
			row--;
			col--;
		}
		int el_sel;
		for (el_sel = 0; el_sel < array_size; el_sel++)
		{
			if (rowIndex[el_sel] == row && colIndex[el_sel] == col) // element found
			{
				return values[el_sel];
			}
		}

		return 0.0;

	}

	void ChAssembledMatrix::SetOneIndexedArray(bool on_off)
	{

		int delta = 0;
		if (on_off == true && oneIndexed_format == false)
		{
			oneIndexed_format = true;
			delta = +1;
		}
		else if (on_off == false && oneIndexed_format == true)
		{
			oneIndexed_format = false;
			delta = -1;
		}
		else
			return;

		for (int el_sel = 0; el_sel < array_size; el_sel++)
		{
			rowIndex[el_sel]=rowIndex[el_sel] + delta;
			colIndex[el_sel]=colIndex[el_sel] + delta;
		}

	}

	void ChAssembledMatrix::PasteMatrix(ChMatrix<>* matra, int insrow, int inscol, bool overwrite, bool transp)
	{
		int maxrows = matra->GetRows();
		int maxcols = matra->GetColumns();
		int i, j;

		if (transp)
		{
			for (i = 0; i < maxcols; i++){
				for (j = 0; j < maxrows; j++){
					if ((*matra)(j, i) != 0) this->SetElement(insrow + i, inscol + j, (*matra)(j, i), overwrite);
				}
			}
		}
		else
		{
			for (i = 0; i < maxrows; i++){
				for (j = 0; j < maxcols; j++){
					if ((*matra)(i, j) != 0) this->SetElement(insrow + i, inscol + j, (*matra)(i, j), overwrite);
				}
			}
		}
	}

	void ChAssembledMatrix::PasteMatrixFloat(ChMatrix<float>* matra, int insrow, int inscol, bool overwrite, bool transp)
	{
		int maxrows = matra->GetRows();
		int maxcols = matra->GetColumns();
		int i, j;

		if (transp)
		{
			for (i = 0; i < maxcols; i++){
				for (j = 0; j < maxrows; j++){
					if ((*matra)(j, i) != 0) this->SetElement(insrow + i, inscol + j, (*matra)(j, i), overwrite);
				}
			}
		}
		else
		{
			for (i = 0; i < maxrows; i++){
				for (j = 0; j < maxcols; j++){
					if ((*matra)(i, j) != 0) this->SetElement(insrow + i, inscol + j, (*matra)(i, j), overwrite);
				}
			}
		}
	};

	void ChAssembledMatrix::PasteClippedMatrix(ChMatrix<>* matra, int cliprow, int clipcol, int nrows, int ncolumns, int insrow, int inscol, bool overwrite)
	{
		for (int i = 0; i < nrows; ++i)
			for (int j = 0; j < ncolumns; ++j)
				this->SetElement(insrow + i, inscol + j, matra->GetElement(i + cliprow, j + clipcol), overwrite);
	}

}