#include <iostream>
#include <ctime>
#include <string>


using namespace std;


/* Matrix errors */
class DimensionMismatchError: public exception {
  public:
    DimensionMismatchError():msg_(this->msg_) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }
  private:
      string msg_="matrix sizes do not match";
};
/* Matrix errors */


class Matrix2d {
	private:
		int rows;
		int cols;
		long int **matrix;

    // Friends
    friend Matrix2d generate_randint_matrix(const int rows, const int cols, const int s, const int x);	
	public:

		string name = "unnamed";

    /* matrix constructor */
		Matrix2d(const int rows, const int cols, const long int * const * const matrix) {
			this->rows = rows;
			this->cols = cols;
      
      /* copy matrix */
			this->matrix = new long int* [rows]; // create new matrix
			for (int i=0; i<rows; i++) {
				this->matrix[i] = new long int[cols]; // create new column
				for (int j=0; j<cols; j++) {
					this->matrix[i][j] = matrix[i][j]; // copy cell value
				}
			}
		}

		Matrix2d(const int rows, const int cols, const int E=0) {	// E=1 is identity matrix; E=0 is null matrix
			this->rows = rows;
			this->cols = cols;

			this->matrix = new long int* [rows];
			if (!E) { // null matrix
				for (int i=0; i<rows; i++) {
					this->matrix[i] = new long int[cols];
					for (int j=0; j<cols; j++) {
						this->matrix[i][j] = 0;
					}
				}
			} else {	// identity matrix
				int e = 0;
				for (int i=0; i<rows; i++) {
					this->matrix[i] = new long int[cols];
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

		Matrix2d(const Matrix2d &other) { // copy other Matrix2d
			this->rows = other.rows;
			this->cols = other.cols;

			
			this->matrix = new long int* [this->rows];
			for (int i=0; i<this->rows; i++) {
				this->matrix[i] = new long int[this->cols];
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

			
			this->matrix = new long int* [this->rows]; // create new matrix
			for (int i=0; i<this->rows; i++) {
				this->matrix[i] = new long int[this->cols]; // create new column
				for (int j=0; j<this->cols; j++) {
					this->matrix[i][j] = other.matrix[i][j]; // copy cell value
				}
			}
			return *this;
		}

		Matrix2d operator+(const int val) {
			Matrix2d op1(*this);
			Matrix2d op2(rows, cols, 1);
			
			return op1 + op2 * val;
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
        throw DimensionMismatchError();
      }
      
      return op1;
		}

		Matrix2d operator*(const Matrix2d &op2) {
			Matrix2d op1(*this);

			if (op1.cols == op2.rows) {
				Matrix2d result(op1.rows, op2.cols);

				long int **A = op1.matrix;
				long int **B = op2.matrix;
				long int **C = result.matrix;

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
      throw DimensionMismatchError();
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

    /* matrix operators */
		

    /* matrix methods */
		int get_val(const int row, const int col) {
			return matrix[row-1][col-1];
		}

		Matrix2d T() { // transposition
			Matrix2d result(cols, rows);
			for (int i=0; i<rows; i++) {
				for (int j=0; j<cols; j++) {
					result.matrix[i][j] = matrix[j][i];
				}
			}
			return result;
		}

		Matrix2d pow(const char ch) { // transposition
			if (ch == 'T') {
				return this->T();
			}
			return *this;
		}

		Matrix2d pow(const int n) { // exponentiation
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

    long int det() {
      long int result = 0;
      if (rows == cols && rows == 2) {
        result = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
      }
      return result;
    }

    string get_represent() { // create string view
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
    /* matrix methods */

};


/* Matrix opertaor for left operand */
Matrix2d operator+(const int op1, Matrix2d &op2) {
	return op2 + op1;
}

Matrix2d operator*(const int op1, Matrix2d &op2) {
	return op2 * op1;
}
/* Matrix opertaor for left operand */


/* random integer */
inline long int randint(const int start=0, const int x=10) {
	return (rand() % x) + start;
}


/* matrix generators */
Matrix2d generate_randint_matrix(const int rows, const int cols, const int s=0, const int x=10) { // matrix with random integers	
	Matrix2d result(rows, cols, 0);

	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			result.matrix[i][j] = randint(s, x);
		}
	}

	return result;
}
/* matrix generators */


ostream &operator<<(ostream &os, Matrix2d &matrix) { // Matrix stream
  os << matrix.get_represent();
  return os;
}


int main() {
	srand(time(0));

  Matrix2d A = generate_randint_matrix(2, 2);
  cout << A << endl;

  cout << "det(A): " << A.det() << endl;

  return 0;
}


/* det(A);
 * Matrix2d.power(-1 or N or "T");
 * */

