import pytest
import numpy as np
from src.math.Vec3 import Vec3
from src.math.Vec4 import Vec4, vertex

# 1. Тест створення нульового вектора
def test_default_vector():
    vec = Vec4()
    expected = np.zeros(4, dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути нульовий вектор"

# 2. Тест створення з 3 чисел (остання координата має бути 0)
def test_vector_from_three_numbers():
    vec = Vec4(1, 2, 3)
    expected = np.array([1, 2, 3, 0], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 3, 0)"

# 3. Тест створення з 4 чисел
def test_vector_from_four_numbers():
    vec = Vec4(1, 2, 3, 4)
    expected = np.array([1, 2, 3, 4], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 3, 4)"

# 4. Тест створення з `Vec3` (має автоматично додати `0.0`)
def test_vector_from_vec3():
    v3 = Vec3(1, 2, 3)
    vec = Vec4(v3)
    expected = np.array([1, 2, 3, 0], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 3, 0) зі створення через Vec3"

# 5. Тест створення з `Vec4` (має бути копія)
def test_vector_from_vec4():
    original = Vec4(4, 5, 6, 7)
    copy = Vec4(original)
    assert np.allclose(copy.data, original.data), "Копія вектора має бути ідентичною оригіналу"

# 6. Тест створення через `vertex()`
def test_vector_vertex():
    vec = vertex(3, 4, 5)
    expected = np.array([3, 4, 5, 1], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: vertex() має створити вектор (3, 4, 5, 1)"

# 7. Тест додавання двох `Vec4`
def test_vector_addition():
    vec1 = Vec4(1, 2, 3, 4)
    vec2 = Vec4(4, 3, 2, 1)
    result = vec1 + vec2
    expected = np.array([5, 5, 5, 5], dtype=float)
    assert np.allclose(result.data, expected), "Помилка: додавання має дати (5, 5, 5, 5)"

# 8. Тест віднімання двох `Vec4`
def test_vector_subtraction():
    vec1 = Vec4(1, 2, 3, 4)
    vec2 = Vec4(4, 3, 2, 1)
    result = vec1 - vec2
    expected = np.array([-3, -1, 1, 3], dtype=float)
    assert np.allclose(result.data, expected), "Помилка: віднімання має дати (-3, -1, 1, 3)"

# 9. Тест множення на скаляр
def test_vector_scalar_multiplication():
    vec = Vec4(1, 2, 3, 4)
    result = vec * 2
    expected = np.array([2, 4, 6, 8], dtype=float)
    assert np.allclose(result.data, expected), "Помилка: множення на 2 має дати (2, 4, 6, 8)"

# 10. Тест скалярного добутку двох векторів
def test_vector_dot_product():
    vec1 = Vec4(1, 2, 3, 4)
    vec2 = Vec4(4, 3, 2, 1)
    result = vec1 * vec2
    expected = 1*4 + 2*3 + 3*2 + 4*1  # = 20
    assert np.isclose(result, expected), f"Помилка: очікуваний скалярний добуток {expected}, отримано {result}"

# 11. Тест нормування вектора
def test_vector_normalization():
    vec = Vec4(3, 4, 0, 0)
    vec.normalize()
    expected_length = 1.0
    assert np.isclose(vec.norm(), expected_length), "Помилка: нормалізований вектор має мати довжину 1"

# 12. Тест повернення нового нормалізованого вектора (метод normalized)
def test_vector_normalized():
    vec = Vec4(3, 4, 0, 0)
    normalized_vec = vec.normalized()
    expected_length = 1.0
    assert np.isclose(normalized_vec.norm(), expected_length), "Помилка: нормалізований вектор має мати довжину 1"
    assert not np.allclose(vec.data, normalized_vec.data), "Оригінальний вектор не має змінюватися"

# 13. Тест від’ємного вектора
def test_vector_negation():
    vec = Vec4(1, -2, 3, -4)
    result = -vec
    expected = np.array([-1, 2, -3, 4], dtype=float)
    assert np.allclose(result.data, expected), "Помилка: від’ємний вектор має бути (-1, 2, -3, 4)"

# 14. Тест доступу та встановлення x, y, z, w
def test_vector_properties():
    vec = Vec4(1, 2, 3, 4)
    vec.x = 10
    vec.y = 20
    vec.z = 30
    vec.w = 40
    expected = np.array([10, 20, 30, 40], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: встановлені значення x, y, z, w мають бути (10, 20, 30, 40)"

# 15. Тест доступу до підвекторів xy, xz, yz, xyz, xyzw
def test_vector_subvectors():
    vec = Vec4(1, 2, 3, 4)
    assert np.allclose(vec.xy, [1, 2]), "Помилка: xy має бути (1, 2)"
    assert np.allclose(vec.xz, [1, 3]), "Помилка: xz має бути (1, 3)"
    assert np.allclose(vec.yz, [2, 3]), "Помилка: yz має бути (2, 3)"
    assert np.allclose(vec.xyz, [1, 2, 3]), "Помилка: xyz має бути (1, 2, 3)"
    assert np.allclose(vec.xyzw, [1, 2, 3, 4]), "Помилка: xyzw має бути (1, 2, 3, 4)"
