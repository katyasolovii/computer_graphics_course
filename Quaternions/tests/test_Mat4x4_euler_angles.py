import numpy as np
import pytest
from scipy.spatial.transform import Rotation as R

from src.math.Mat4x4 import Mat4x4
from src.math.utils_matrix import is_same_matrix


# ========================  ТЕСТИ ДЛЯ КУТІВ ОЙЛЕРА  ===========================

# 1. Тест обертання XYZ -> повернення до кутів Euler
def test_euler_rotation_xyz():
    angles_orig = (30, 45, 60)  # Градуси
    angles = np.radians(angles_orig)
    mat = Mat4x4.rotation_euler(*angles, configuration="xyz")
    recovered_angles = mat.toEuler("XYZ")
    assert np.allclose(np.degrees(recovered_angles), angles_orig, atol=1e-5), "Помилка у відновленні кутів Euler (XYZ)"


# 2. Тест обертання ZXZ -> повернення до кутів Euler
def test_euler_rotation_zxz():
    angles_orig = (90, 45, 30)
    angles = np.radians(angles_orig)
    mat = Mat4x4.rotation_euler(*angles, configuration="zxz")
    recovered_angles = mat.toEuler("ZXZ")
    assert np.allclose(np.degrees(recovered_angles), angles_orig, atol=1e-5), "Помилка у відновленні кутів Euler (ZXZ)"


# 3. Тест, що обертання на (0,0,0) дає одиничну матрицю
def test_euler_zero_rotation():
    mat = Mat4x4.rotation_euler(0, 0, 0, configuration="xyz")
    expected = Mat4x4.identity()
    assert np.allclose(mat.data, expected.data), "Помилка: очікувалась одинична матриця при обертанні (0,0,0)"


# 4. Тест, що `toEuler()` правильно обробляє одиничну матрицю
def test_toEuler_from_identity():
    mat = Mat4x4.identity()
    angles = mat.toEuler("XYZ")
    expected = (0.0, 0.0, 0.0)
    assert np.allclose(angles, expected), "Помилка у отриманні кутів Euler з одиничної матриці"


# 5. Тест, що `toEuler()` працює після множення обертань
def test_toEuler_after_multiple_rotations():
    phi, theta, psi = np.radians((30, 45, 60))
    mat = (Mat4x4.rotation_euler(phi, 0, 0, "xyz")
           @ Mat4x4.rotation_euler(0, theta, 0, "xyz")
           @ Mat4x4.rotation_euler(0, 0, psi, "xyz"))
    recovered_angles = mat.toEuler("XYZ")
    assert np.allclose(np.degrees(recovered_angles), (30, 45, 60), atol=1e-5), "Помилка у оберненні обертання XYZ"


# 6. Тест некоректного порядку обертання у `rotation_euler()`
def test_invalid_euler_rotation():
    with pytest.raises(ValueError, match="Unknown Euler configuration"):
        Mat4x4.rotation_euler(30, 45, 60, configuration="abc")


# 7. Тест некоректного порядку обертання у `toEuler()`
def test_invalid_toEuler():
    mat = Mat4x4.identity()
    with pytest.raises(ValueError, match="Unknown Euler configuration"):
        mat.toEuler("ABC")


# 8. Тест обернення `rotation_euler()` та `toEuler()`
def test_euler_rotation_and_reversal():
    angles_orig = (10, 20, 30)
    angles = np.radians(angles_orig)
    mat = Mat4x4.rotation_euler(*angles, configuration="xyz")
    recovered_angles = mat.toEuler("XYZ")
    assert np.allclose(np.degrees(recovered_angles), angles_orig,
                       atol=1e-5), "Помилка у відновленні кутів Euler після обертання"


# 9. Тест для обертань на 180 градусів (можливість двозначності)
def test_euler_180_degree_rotation():
    mat = Mat4x4.rotation_euler(np.radians(180), 0, 0, "xyz")
    recovered_angles = mat.toEuler("XYZ")
    assert np.isclose(np.degrees(recovered_angles[0]), 180, atol=1e-5), "Помилка у відновленні кута 180 градусів"


