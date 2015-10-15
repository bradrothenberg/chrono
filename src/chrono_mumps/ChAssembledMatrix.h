 #ifndef CHASSEMBLEDMATRIX_H
 #define CHASSEMBLEDMATRIX_H

#include "ChApiMumps.h"
#include "chrono/core/ChMatrix.h"
#include "chrono/core/ChSpmatrix.h"
#include <vector>

namespace chrono{
	class ChApiMumps ChAssembledMatrix : public ChSparseMatrixBase
	{

	private:
		int array_size;
		int array_memory_occupancy;
		int memory_augmentation;

		int mat_rows;
		int mat_cols;

		bool oneIndexed_format;
		bool safe_overwrite;

		std::vector<double> values;
		std::vector<int> rowIndex;
		std::vector<int> colIndex;


	public:
		ChAssembledMatrix(int mat_rows = 3, int mat_cols = 3, int nonzeros = 0);
		virtual ~ChAssembledMatrix(){};


		//
		void SetElement(int insrow, int inscol, double insval, bool overwrite = true) override;
		double GetElement(int row, int col);

		virtual void PasteMatrix(ChMatrix<>* matra, int insrow, int inscol, bool overwrite = true, bool transp = false) override;
		virtual void PasteMatrixFloat(ChMatrix<float>* matra, int insrow, int inscol, bool overwrite = true, bool transp = false) override;
		virtual void PasteClippedMatrix(ChMatrix<>* matra, int cliprow, int clipcol, int nrows, int ncolumns, int insrow, int inscol, bool overwrite = true) override;


		bool Resize(int row, int col, int nonzeros = 0) override;
		void Reset(int row, int col, int nonzeros = 0) override;
		void Trim();
		void Prune();

		int GetRows() const override { return mat_rows; }
		int GetColumns() const override { return mat_cols; }
		int GetArraysLength() const { return array_size; }


		// Auxiliary functions
		void SetOneIndexedArray(bool on_off);
		void SetSafeSetElementOverwrite(bool on_off) { safe_overwrite = on_off; }

		double* GetValuesAddress() const { return const_cast<double*>(values.data()); }
		int* GetColIndexAddress() const { return const_cast<int*>(colIndex.data()); }
		int* GetRowIndexAddress() const { return const_cast<int*>(rowIndex.data()); }

	};

}

#endif