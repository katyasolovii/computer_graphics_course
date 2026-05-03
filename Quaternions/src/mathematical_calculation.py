import math
import numpy as np
from src.math.Quaternion import Quaternion
from src.math.Vec3 import Vec3
from src.math.Vec4 import Vec4

def solve_task_0():
    print(f"\n{'='*20} ЗАВДАННЯ 0: ВІД ОСІ ТА КУТА ДО КВАТЕРНІОНА {'='*20}")
    
    theta_deg = 60
    u = (1/math.sqrt(3), 1/math.sqrt(3), 1/math.sqrt(3))
    
    print(f"Вихідні дані:")
    print(f"Кут theta: {theta_deg}°")
    print(f"Вектор осі u: {tuple(round(c, 4) for c in u)}")

    #  q = (w, x, y, z), q = cos(theta/2) + u * sin(theta/2)
    half_theta_rad = math.radians(theta_deg / 2)
    w = math.cos(half_theta_rad)
    sin_half = math.sin(half_theta_rad)
    
    x = u[0] * sin_half
    y = u[1] * sin_half
    z = u[2] * sin_half
    
    q = (w, x, y, z)
    print(f"\n1. Побудований кватерніон q (w, x, y, z):")
    print(f"{tuple(round(c, 4) for c in q)}")

    # |q| = sqrt(w^2 + x^2 + y^2 + z^2)
    norm = math.sqrt(w**2 + x**2 + y**2 + z**2)
    print(f"\n2. Норма кватерніона |q| = {round(norm, 4)}")

    q0, q1, q2, q3 = w, x, y, z

    r00 = 1 - 2*(q2**2 + q3**2)
    r11 = 1 - 2*(q1**2 + q3**2)
    r22 = 1 - 2*(q1**2 + q2**2)

    r01 = 2*(q1*q2 - q0*q3)
    r02 = 2*(q1*q3 + q0*q2)
    
    r10 = 2*(q1*q2 + q0*q3)
    r12 = 2*(q2*q3 - q0*q1)
    
    r20 = 2*(q1*q3 - q0*q2)
    r21 = 2*(q2*q3 + q0*q1)
    
    R = [
        [r00, r01, r02],
        [r10, r11, r12],
        [r20, r21, r22]
    ]

    print(f"\n3. Матриця повороту R:")
    for row in R:
        print([round(val, 4) for val in row])
    
    return q, R


def solve_task_1():
    print(f"\n{'='*20} ЗАВДАННЯ 1: ОПЕРАЦІЯ ПОВОРОТУ ВЕКТОРА {'='*20}")
    
    p = Vec3(1, 0, 0)
    theta = math.radians(90)
    q = Quaternion.rotation_z(theta)
    print(f"1. Точка p: {p}")
    print(f"   Кватерніон повороту q (Z, 90°): {q}")

    v = Quaternion(p)
    print(f"\n2. Чистий кватерніон v (w=0, x=p.x, y=p.y, z=p.z):")
    print(f"   v = {v}")

    # v' = q * v * q_inv
    q_inv = q.inverse()
    v_prime = q * v * q_inv
    print(f"\n3. Результат операції v' = q * v * q_inv:")
    print(f"   v' = {v_prime}")

    # Векторна частина - компоненти x, y, z кватерніона v'
    result_coords = (float(v_prime.x), float(v_prime.y), float(v_prime.z))
    
    expected = (0.0, 1.0, 0.0)
    print(f"\n4. Виділена векторна частина з v':")
    print(f"   Координати: {result_coords}")
    print(f"\n5. Перевірка:")
    print(f"   Отриманні координати: {tuple(round(c, 4) for c in result_coords)}")
    print(f"   Очікувані координати: {expected}")
    
    is_correct = np.allclose(result_coords, expected, atol=1e-7)
    print(f"   Результати збігаються: {is_correct}")


def solve_task_2():
    print(f"\n{'='*20} ЗАВДАННЯ 2: КОМПОЗИЦІЯ ОБЕРТАНЬ {'='*20}")
    
    vertices = [
        Vec3(0, 0, 0),
        Vec3(1, 0, 0),
        Vec3(0, 1, 0),
        Vec3(0, 0, 1)
    ]
    
    q1 = Quaternion.rotation_x(math.radians(45))
    q2 = Quaternion.rotation_y(math.radians(30))
    
    q_total = q2 * q1
    print(f"Результуючий кватерніон q_total: {q_total}")
    
    angle_rad, axis = q_total.to_angle_axis()
    print(f"\nПараметри total повороту:")
    print(f"Кут: {round(math.degrees(angle_rad), 2)}°")
    print(f"Вісь: ({round(axis.x, 4)}, {round(axis.y, 4)}, {round(axis.z, 4)})")
    
    print(f"\nНові координати вершин тетраедра:")
    new_vertices = []
    for i, v in enumerate(vertices):
        v_prime = q_total.rotate_vector(v)
        coords = (float(v_prime.x), float(v_prime.y), float(v_prime.z))
        new_vertices.append(coords)
        print(f"V{i} {tuple(float(c) for c in v)} -> {tuple(round(c, 4) for c in coords)}")
    
    return q_total, new_vertices