# 10. Тест для обертання в декількох напрямках і відновлення кутів
def test_euler_complex_rotation():
    phi, theta, psi = np.radians(30), np.radians(45), np.radians(60)
    phi1, theta1, psi1 = np.radians(-30), np.radians(-45), np.radians(-60)
    mat_to = Mat4x4.rotation_euler(phi, theta, psi, "xyz")
    mat_back = Mat4x4.rotation_z(psi1) * Mat4x4.rotation_y(theta1) * Mat4x4.rotation_x(phi1)
    mat = mat_to * mat_back

    recovered_angles = mat.toEuler("XYZ")
    expected = (0, 0, 0)
    assert np.allclose(np.degrees(recovered_angles), expected,
                       atol=1e-5), "Помилка: очікувалось повернення до (0,0,0) після зворотних обертань"


# ========================  ТЕСТИ ДЛЯ КУТІВ ОЙЛЕРА З ЯВНИМИ МАТРИЦЯМИ ===========================

# 1. Тест обертання на 90 градусів навколо X (XYZ)
def test_explicit_rotation_x():
    mat = Mat4x4.rotation_euler(np.radians(90), 0, 0, configuration="xyz")
    expected = np.array([[1, 0, 0, 0],
                         [0, 0, -1, 0],
                         [0, 1, 0, 0],
                         [0, 0, 0, 1]], dtype=float)
    assert np.allclose(mat.data, expected, atol=1e-5), "Помилка у матриці обертання на 90° навколо X"


# 2. Тест обертання на 90 градусів навколо Y (XYZ)
def test_explicit_rotation_y():
    mat = Mat4x4.rotation_euler(0, np.radians(90), 0, configuration="xyz")
    expected = np.array([[0, 0, 1, 0],
                         [0, 1, 0, 0],
                         [-1, 0, 0, 0],
                         [0, 0, 0, 1]], dtype=float)
    assert np.allclose(mat.data, expected, atol=1e-5), "Помилка у матриці обертання на 90° навколо Y"


# 3. Тест обертання на 90 градусів навколо Z (XYZ)
def test_explicit_rotation_z():
    mat = Mat4x4.rotation_euler(0, 0, np.radians(90), configuration="xyz")
    expected = np.array([[0, -1, 0, 0],
                         [1, 0, 0, 0],
                         [0, 0, 1, 0],
                         [0, 0, 0, 1]], dtype=float)
    assert np.allclose(mat.data, expected, atol=1e-5), "Помилка у матриці обертання на 90° навколо Z"


# 4. Тест обертання на 45 градусів навколо XYZ (комбіноване обертання)
def test_explicit_rotation_xyz():
    phi, theta, psi = np.radians(45), np.radians(45), np.radians(45),
    mat = Mat4x4.rotation_euler(phi, theta, psi, configuration="xyz")
    expected = np.array([[0.5, -0.5, 0.70710678, 0],
                         [0.85355339, 0.14644661, -0.5, 0],
                         [0.14644661, 0.85355339, 0.5, 0],
                         [0, 0, 0, 1]], dtype=float)
    assert np.allclose(mat.data, expected, atol=1e-5), "Помилка у матриці обертання (45°, 45°, 45°)"


# 4. Тест обертання на 45 градусів навколо XYZ (комбіноване обертання)
def test_explicit_rotation_xyz2():
    phi, theta, psi = np.radians((30, 45, 60))
    mat = Mat4x4.rotation_euler(phi, theta, psi, configuration="xyz")
    expected = Mat4x4([
        [0.35355339, -0.61237244, 0.70710678, 0],
        [0.9267767, 0.12682648, -0.35355339, 0],
        [0.12682648, 0.78033009, 0.61237244, 0],
        [0, 0, 0, 1],
    ])

    assert is_same_matrix(mat.data, expected), "Помилка у матриці обертання (30°, 45°, 60°)"


# 5. Тест обертання на 30 градусів у конфігурації ZXZ
def test_explicit_rotation_zxz():
    phi, theta, psi = np.radians((30, 45, 60))
    mat = Mat4x4.rotation_euler(phi, theta, psi, configuration="ZXZ")

    # Виправлена очікувана матриця для ZXZ
    expected = np.array(
        [[0.12682648, -0.9267767, 0.35355339, 0],
         [0.78033009, -0.12682648, -0.61237244, 0],
         [0.61237244, 0.35355339, 0.70710678, 0],
         [0, 0, 0, 1]]
        , dtype=float)

    assert np.allclose(mat.data, expected, atol=1e-5), "Помилка у матриці обертання ZXZ (30°, 45°, 60°)"
