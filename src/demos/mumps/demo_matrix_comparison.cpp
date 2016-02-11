#include "chrono_mkl/ChCSR3Matrix.h"
#include "chrono/core/ChSpmatrix.h"
#include "chrono/core/ChMatrixDynamic.h"

#include "chrono_mkl/ChLcpMklSolver.h"
#include <chrono_mumps/ChAssembledMatrix.h>


std::ofstream file;

class myTimer
{
private:
	double PCFreq;
	__int64 CounterStart;

public:
	myTimer() :PCFreq(0), CounterStart(0){ StartCounter(); };

	void StartCounter()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			std::cout << "QueryPerformanceFrequency failed!\n";

		PCFreq = double(li.QuadPart) / 1000.0;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}

	double GetCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}
} tim;

using namespace chrono;


template<class ChMatrixIN>
void PrintMatrix(ChMatrixIN& matrice){
	for (int i = 0; i < matrice.GetRows(); i++){
		for (int j = 0; j < matrice.GetColumns(); j++){
			printf("%f ", matrice.GetElement(i,j));
		}
		printf("\n");
	}
}

void LoadFromMatrix(ChMatrix<>& output_mat, std::string filename)
{
	std::ifstream my_file;
	my_file.open(filename);

	double temp;
	int row_sel = -1;
	for (row_sel = 0; row_sel < output_mat.GetRows(); row_sel++)
	{
		my_file >> temp;
		output_mat.SetElement(row_sel,0,temp);
	}
	my_file.close();
}

void bench_CSR3(int n, int nnz, int reads, double guess)
{
	srand(time(nullptr));

	// CSR3 benchmark
	//int n = 1000;
	//int nnz = n*n*0.1;
	//int reads = 10000;
	ChCSR3Matrix mat(n, n, nnz*guess);

	// Write CSR3
	int time_start = tim.GetCounter();
	for (int cont = 0; cont < nnz; ++cont)
	{
		mat.SetElement(rand() % n, rand() % n, rand());
	}
	mat.Compress();
	int time_end = tim.GetCounter();
	printf("\nCSR3 write: %d", time_end - time_start);
	file << std::endl << "CSR3 write: " << time_end - time_start;

	// Read CSR3
	
	double dummy = 0;
	for (int cont = 0; cont < reads; ++cont)
	{
		dummy = mat.GetElement(rand() % n, rand() % n);
	}
	int time_end2 = tim.GetCounter();
	printf("| CSR3 read: %d", time_end2 - time_end);
	file << "| CSR3 read: " << time_end2 - time_end;

}


void bench_assembled(int n, int nnz, int reads, double guess, bool duplicate_admitted, bool ovr_means_new, bool read)
{
	srand(time(nullptr));

	// assembled benchmark
	ChAssembledMatrix mat(n, n, nnz*guess, duplicate_admitted, ovr_means_new);

	// Write assembled
	int time_start = tim.GetCounter();
	for (int cont = 0; cont < nnz; ++cont)
	{
		mat.SetElement(rand() % n, rand() % n, rand(), true);
	}
	int time_end = tim.GetCounter();
	printf("\nAssembled (dupl: %d, ovr_new:%d) write: %d", duplicate_admitted, read, time_end - time_start);
	file << std::endl << "Assembled (dupl: %" << duplicate_admitted << ", ovr_new:" << read << ") write: " << time_end - time_start;

	if (read)
	{
		// Read assembled
		double dummy = 0;
		time_start = tim.GetCounter();
		for (int cont = 0; cont < reads; ++cont)
		{
			dummy = mat.GetElement(rand() % n, rand() % n);
		}
		time_end = tim.GetCounter();
		printf("| read: %d", time_end - time_start);
		file << "| read: " << time_end - time_start;
	}
	
}


void RunTable(int* row, int *col, int nnz, int n)
{
	for (int cont = 0; cont < nnz; cont++)
	{
		row[cont] = rand() % n;
		col[cont] = rand() % n;
	}
}

void test_write(ChCSR3Matrix& mat, int n, int nnz, int* row_list, int* col_list)
{
	srand(time(nullptr));
	// Write CSR3
	int time_start = tim.GetCounter();
	for (int cont = 0; cont < nnz; ++cont)
	{
		mat.SetElement(row_list[cont], col_list[cont], rand());
	}
	mat.Compress();
	int time_end = tim.GetCounter();
}



void test_repeat(int n, int nnz, double guess)
{
	std::vector<int> row_list;
	std::vector<int> col_list;

	row_list.resize(nnz);
	col_list.resize(nnz);

	RunTable(row_list.data(), col_list.data(), nnz, n);

	printf("\nTest soft reset with n:%d, nnz:%d, guess:%f,", n, nnz, guess);
	file << std::endl << "Test soft reset with n:" << n << ", nnz:" << nnz << ", guess :" << guess;
	
	int time_start = tim.GetCounter();
	ChCSR3Matrix mat1(n, n, nnz*guess);
	test_write(mat1, n, nnz, row_list.data(), col_list.data());
	int time_end1 = tim.GetCounter() - time_start;
	mat1.SetRowIndexLock(true);
	mat1.SetColIndexLock(true);
	mat1.Reset(n, n);
	test_write(mat1, n, nnz, row_list.data(), col_list.data());
	int time_end2 = tim.GetCounter() - time_end1;
	printf("\nFirst write: %d", time_end1);
	printf("\nSecond write: %d", time_end2);
	file << std::endl << "First write: " << time_end1 << "; Second write:" << time_end2;
}


void test_caller(int n, int nnz, double init_guess, int reads)
{
	printf("\nn: %d, nnz: %d, guess: %f, reads: %d", n, nnz, init_guess, reads);

	bench_CSR3(n, nnz, reads, init_guess);
	bench_assembled(n, nnz, reads, init_guess, true, true, true);
	bench_assembled(n, nnz, reads, init_guess, true, false, false);
}



int main(){
	srand(time(nullptr));
	//int n = 1500;
	//int nnz = 5e5;
	//double guess = 1.5;
	//int reads = 10000;

	file.open("test.txt");
	file << "RUN" << std::endl;

	int nnz[] = { 1e4, 1e5, 5e5, 1e6 };
	int n[] = { 10000 };
	double guess[] = {0.1, 0.5, 1.1};

	int num_n = sizeof(n) / sizeof(n[0]);
	int num_nnz = sizeof(nnz) / sizeof(nnz[0]);
	int num_guess = sizeof(guess) / sizeof(guess[0]);

	for (int cont_nnz = 0; cont_nnz < num_nnz; cont_nnz++)
		for (int cont_n = 0; cont_n < num_n; cont_n++)
			for (int cont_guess = 0; cont_guess < num_guess; cont_guess++)
			{
				printf("\nTest %d/%d", cont_nnz*(num_n*num_guess) + cont_n*num_guess + (cont_guess + 1), num_nnz*num_n*num_guess);
				file << "\nTest " << cont_nnz*(num_n*num_guess) + cont_n*num_guess + (cont_guess + 1) << "/" << num_nnz*num_n*num_guess;
				test_caller(n[cont_n], nnz[cont_nnz], guess[cont_guess], nnz[cont_nnz]);
			}
	printf("\n");

	test_repeat(1e4,1e4,1);
	test_repeat(1e4,1e5,1);
	test_repeat(1e4,5e5,1);
	test_repeat(1e4,1e6,1);

	file.close();


	getchar();
	return 0;

}