#include <cmath>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

#include <cwchar>

double TimeTrans(double time, int32_t modificator) {
  if (modificator < 0) {
    for (; modificator != 0; ++modificator) {
      time *= 60.;
    }
  } else if (modificator > 0) {
    for (; modificator != 0; --modificator) {
      time /= 60.;
    }
  }

  return time;
}

void F1() {
  std::unordered_map<uint8_t, int32_t> times{{'s', 1}, {'m', 2}, {'h', 3}};

  std::cout << "Введите текст в формате \"время единица_измерения "
               "единица_измерения\":\n";

  double num;
  uint8_t ch1, ch2;
  std::cin >> num >> ch1 >> ch2;

  std::cout << TimeTrans(num, times.at(ch2) - times.at(ch1)) << ' ' << ch2;
}

double SumPersent(double init_sum, int32_t years) {
  double persent = 1.;
  persent += init_sum / 10000 * 0.003;
  if (years <= 3) {
    persent += 0.03;
  } else if (years > 3 && years <= 6) {
    persent += 0.05;
  } else {
    persent += 0.02;
  }

  for (; years != 0; --years) {
    init_sum *= persent;
  }

  return init_sum;
}

void F2() {
  std::cout << "Введите сумму и срок:\n";
  double sum;
  int32_t years;
  std::cin >> sum >> years;
  if (sum < 30000) {
    throw std::logic_error("Минимальная сумма 30000");
  } else if (years < 1) {
    throw std::logic_error("Нереальный срок");
  }

  std::cout << SumPersent(sum, years) - sum;
}

void F3() {
  std::cout << "Введите начало и конец и диапазона:";
  int32_t begin, end;
  std::cin >> begin >> end;
  if (end > begin) {
    throw std::logic_error("начало больше конца");
  } else if (begin == end) {
    throw std::logic_error("Нет символов");
  }
  std::vector<int32_t> i_vec;
  i_vec.reserve(static_cast<size_t>(begin - end));

  for (; begin != end; ++begin) {
    bool is_pure = true;
    for (int i = 2; i != begin; ++i) {
      if (begin % i == 0) {
        is_pure = false;
        break;
      }
    }
    if (is_pure) {
      i_vec.push_back(begin);
    }
  }
  if (i_vec.size() == 0) {
    throw std::logic_error("диапазон без простых чисел");
  } else {
    for (auto &val : i_vec) {
      std::cout << val << ' ';
    }
    std::cout << '\n';
  }
}

class Matrix {
public:
  Matrix(std::vector<std::vector<double>> value) : matrix_(std::move(value)) {}
  Matrix(size_t M, size_t N) {
    matrix_ = std::vector<std::vector<double>>(M, std::vector<double>(N, 0.));
  }
  Matrix(const Matrix &mat) { matrix_ = mat.matrix_; }
  Matrix(Matrix &&mat) : matrix_(std::move(mat.matrix_)) {}

  Matrix &operator=(Matrix &&mat) {
    matrix_ = std::move(mat.matrix_);
    return *this;
  }

  Matrix &Swap(Matrix &mat) {
    std::swap(matrix_, mat.matrix_);
    return *this;
  }

  Matrix &operator=(const Matrix &other) {
    if (this != &other) {
      Matrix temp(other);
      Swap(temp);
    }
    return *this;
  }

  bool IsAvaliableToAdd(const Matrix &other) const {
    return Size() == other.Size();
  }

  std::pair<size_t, size_t> Size() const {
    if (matrix_.size() != 0) {
      return {matrix_.size(), matrix_.at(0).size()};
    } else {
      return {0, 0};
    }
  }

  Matrix &operator+=(const Matrix &other) {
    if (IsAvaliableToAdd(other)) {
      for (size_t a = 0; a != other.matrix_.size(); ++a) {
        for (size_t b = 0; b != other.matrix_.at(0).size(); ++b) {
          matrix_[a][b] += other.matrix_[a][b];
        }
      }
    }

    return *this;
  }

  double &get(size_t n, size_t k) { return matrix_[n][k]; }
  const double &take(size_t n, size_t k) const { return matrix_.at(n).at(k); }

private:
  std::vector<std::vector<double>> matrix_;
};

std::vector<double> Parse(std::string_view str, char sep) {
  std::vector<double> d_vec;
  d_vec.reserve(str.size() / 2 + 1);

  while (str.size() != 0) {
    size_t end = str.find(sep);
    std::string value;
    for (size_t i = 0; i < std::min(end, str.size()); ++i) {
      if (isalnum(str[i])) {
        value += str[i];
      }
    }
    d_vec.push_back(stod(value));

    size_t count_del_nums = (end == std::string::npos) ? str.size() : end + 1;

    str.remove_prefix(count_del_nums);
  }
  return d_vec;
}

Matrix operator+(Matrix mat1, Matrix mat2) { return mat1 += mat2; }

std::istream &operator>>(std::istream &is, Matrix &mat) {

  for (size_t a = 0; a != mat.Size().first; ++a) {
    std::string line;
    std::getline(is, line);

    if (!is.good()) {
      throw std::logic_error("для матрицы не хватает строчек");
    }

    auto parsed_line = Parse(line, ' ');

    if (parsed_line.size() != mat.Size().second) {
      throw std::logic_error(
          "количество введенных в строчке элементов не равно размеру матрицы");
    }

    for (size_t b = 0; b != mat.Size().second; ++b) {
      mat.get(a, b) = std::move(parsed_line[b]);
    }
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const Matrix &mat) {
  for (size_t a = 0; a != mat.Size().first; ++a) {
    for (size_t b = 0; b != mat.Size().second; ++b) {
      os << mat.take(a, b) << ' ';
    }
    os << '\n';
  }
  return os;
}

void F4() {
  std::cout << "Напишите размер матриц и ниже, рязделяя числа пробелами сами "
               "матрицы:\n";
  size_t N;
  std::cin >> N;
  std::cin.ignore();

  if (N < 2) {
    std::cout << "Минимальный размер матрицы - 2";
    return;
  }

  Matrix mat1(N, N), mat2(N, N);

  std::cin >> mat1 >> mat2;

  std::cout << '\n' << mat1 + mat2;
}

void F5() {
  std::string str;
  std::wcout << L"Введите строчку:\n";
  std::getline(std::cin, str);
  for (auto it = str.begin(); it != str.end(); ++it) {
    if (*it == ' ') {
      str.erase(it);
    }
  }

  auto it1 = str.begin();
  auto it2 = std::prev(str.end());
  for (; it1 < it2; ++it1, --it2) {
    if (tolower(*it1) != tolower(*it2)) {
      std::cout << "Нет";
      return;
    }
  }
  std::wcout << L"Да";
}
int main() {
  setlocale(LC_ALL, "ru-RU");
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  F5();
}