def solve_task_3():
    print(f"\n{'='*20} ЗАВДАННЯ 3: КОНВЕРТАЦІЯ ТА GIMBAL LOCK {'='*20}")
    
    alpha, beta, gamma = 20, 90, 50
    
    print(f"Кути: Yaw(Z)={alpha}°, Pitch(Y)={beta}°, Roll(X)={gamma}°")
    
    qz = Quaternion.rotation_z(math.radians(alpha))
    qy = Quaternion.rotation_y(math.radians(beta))
    qx = Quaternion.rotation_x(math.radians(gamma))
    
    print(f"\nКватерніони:")
    print(f"qz: {qz}")
    print(f"qy: {qy}")
    print(f"qx: {qx}")
    
    q_final = qz * qy * qx
    print(f"\nФінальний кватерніон q: {q_final}")
    
    angle_rad, axis = q_final.to_angle_axis()
    print(f"\nПараметри сумарного повороту:")
    print(f"Кут: {round(math.degrees(angle_rad), 2)}°")
    print(f"Вісь: ({round(float(axis.x), 4)}, {round(float(axis.y), 4)}, {round(float(axis.z), 4)})")
    
    print("Кватерніон q чітко описує фінальну орієнтацію.")
    print("Математично ми не отримали поділу на нуль або невизначеності,")
    print("що підтверджує стабільність кватерніонів у точці Gimbal Lock.")

    return q_final


def solve_task_4():
    print(f"\n{'='*20} ЗАВДАННЯ 4: ДЕКОМПОЗИЦІЯ МАТРИЦІ В КВАТЕРНІОН {'='*20}")
    
    R = [
        [0, -1, 0],
        [1,  0, 0],
        [0,  0, 1]
    ]
    
    print("Матриця R:")
    for row in R:
        print(row)

    r00, r01, r02 = R[0][0], R[0][1], R[0][2]
    r10, r11, r12 = R[1][0], R[1][1], R[1][2]
    r20, r21, r22 = R[2][0], R[2][1], R[2][2]

    # q0^2 = 1/4 * (1 + r00 + r11 + r22)
    q0_sq = 0.25 * (1 + r00 + r11 + r22)
    q0 = math.sqrt(q0_sq)

    # q1 = (r21 - r12) / 4q0
    # q2 = (r02 - r20) / 4q0
    # q3 = (r10 - r01) / 4q0
    divisor = 4 * q0
    q1 = (r21 - r12) / divisor
    q2 = (r02 - r20) / divisor
    q3 = (r10 - r01) / divisor

    q_final = Quaternion(q0, q1, q2, q3)
    
    print(f"\nРозраховані компоненти:")
    print(f"q0 (w) = {round(float(q0), 5)}")
    print(f"q1 (x) = {round(float(q1), 5)}")
    print(f"q2 (y) = {round(float(q2), 5)}")
    print(f"q3 (z) = {round(float(q3), 5)}")
    
    print(f"\nПідсумковий кватерніон q: {q_final}")
    
    angle_rad, axis = q_final.to_angle_axis()
    print(f"\nАналіз отриманого кватерніона:")
    print(f"Кут повороту: {round(math.degrees(angle_rad), 1)}°")
    print(f"Вісь повороту: ({round(float(axis.x), 1)}, {round(float(axis.y), 1)}, {round(float(axis.z), 1)})")

    return q_final


def solve_task_5():
    print(f"\n{'='*20} ЗАВДАННЯ 5: ПОВНА ДЕКОМПОЗИЦІЯ АФІННОЇ МАТРИЦІ {'='*20}")
    
    M = np.array([
        [0, -2,  0,   10],
        [1,  0,  0,   -5],
        [0,  0,  1.5,  3],
        [0,  0,  0,    1]
    ])
    
    print("Вхідна матриця M:")
    print(M)
    tx = M[0, 3]
    ty = M[1, 3]
    tz = M[2, 3]
    translation = (tx, ty, tz)
    print(f"\n1. Вектор перенесення T: {tuple(float(c) for c in translation)}")
    sub_m = M[:3, :3]
    sx = np.linalg.norm(sub_m[:, 0])
    sy = np.linalg.norm(sub_m[:, 1])
    sz = np.linalg.norm(sub_m[:, 2])
    scale = (sx, sy, sz)
    print(f"2. Масштабні коефіцієнти S: {tuple(float(c) for c in scale)}")

    r0 = sub_m[:, 0] / sx
    r1 = sub_m[:, 1] / sy
    r2 = sub_m[:, 2] / sz
    
    R = np.column_stack((r0, r1, r2))
    print("\n3. Чиста матриця обертання R (після нормалізації):")
    print(R)

    det = np.linalg.det(R)
    print(f"Детермінант R: {round(det, 4)} (якщо 1.0 - зсуву немає)")

    r00, r11, r22 = R[0, 0], R[1, 1], R[2, 2]
    trace = r00 + r11 + r22
    q0 = 0.5 * math.sqrt(max(0, 1 + trace))
    
    divisor = 4 * q0
    q1 = (R[2, 1] - R[1, 2]) / divisor
    q2 = (R[0, 2] - R[2, 0]) / divisor
    q3 = (R[1, 0] - R[0, 1]) / divisor

    q_final = Quaternion(q0, q1, q2, q3)
    
    print(f"\n4. Підсумковий одиничний кватерніон q: {q_final}")
    angle, axis = q_final.to_angle_axis()
    print(f"\nФізичний зміст трансформації:")
    print(f"Об'єкт переміщено в {float(translation[0])}, {float(translation[1])}, {float(translation[2])}")
    print(f"масштабовано в {float(scale[0])}, {float(scale[1])}, {float(scale[2])} рази,")
    print(f"та повернуто на {round(math.degrees(angle), 1)}° навколо осі {axis.xyz}")

    return translation, scale, q_final

if __name__ == "__main__":
    solve_task_0()
    solve_task_1()
    solve_task_2()
    solve_task_3()
    solve_task_4()
    solve_task_5()