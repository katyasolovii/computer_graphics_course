import pytest
import numpy as np
from src.math.Mat3x3 import Mat3x3
from src.math.Vec3 import Vec3
from src.math.Rotations import rotation_matrix_x, rotation_matrix_y, rotation_matrix_z
from src.math.Scale import scale_matrix
from src.math.Translation import translation_matrix2d


# 1. Тест додавання двох матриць
def test_matrix_addition():
    mat1 = Mat3x3(1, 2, 3, 4, 5, 6, 7, 8, 9)
    mat2 = Mat3x3(9, 8, 7, 6, 5, 4, 3, 2, 1)
    result = mat1 + mat2
    expected = np.array(mat1.data + mat2.data)

    assert np.allclose(result.data, expected), f"Помилка в додаванні: очікував {expected}, отримав {result.data}"


# 2. Тест множення матриці на іншу матрицю
def test_matrix_multiplication():
    mat1 = Mat3x3(1, 2, 3, 4, 5, 6, 7, 8, 9)
    mat2 = Mat3x3(9, 8, 7, 6, 5, 4, 3, 2, 1)
    result = mat1 @ mat2
    expected = np.dot(mat1.data, mat2.data)

    assert np.allclose(result.data, expected), f"Помилка у множенні матриць: очікував {expected}, отримав {result.data}"


# 3. Тест множення матриці на вектор
def test_matrix_vector_multiplication():
    mat = Mat3x3.identity()
    vec = Vec3(3, 4, 5)
    result = mat @ vec
    expected = vec.data  # Оскільки це одинична матриця, вектор не змінюється

    assert np.allclose(result.data,
                       expected), f"Помилка: результат множення має бути {expected}, отримано {result.data}"


# 4. Тест знаходження оберненої матриці
def test_matrix_inverse():
    mat = Mat3x3(2, -1, 0, -1, 2, -1, 0, -1, 2)
    inverse_mat = mat.inverse()
    expected = np.linalg.inv(mat.data)

    assert np.allclose(inverse_mat.data,
                       expected), f"Помилка: обернена матриця має бути {expected}, отримано {inverse_mat.data}"


# 5. Тест помилки при знаходженні оберненої особливої (сингулярної) матриці
def test_singular_matrix_inverse():
    singular_mat = Mat3x3(1, 2, 3, 4, 5, 6, 7, 8, 9)  # Визначник = 0
    # singular_mat = Mat3x3(1, 1, 1, 4, 4, 4, 7, 8, 9)  # Визначник = 0

    with pytest.raises(ValueError, match=Mat3x3.ERROR_MESSAGE_INV_DOESNT_EXIST):
        singular_mat.inverse()


# 6. Тест одиничної матриці
def test_matrix_identity():
    identity_mat = Mat3x3.identity()
    expected = np.eye(3, dtype=float)

    assert np.allclose(identity_mat.data, expected), "Помилка: одинична матриця має бути стандартною"

# 6.1. Тест одиничної матриці
def test_matrix_identity2():
    identity_mat = Mat3x3()
    expected = np.eye(3, dtype=float)

    assert np.allclose(identity_mat.data, expected), "Помилка: одинична матриця має бути стандартною"


# 7. Тест створення матриці обертання навколо X
def test_rotation_x():
    mat = Mat3x3.rotation_x(90, is_radians=False)
    expected = rotation_matrix_x(np.radians(90))

    assert np.allclose(mat.data,
                       expected), f"Помилка: матриця обертання навколо X має бути {expected}, отримано {mat.data}"


# 8. Тест створення матриці обертання навколо Y
def test_rotation_y():
    mat = Mat3x3.rotation_y(90, is_radians=False)
    expected = rotation_matrix_y(np.radians(90))

    assert np.allclose(mat.data,
                       expected), f"Помилка: матриця обертання навколо Y має бути {expected}, отримано {mat.data}"


# 9. Тест створення матриці обертання навколо Z
def test_rotation_z():
    mat = Mat3x3.rotation_z(90, is_radians=False)
    expected = rotation_matrix_z(np.radians(90))

    assert np.allclose(mat.data,
                       expected), f"Помилка: матриця обертання навколо Z має бути {expected}, отримано {mat.data}"


# 10. Тест створення матриці трансляції
def test_matrix_translation():
    mat = Mat3x3.translation(3, 4)
    expected = translation_matrix2d(3, 4)

    assert np.allclose(mat.data, expected), f"Помилка: матриця переносу має бути {expected}, отримано {mat.data}"


# 11. Тест створення матриці масштабування з двома параметрами
def test_matrix_scaling_two_values():
    mat = Mat3x3.scale(2, 3)
    expected = scale_matrix(2, 3)

    assert np.allclose(mat.data, expected), f"Помилка: матриця масштабування має бути {expected}, отримано {mat.data}"


# 12. Тест створення матриці масштабування з одним параметром (універсальний масштаб)
def test_matrix_scaling_one_value():
    mat = Mat3x3.scale(2)
    expected = scale_matrix(2, 2, 2)

    assert np.allclose(mat.data, expected), f"Помилка: матриця масштабування має бути {expected}, отримано {mat.data}"


# 13. Тест створення матриці масштабування з вектором
def test_matrix_scaling_with_vector():
    vec = Vec3(2, 3, 1)
    mat = Mat3x3.scale(vec)
    expected = scale_matrix(2, 3, 1)

    assert np.allclose(mat.data, expected), f"Помилка: матриця масштабування має бути {expected}, отримано {mat.data}"


# 14. Тест помилки при некоректному масштабуванні
def test_invalid_matrix_scaling():
    with pytest.raises(ValueError, match=Mat3x3.ERROR_MESSAGE_SCALE):
        Mat3x3.scale("invalid")


# 15. Тест множення на масив numpy (дозволено)
def test_matrix_numpy_multiplication():
    mat = Mat3x3(1, 2, 3, 4, 5, 6, 7, 8, 9)
    array = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]])
    result = mat @ array
    expected = np.dot(mat.data, array)

    assert np.allclose(result.data,
                       expected), f"Помилка: результат множення має бути {expected}, отримано {result.data}"
