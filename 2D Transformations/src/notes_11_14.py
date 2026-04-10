import numpy as np

def get_t(tx, ty):
    return np.array([[1, 0, tx], [0, 1, ty], [0, 0, 1]])

def get_s(sx, sy):
    return np.array([[sx, 0, 0], [0, sy, 0], [0, 0, 1]])

def get_r(deg):
    rad = np.radians(deg)
    c, s = np.cos(rad), np.sin(rad)
    return np.array([[c, -s, 0], [s, c, 0], [0, 0, 1]])

def pivot_wrap(matrix, px, py):
    """Трансформація навколо опорної точки: M = T(p) * M_base * T(-p)"""
    step1 = np.dot(get_t(px, py), matrix)
    return np.dot(step1, get_t(-px, -py))


square_pts = [np.array([0, 0, 1]), np.array([1, 0, 1]), np.array([1, 1, 1]), np.array([0, 1, 1])]
labels = ['A', 'B', 'C', 'D']

print("=== ЗАВДАННЯ 11 ===")
M11 = np.array([
    [2.934, -0.416, 2.000],
    [0.624, 1.956, 3.400],
    [0, 0, 1]
])
M11_inv = np.linalg.inv(M11)
pts11_global = [np.array([2, 3.4, 1]), np.array([4.9, 4, 1]), np.array([4.5, 6, 1]), np.array([1.6, 5.4, 1])]
print(f"Матриця зворотної трансформації M^-1:\n{np.round(M11_inv, 3)}")
print("\nКоординати прямокутника в локальній системі:")
for i, p in enumerate(pts11_global):
    local_p = np.dot(M11_inv, p) 
    print(f"  P{i+1}_local: ({local_p[0]:.3f}, {local_p[1]:.3f})")


print("\n" + "="*30 + "\n=== ЗАВДАННЯ 12 ===")
M12 = np.array([[0.866, 0.5, 4], [0.5, 0.866, 3], [0, 0, 1]])
check12 = M12[0,0]*M12[0,1] + M12[1,0]*M12[1,1]
print(f"Матриця M:\n{M12}")
print(f"Перевірка умови TRS: a11*a12 + a21*a22 = {check12:.3f}")
print("Пояснення: Розкласти на TRS неможливо, оскільки матриця містить скошення, що порушує умову для чистого TRS. Це видно з того, що a11*a12 + a21*a22 не дорівнює нулю.")


print("\n" + "="*30 + "\n=== ЗАВДАННЯ 13 ===")
M13 = np.array([[1.414, -2.121, 1], [1.414, 2.121, 1], [0, 0, 1]])
tx13, ty13 = M13[0, 2], M13[1, 2] # Переміщення 
sx13 = np.sqrt(M13[0, 0]**2 + M13[1, 0]**2) # Розтяг Ox 
sy13 = np.sqrt(M13[0, 1]**2 + M13[1, 1]**2) # Розтяг Oy 
phi13 = np.degrees(np.arctan2(M13[1, 0], M13[0, 0])) # Поворот
print(f"Компоненти:\n  Переміщення T: ({tx13}, {ty13})\n  Розтяг S: ({sx13:.2f}, {sy13:.2f})\n  Поворот R: {phi13:.1f}°")
print("\nФінальні координати вершин:")
for i, p in enumerate(square_pts):
    res = np.dot(M13, p)
    print(f"  {labels[i]}': ({res[0]:.3f}, {res[1]:.3f})")


print("\n" + "="*30 + "\n=== ЗАВДАННЯ 14 ===")
M14 = np.array([[1.732, -1, 5], [1, 1.732, -3], [0, 0, 1]])
px, py = 1, 1
sx14 = np.sqrt(M14[0, 0]**2 + M14[1, 0]**2)
sy14 = np.sqrt(M14[0, 1]**2 + M14[1, 1]**2)
phi14 = np.degrees(np.arctan2(M14[1, 0], M14[0, 0]))
RS = np.dot(get_r(phi14), get_s(sx14, sy14))
M_pivot = pivot_wrap(RS, px, py)
T_final_mat = np.dot(M14, np.linalg.inv(M_pivot))

print(f"Компоненти (відносно Pivot 1,1):")
print(f"  Розтяг S: ({sx14:.2f}, {sy14:.2f})\n  Поворот R: {phi14:.1f}°")
print(f"  Додаткове переміщення T: ({T_final_mat[0,2]:.2f}, {T_final_mat[1,2]:.2f})")
print(f"\nПеревірка (відновлена матриця):\n{np.round(np.dot(T_final_mat, M_pivot), 3)}")
print("Координати точок:")
for i, p in enumerate(square_pts):
    res = np.dot(M14, p)
    print(f"  {labels[i]}': ({res[0]:.3f}, {res[1]:.3f})")