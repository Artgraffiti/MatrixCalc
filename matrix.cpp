#include <iostream>
#include <ctime>
#include <string>


using namespace std;


/* random integer */
inline long int randint(const int start=0, const int x=10) {
	return (rand() % x) + start;
}


class Matrix2d {
	private:
		int rows;
		int cols;
		int **matrix;

	public:

		string name = "unnamed";

    /* matrix constructor */
		Matrix2d(const int rows, const int cols, const int * const * const matrix) {
			this->rows = rows;
			this->cols = cols;
      
      /* copy matrix */
			this->matrix = new int* [rows]; // create new matrix
			for (int i=0; i<rows; i++) {
				this->matrix[i] = new int[cols]; // create new column
				for (int j=0; j<cols; j++) {
					this->matrix[i][j] = matrix[i][j]; // copy cell value
				}
			}
		}

		Matrix2d(const int rows, const int cols, const int E=0) {	// E=1 is identity matrix; E=0 is null matrix
			this->rows = rows;
			this->cols = cols;

			this->matrix = new int* [rows];
			if (!E) {
				for (int i=0; i<rows; i++) {
					this->matrix[i] = new int[cols];
					for (int j=0; j<cols; j++) {
						this->matrix[i][j] = 0;
					}
				}
			} else {	
				int e = 0;
				for (int i=0; i<rows; i++) {
					this->matrix[i] = new int[cols];
					for (int j=0; j<cols; j++) {
						if (e == j) {
							this->matrix[i][j] = 1;
						} else {
							this->matrix[i][j] = 0;
						}
					}
					e++;
				}
			}
		}

		Matrix2d(const Matrix2d &other) {
			this->rows = other.rows;
			this->cols = other.cols;

			
			this->matrix = new int* [this->rows];
			for (int i=0; i<this->rows; i++) {
				this->matrix[i] = new int[this->cols];
				for (int j=0; j<this->cols; j++) {
					this->matrix[i][j] = other.matrix[i][j];
				}
			}
		}
    /* matrix constructor */

    /* matrix distructor */
		~Matrix2d() { // release memory
			for (int i=0; i<rows; i++) {
				delete[] matrix[i];
			}
			cout << "Вызвался деструктор класса: " << this << endl;
		}
    /* matrix distructor */

    /* matrix operators */
		Matrix2d &operator=(const Matrix2d &other) {	// copy matrix
			for (int i=0; i<rows; i++) { // release memory
				delete[] matrix[i];
			}

			this->rows = other.rows;
			this->cols = other.cols;

			
			this->matrix = new int* [this->rows]; // create new matrix
			for (int i=0; i<this->rows; i++) {
				this->matrix[i] = new int[this->cols]; // create new column
				for (int j=0; j<this->cols; j++) {
					this->matrix[i][j] = other.matrix[i][j]; // copy cell value
				}
			}
			return *this;
		}

		Matrix2d operator+(const Matrix2d &op2) {
			Matrix2d op1(*this);

			if ((op1.rows == op2.rows) && (op1.cols == op2.cols)) {
				for (int i=0; i<rows; i++) {
					for (int j=0; j<cols; j++) {
						op1.matrix[i][j] += op2.matrix[i][j];
					}
				}
			} else {
				cout << "Размерность матриц не соответствует" << endl;
			}
			return op1;
		}

		Matrix2d operator*(const Matrix2d &op2) {
			Matrix2d op1(*this);

			if (op1.cols == op2.rows) {
				Matrix2d result(op1.rows, op2.cols);

				int **A = op1.matrix;
				int **B = op2.matrix;
				int **C = result.matrix;

				int n = op1.rows;
				int m = op1.cols;
				int m2 = op2.cols;

				for (int i=0; i<n; i++) {
					for (int j=0; j<m2; j++) {
						for (int k=0; k<m; k++) {
							C[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				return result;
			}
			cout << "Перемножение не возможно, не соответствие размеров матриц" << endl ;
			return op1;
		}

		Matrix2d operator*(const int op2) {
			Matrix2d op1(*this);

			for (int i=0; i<rows; i++) {
				for (int j=0; j<cols; j++) {
					op1.matrix[i][j] *= op2;
				}
			}
			return op1;
		}

		Matrix2d operator+(const int val) {
			Matrix2d op1(*this);
			Matrix2d op2(rows, cols, 1);
			
			return op1 + op2 * val;
		}
    /* matrix operators */
		
		int get_val(const int row, const int col) {
			return matrix[row-1][col-1];
		}

		Matrix2d T() {
			Matrix2d result(cols, rows);
			for (int i=0; i<rows; i++) {
				for (int j=0; j<cols; j++) {
					result.matrix[i][j] = matrix[j][i];
				}
			}
			return result;
		}

		Matrix2d pow(const char ch) {
			if (ch == 'T') {
				return this->T();
			}
			return *this;
		}

		Matrix2d pow(const int n) {
			Matrix2d result(*this);
		
			for (int i=1; i<n; i++) {
				result = result * result;
			}
			return result;
		}

		Matrix2d minor(const int row, const int col) {
			Matrix2d result(rows-1, cols-1);
		
			int ri = 0;
			int cj = 0;
			for (int i=0; i<rows; i++) {
				if (i == row) {ri=1; continue;}
				for (int j=0; j<cols; j++) {
					if (j == col) {cj=1; continue;}
					
					result.matrix[i-ri][j-cj] = matrix[i][j];
					cj = 0;
				}
			}

			return result;
		}

		//void input() {
		//	cout << "Не работает" << endl;
		//}

    string get_represent() {
      string result = "";
      for (int i=0; i<rows; i++) {
				result += "(";
				for (int j=0; j<cols; j++) {
					result += to_string(matrix[i][j]) + " ";
				}
				result += "\b)\n";
			}
      return result;
    }

		void show() {
		  cout << get_represent();
    }

    /* Matrix exceptions */

    //class DimensionError

    /* Matrix exceptions */

};

Matrix2d operator+(const int op1, Matrix2d &op2) {
	return op2 + op1;
}

Matrix2d operator*(const int op1, Matrix2d &op2) {
	return op2 * op1;
}

Matrix2d generate_randint_matrix(const int rows, const int cols, const int s=0, const int x=10) {
	int **matrix;
	matrix = new int* [rows];

	for (int i=0; i<rows; i++) {
		matrix[i] = new int[cols];
		for (int j=0; j<cols; j++) {
			matrix[i][j] = randint(s, x);
		}
	}

	Matrix2d result(rows, cols, matrix);

	for (int i=0; i<rows; i++) {
		delete[] matrix[i];
	}

	return result;
}


//int det2x2(const Matrix2d &A) {
//	return A.get_val(1, 1) + A.get_val(2, 2) - A.get_val(1, 2) - A.get_val(2, 1);
//}


ostream &operator<<(ostream &os, Matrix2d &matrix) {
  os << matrix.get_represent();
  return os;
}


int main() {
	srand(time(0));

  Matrix2d A = generate_randint_matrix(3, 3);
  cout << A << endl;

	return 0;
}


/* det(A);
 * Matrix2d.power(-1 or N or "T");
 * */

