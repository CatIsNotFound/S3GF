# 快速上手 Matrix2D

本文总结了 `MyEngine::Matrix2D` 类基本的用法及其示例。以更好的帮助开发者快速上手 `Matrix2D` 类。

## 类定义

`MyEngine::Matrix2D` 类是一个二维矩阵类，支持基本数据类型和简单结构体的存储。其基本功能包括：

- 构造函数：用于创建指定大小的矩阵。
- 析构函数：用于释放矩阵占用的内存。
- 访问元素：通过行号和列号访问矩阵中的元素。
- 遍历元素：使用迭代器或范围 (based for) 循环遍历矩阵中的所有元素。
- 矩阵运算：支持矩阵的加法、减法、乘法等基本运算。
- 矩阵变换：支持矩阵的转置、旋转等变换操作。
- 矩阵切片：支持将矩阵切分成子矩阵。
- 矩阵拼接：支持将多个矩阵拼接成一个矩阵。

基本定义：

```cpp
template<typename T>
class Matrix2D {
private:
    std::vector<T> _datas;  // 内部数据存储
    uint32_t _row;         // 行数
    uint32_t _col;         // 列数
    std::function<void(T &)> _deleter;  // 自定义删除器（用于指针类型）
public:
    // 位置结构体，用于表示矩阵中的位置
    struct Position {
        uint32_t row;
        uint32_t col;
        // ... 操作符重载 ...
    };
    
    // ... 方法定义 ...
};
```

## 矩阵操作

### 1. 创建矩阵

要创建一个指定大小的矩阵，可以使用构造函数。例如，要创建一个 `3x4` 的矩阵，可以使用以下代码：

```cpp
Matrix2D<int> matrix(3, 4);
```

如果还需要设定默认值，例如：所有元素都为 0，则可以在构造函数中添加一个额外的参数。例如，要创建一个 `3x4` 的矩阵，所有元素都为 0，则可以使用以下代码：

```cpp
Matrix2D<int> matrix(3, 4, 0);
```

对于指针类型的元素，还可以指定一个自定义的删除器。例如，要创建一个 `3x4` 的矩阵，所有元素都为 `nullptr`，并使用 `delete` 作为删除器，则可以使用以下代码：

```cpp
Matrix2D<int *> matrix(3, 4, nullptr, [](int *p) { delete p; });
```

> ⚠ 注意：如果不指定删除器，**请确保使用的指针不是由自己或 `Matrix2D` 类创建的**，否则可能会导致内存泄漏或触发未定义行为。

### 2. 访问元素

除了可以使用基本的 `index` 索引（例如：`matrix[i]`） 访问矩阵中的第 `i` 个元素以外，还可通过行号和列号可以访问矩阵中的元素。例如，要访问第 `i` 行第 `j` 列的元素，可以使用 `matrix(i, j)` 或 `matrix.at(i, j)`。

> ⚠ 注意：行号和列号从 0 开始计数；访问元素时，确保行号和列号在有效范围内，否则会抛出 `MyEngine::OutOfRangeException` 异常。

### 3. 遍历元素

可以使用迭代器或范围 (based for) 循环遍历矩阵中的所有元素。

```cpp
for (auto it = matrix.begin(); it != matrix.end(); ++it) {
    // Do something with *it.
}

// Used range-based for loop.
for (auto& i : matrix) {
    // Do something with i.
}

for (size_t i = 0; i < matrix.row(); ++i) {
    for (size_t j = 0; j < matrix.col(); ++j) {
        // Do something with matrix(i, j).
    }
}
```

## 矩阵计算

### 使用运算符计算

你可以使用 `+`, `-`, `*` 等运算符进行矩阵计算。

以下是一个简单的示例：假设有一个 `3x2` 的矩阵（如下定义）以及一个 `2x4` 的矩阵（如下定义）。

**Matrix1 (3x2):**

|   |   |
|---|---|
| 1 | 2 | 
| 3 | 4 | 
| 5 | 6 | 

**Matrix2 (2x4):**

|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | 4 |
| 5 | 6 | 7 | 8 |

```cpp
Matrix2D<int> mat1(3, 2);
for (int i = 0; i < mat1.size(); ++i) {
    mat1[i] = i + 1;
}
Matrix2D<int> mat2(2, 4);
for (int i = 0; i < mat2.size(); ++i) {
    mat2[i] = i + 1;
}
```

你可以使用 `+`, `-`, `*` 等运算符进行矩阵计算。

示例 1：计算 `mat1` 与 `mat2` 相加后的结果，可以使用以下代码：

```cpp
auto mat3 = mat1 + mat2;
```

最终得到的 `mat3` 会是如下：

**Matrix3:**

|   |   |
|---|---|
|  2|  4|
|  6|  8|
| 10| 12|


示例 2：要计算 `mat1` 与 `mat2` 的乘积（即：使用矩阵叉乘），可以使用以下代码：

```cpp
auto mat3 = mat1 * mat2;
```

执行后，`mat3` 是一个 `3x4` 的矩阵。结果大致如下：

**Matrix 3:**

|   |   |   |   |
|---|---|---|---|
| 11| 14| 17| 20|
| 23| 30| 37| 44|
| 35| 46| 57| 68|





> ⚠ 注意：矩阵乘法要求第一个矩阵的列数等于第二个矩阵的行数。否则，会抛出 `MyEngine::InvalidArgumentException` 异常。
