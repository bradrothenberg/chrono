#include "chrono_mumps/ChAssembledMatrix.h"
#include <algorithm>

namespace chrono
{

	ChAssembledMatrix::ChAssembledMatrix(int mat_rows, int mat_cols, int nonzeros, bool duplicate_adm, bool ovr_means_new) :
		array_size(0),
		memory_augmentation(4),
	    mat_rows(mat_rows),
		mat_cols(mat_cols),
		oneIndexed_format(false),
		duplicates_allowed(duplicate_adm),
		overwrite_means_new(ovr_means_new)

	{
		assert(nonzeros >= 0 && mat_rows > 0 && mat_cols > 0);

		if (nonzeros == 0)
			nonzeros = static_cast<int>(mat_rows*mat_cols*SPM_DEF_FULLNESS);

		ChAssembledMatrix::Reset(mat_rows, mat_cols, nonzeros);
	}

	bool ChAssembledMatrix::Resize(int row, int col, int nonzeros)
	{
		mat_rows = row;
		mat_cols = col;
		if (nonzeros == 0)
			return true;

		values.resize(nonzeros);
		rowIndex.resize(nonzeros);
		colIndex.resize(nonzeros);

		return true;
	}

 	void ChAssembledMatrix::Reset(int row, int col, int nonzeros)
 	{
		mat_rows = row;
		mat_cols = col;

		values.reserve(nonzeros);
		rowIndex.reserve(nonzeros);
		colIndex.reserve(nonzeros);
		array_size = 0;
		SetOneIndexedArray(false);
	}

	void ChAssembledMatrix::Trim()
	{
		values.resize(array_size);
		colIndex.resize(array_size);
		rowIndex.resize(array_size);
 	}

	void ChAssembledMatrix::Prune()
	{
		int cont = 0;
		int shift = 0;
		for (cont = 0; cont<array_size; cont++)
		{
			if (values[cont] == 0)
			{
				break;
			}
				
		}

		if (cont == 0)
		{
			printf("Matrix inconsistent");
			assert(0);
		}
		

		for (int cont_source = cont; cont_source < array_size; cont_source++)
		{
			if (values[cont_source] != 0)
			{
				values[cont_source - shift] = values[cont_source];
				rowIndex[cont_source - shift] = rowIndex[cont_source];
				colIndex[cont_source - shift] = colIndex[cont_source];
			}
			else
				shift++;	
		}
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

		bool add_element = true;

		if ( (overwrite && !overwrite_means_new) ||
			 (!overwrite && !duplicates_allowed) ) // case: add to an eventually existing item
			
		{
			for (int el_sel = 0; el_sel < array_size; el_sel++)
			{
				if (rowIndex[el_sel] == insrow && colIndex[el_sel] == inscol) // element found
				{
						(overwrite) ? values[el_sel] = insval : values[el_sel] += insval;

						if (!duplicates_allowed)
							break;

						add_element = false;
						insval = 0;
				}
			} //end loop
		}
		

		// element not found or not overwrite or overwrite_new
		if (add_element)
		{
			int entry_point = array_size;
			Resize(mat_rows, mat_cols, array_size + 1);

			values[entry_point] = insval;
			rowIndex[entry_point] = insrow;
			colIndex[entry_point] = inscol;
			array_size++;
		}

	}

	double ChAssembledMatrix::GetElement(int row, int col) const
	{
		if (oneIndexed_format)
		{
			row--;
			col--;
		}

		double value = 0;
		for (int el_sel = 0; el_sel < array_size; el_sel++)
		{
			if (rowIndex[el_sel] == row && colIndex[el_sel] == col) // element found
			{
				value += values[el_sel];
				if (!duplicates_allowed)
					break;
			}
		}

		return value;

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