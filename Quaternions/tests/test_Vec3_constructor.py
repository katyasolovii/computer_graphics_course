import pytest
import numpy as np
from src.math.Vec3 import Vec3  # Переконайся, що шлях правильний!

# 1. Створення нульового вектора
def test_default_vector():
    vec = Vec3()
    expected = np.zeros(3, dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути нульовий вектор"

# 2. Створення з 3 чисел
def test_vector_from_three_numbers():
    vec = Vec3(1, 2, 3)
    expected = np.array([1, 2, 3], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 3)"

# 2.1. Створення з 2 чисел
def test_vector_from_two_numbers():
    vec = Vec3(1, 2)
    expected = np.array([1, 2, 0], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 0)"

# 3. Створення з 3-елементного списку
def test_vector_from_list():
    vec = Vec3([1, 2, 3])
    expected = np.array([1, 2, 3], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (1, 2, 3) зі списку"

# 4. Створення з 3-елементного `numpy.ndarray`
def test_vector_from_numpy():
    data = np.array([1, 2, 3], dtype=float)
    vec = Vec3(data)
    assert np.allclose(vec.data, data), "Помилка: має бути вектор (1, 2, 3) з numpy"

# 5. Створення копії іншого `Vec3`
def test_vector_from_existing_vec3():
    original = Vec3(4, 5, 6)
    copy = Vec3(original)
    assert np.allclose(copy.data, original.data), "Копія вектора має бути ідентичною оригіналу"

# 6. Створення з `tuple`
def test_vector_from_tuple():
    vec = Vec3((7, 8, 9))
    expected = np.array([7, 8, 9], dtype=float)
    assert np.allclose(vec.data, expected), "Помилка: має бути вектор (7, 8, 9) з tuple"

# 7. Некоректний розмір (наприклад, список із 2 елементів) -> очікується ValueError
def test_vector_invalid_size():
    with pytest.raises(ValueError, match=Vec3.ERROR_MESSAGE_CONSTRUCTOR):
        Vec3([1, 2])  # Неправильний розмір

# 8. Некоректний тип (наприклад, рядок) -> очікується TypeError
def test_vector_invalid_type():
    with pytest.raises(ValueError, match=Vec3.ERROR_MESSAGE_CONSTRUCTOR):
        Vec3("invalid input")  # Неправильний тип

# 9. Створення з numpy-масиву неправильного розміру -> очікується ValueError
def test_vector_invalid_numpy_size():
    with pytest.raises(ValueError, match=Vec3.ERROR_MESSAGE_CONSTRUCTOR):
        Vec3(np.array([1, 2, 3, 4]))  # Забагато елементів

# 10. Створення з іншого типу, який не є `Vec3`, `list`, `tuple` або `numpy.ndarray` -> очікується TypeError
def test_vector_invalid_object():
    class Dummy:
        pass

    with pytest.raises(ValueError, match=Vec3.ERROR_MESSAGE_CONSTRUCTOR):
        Vec3(Dummy())  # Некоректний тип
