import numpy as np
import pytest

from src.math.Mat4x4 import Mat4x4


# 12. Тест помилки при створенні з рядка
def test_invalid_matrix_from_string():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4("invalid")


# 13. Тест помилки при створенні з масиву меншого за 2×2
def test_invalid_matrix_from_small_list():
    with pytest.raises(ValueError,
                       match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4([1, 2, 3])  # Очікується помилка


# 14. Тест помилки при створенні з неправильного списку списків
def test_invalid_matrix_from_irregular_list():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4([[1, 2], [3]])  # Некоректний вкладений список


# 15. Тест помилки при створенні з 1D `numpy.ndarray`
def test_invalid_matrix_from_1D_numpy():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4(np.array([1, 2, 3]))  # Очікується помилка


# 16. Тест помилки при створенні з `numpy.ndarray`, який не 2D (наприклад, 3×2)
def test_invalid_matrix_from_wrong_numpy_shape():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4(np.array([[1, 2], [3, 4], [5, 6]]))  # Некоректний розмір (3×2)


# 17. Тест помилки при передачі `None`
def test_invalid_matrix_from_none():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4(None)  # Передаємо None


# 18. Тест помилки при створенні з `set` (множини)
def test_invalid_matrix_from_set():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4({1, 2, 3, 4})  # Непідтриманий тип


# 19. Тест помилки при створенні з `dict` (словника)
def test_invalid_matrix_from_dict():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4({"a": 1, "b": 2})  # Непідтриманий тип


# 20. Тест помилки при створенні з `bool` (логічного типу)
def test_invalid_matrix_from_bool():
    with pytest.raises(ValueError, match=Mat4x4.ERROR_MESSAGE_CONSTRUCTOR):
        Mat4x4(True)  # Непідтриманий тип
