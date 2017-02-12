#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <numeric>

using namespace std;


//NOTE:
//vectors are represented as COLUMN MATRICES
//matrices are represented as a SERIES of COLUMNS, not the other way around
//i.e.
//vec = 1
//	2
//	3
//
//mat = 1 4 7
//	2 5 8
//	3 6 9

//print vectors
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if ( !v.empty() ) {
		out << "[";
		std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}
//some extra vector operations, make c++ vector behave like linear algebra vector
template <typename T>
vector<T> operator+(const vector<T>& v1, const vector<T>& v2) {
	vector<T> result;
	for(int i = 0; i < v1.size(); ++i) {
		result.push_back(v1[i] + v2[i]);
	}
	return result;
}

template <typename T>
vector<T> operator-(const vector<T>& v1, const vector<T>& v2) {
	vector<T> result;
	for(int i = 0; i < v1.size(); ++i) {
		result.push_back(v1[i] - v2[i]);
	}
	return result;
}

//dot product
template <typename T>
T operator*(const vector<T>& v1, const vector<T>& v2) {
	if(v1.size() != v2.size()) {
		cout<<"incompatible sizes: "<<v1.size()<<" and "<<v2.size()<<endl;
		return 0;
	}
	T accum = 0;
	for(int i = 0; i < v1.size(); ++i) {
		//cout<<"adding "<<i<<": "<<v1[i]<<" * "<<v2[i]<<endl;
		accum+=v1[i]*v2[i];
		//cout<<"accum now at "<<accum<<endl;
	}
	return accum;

	//return inner_product(v1.begin(), v1.end(), v2.begin(), 0);
	
}

// mat = vector<vector<int>>

template <typename T>
class matrix {
	private:
		vector<vector<T>> data;
	public:
			matrix(initializer_list<initializer_list<T>> init){
			for(auto i = init.begin(); i < init.end(); ++i) {
				data.push_back(*i);
			}
		}
		matrix(size_t width, size_t height) {
			for(int i = 0; i < width; ++i) {
				data.push_back(vector<T>(height));
			}
		}
		vector<T> col(int index) const;
		vector<T> row(int index) const;
		const T val(int r, int c) const;
		vector<T>& operator[](int index); //can't be const because []=
		const vector<T>& operator[](int index) const;
		bool operator==(matrix<T> other) const;
		bool operator!=(matrix<T> other) const;
		matrix<T> operator+(const matrix<T>& other) const;
		matrix<T> operator-(const matrix<T>& other) const;
		matrix<T> operator*(const matrix<T>& other) const;
		// division not defined (by me)
		bool empty() const;
		size_t height() const;//length of each vector
		size_t width() const; //# of vectors
};

//standard notation
template <typename T>
const T matrix<T>::val(int r, int c) const {
	if(r-1 >= 0 && r-1 < this->height() && c-1 >= 0 && c-1 < this->width()) {
		return (*this)[c-1][r-1];
	} else {
		cout<<"not within matrix bounds: "<<r<<","<<c<<endl;
		return 0;
	}
}

template <typename T>
vector<T> matrix<T>::col(int index) const {
	return data[index];
}
template <typename T>
vector<T> matrix<T>::row(int index) const {
	vector<T> ret = {};
	for(auto i : data) {
		ret.push_back(i[index]);
	}
	return ret;
}

//c++ notation
//matrix[x][y]
template <typename T>
vector<T>& matrix<T>::operator[](int index) {
	return data[index];
}
template <typename T>
const vector<T>& matrix<T>::operator[](int index) const {
	return data[index];
}

template <typename T>
bool matrix<T>::operator==(matrix<T> other) const {
	if(this->width() != other.width() || this->height() != other.height()) 
		return false;
	for(int i = 0; i < this->width(); ++i) {
		if((*this)[i] != other[i])
			return false;
	}
	return true;
}
template <typename T>
bool matrix<T>::operator!=(matrix<T> other) const {
	if(this->width() != other.width() || this->height() != other.height()) 
		return true;
	for(int i = 0; i < this->width(); ++i) {
		if((*this)[i] != other[i])
			return true;
	}
	return false;
}

template <typename T>
bool matrix<T>::empty() const {
	return data.empty();
}
template <typename T>
size_t matrix<T>::height() const {
	return data[0].size();
}
template <typename T>
size_t matrix<T>::width() const {
	return data.size();
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const matrix<T>& m) {
	if ( !m.empty() ) {
		for(int i = 0; i < m.width(); ++i) {
			out << "[" << m[i] << "]"<< endl;
		}
	}
	return out;
}

//MATRIX OPERATIONS
template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
	matrix<T> ret = matrix<T>(other.width(), this->height());
	if(this->width() != other.height()) {
		cout<<"incompatible sizes: "<<this->width()<<" is not equal to "<<other.height()<<endl;
		return ret;
	}
	for(int i = 0; i < other.width(); ++i) {
		for(int j = 0; j < this->height(); ++j) {
			//cout<<"---"<<endl;
			//cout<<this->row(j)<<endl;
			//cout<<other.col(i)<<endl;
			//cout<<"dot product: "<<this->row(j) * other.col(i)<<endl;
			ret[i][j] = this->row(j) * other.col(i);
			//cout<<"assigned: "<<ret[i][j]<<endl;
		}
	}
	//cout<<ret<<endl;
	return ret;
}


//SOLVING SYSTEM
//TODO

template<typename T>
matrix<T> identity(size_t size) {
	matrix<T> id = matrix<T>(size, size);
	for(int i = 0; i < id.width(); ++i) {
		for(int j = 0; j < id.height(); ++j) {
			id[i][i] = static_cast<T>(1);
		}
	}
	return id;
}

template<typename T>
void printSystem(matrix<T> system) {
	for(int i = 0; i < system.height(); ++i) {
		for(int j = 0; j < system.width(); ++j) {
			if(j == system.width() - 1) {
				cout<<"\b\b"<<"= "<<system[j][i];
			} else {
				cout<<system[j][i]<<char('x'+j)<<" + ";
			}
		}
		cout<<endl;
	}
}

template<typename T>
vector<T> solveSystem(matrix<T> system) {
	vector<T> solutions = vector<T>(0);
	cout<<"solving system of "<<system.height()<<" equations."<<endl;
	printSystem(system);

	//start on second row, work down to reduce to upper triangular form
	for(int i = 1; i < system.height(); ++i) {
		for(int j = 0; j < i; ++j) {
			T pivot = system[j][i]/system[j][j];
			matrix<T> elim = identity<T>(system.height());
			elim[j][i] = -pivot;
			cout<<"elimination matrix: "<<i+1<<j+1<<endl;
			cout<<elim<<endl;
			system = elim * system;
			printSystem(system);
		}
	}

	//produce solutions via backwards substitution
	for(int i = system.height()-1; i >= 0; --i) {
		T subtractor = 0;
		cout<<"solving for "<<char('x' + i)<<endl;
		for(int j = 0; j < solutions.size(); ++j) {
			cout<<"(in step) solving for "<<char('z'- j)<<endl;
			cout<<"subtracting "<<system[system.width()-2-j][i]<<"*"<<solutions[j]<<" from "<<system[system.width()-1][i]<<endl;
			system[i+2][i] -= solutions[j]*system[system.width()-2-j][i];
		}
		solutions.push_back(system[system.width() - 1][i] / system[i][i]); 
	}

	reverse(solutions.begin(), solutions.end());
	return solutions;
}

int main(int argc, char **argv) {

	//last column is solutions, always
	matrix<double> mat = {{-1.0, 2.0, 1.0}, 
			      {2.0, 3.0, 1.0}, 
			      {1.0, -3.0, 2.0}, 
			      {2.0, 3.0, 4.0}};
	cout<<solveSystem(mat)<<endl;

	cout<<identity<int>(3)<<endl;
	return 0;
}
