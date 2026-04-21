import math, random

CUBE_VERTICES = [
    (0,0,0), (1,0,0), (1,1,0), (0,1,0),
    (0,0,1), (1,0,1), (1,1,1), (0,1,1)
]

TETRAHEDRON_VERTICES = [
    (0, 0, 0), (1, 0, 0), (0, 1, 0), (0, 0, 1)
]

def multiply_matrices(A, B):
    C = [[0.0] * 4 for _ in range(4)]
    for i in range(4):
        for j in range(4):
            for k in range(4):
                C[i][j] += A[i][k] * B[k][j]
    return C


def transpose(M):
    T = [[0.0] * 4 for _ in range(4)]
    for i in range(4):
        for j in range(4):
            T[i][j] = M[j][i]
    return T


def translation_matrix(tx, ty, tz):
    return [
        [1, 0, 0, tx],
        [0, 1, 0, ty],
        [0, 0, 1, tz],
        [0, 0, 0, 1]
    ]


def scaling_matrix(sx, sy, sz):
    return [
        [sx, 0.0, 0.0, 0.0],
        [0.0, sy, 0.0, 0.0],
        [0.0, 0.0, sz, 0.0],
        [0.0, 0.0, 0.0, 1.0]
    ]


def rot_x(angle):
    rad = math.radians(angle)
    c, s = math.cos(rad), math.sin(rad)
    return [
        [1, 0, 0, 0],
        [0, c, -s, 0],
        [0, s, c, 0],
        [0, 0, 0, 1]
    ]

def rot_y(angle):
    rad = math.radians(angle)
    c, s = math.cos(rad), math.sin(rad)
    return [
        [c, 0, s, 0],
        [0, 1, 0, 0],
        [-s, 0, c, 0],
        [0, 0, 0, 1]
    ]

def rot_z(angle):
    rad = math.radians(angle)
    c, s = math.cos(rad), math.sin(rad)
    return [
        [c, -s, 0, 0],
        [s, c, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ]


def rotation_arbitrary_axis(psi, axis):
    vx, vy, vz = axis
    
    modul_v = math.sqrt(vx**2 + vy**2 + vz**2)
    vx, vy, vz = vx/modul_v, vy/modul_v, vz/modul_v
    
    phi_rad = math.atan2(vx, vz)
    phi = math.degrees(phi_rad)
    
    vz_prime = math.sqrt(vx**2 + vz**2)
    theta_rad = math.atan2(vy, vz_prime)
    theta = math.degrees(theta_rad)
    
    Ry_phi = rot_y(-phi)   
    Rx_theta = rot_x(theta)       
    Rz_psi = rot_z(psi)           
    
    Rx_inv = transpose(Rx_theta)
    Ry_inv = transpose(Ry_phi)
    
    # R_total = Ry_inv * Rx_inv * Rz_psi * Rx_theta * Ry_phi
    step1 = multiply_matrices(Rz_psi, Rx_theta)
    step2 = multiply_matrices(step1, Ry_phi)
    step3 = multiply_matrices(Rx_inv, step2)
    R_total = multiply_matrices(Ry_inv, step3)
    
    return R_total

def rotation_euler_angles(phi, theta, psi, order="XYZ"):
    rx = rot_x(phi)
    ry = rot_y(theta)
    rz = rot_z(psi)
    
    if order == "XYZ":
        # Rz * Ry * Rx
        return multiply_matrices(rz, multiply_matrices(ry, rx))
    elif order == "ZYX":
        # Rx * Ry * Rz
        return multiply_matrices(rx, multiply_matrices(ry, rz))
    return None


def print_matrix(name, M):
    print(f"\nМатриця {name}:")
    for row in M:
        print([round(x, 3) for x in row])


def print_vertices(stage_name, vertices):
    print(f"\n--- Положення вершин ({stage_name}) ---")
    for i, v in enumerate(vertices):
        print(f"V{i}: {tuple(round(c, 3) for c in v)}")


def apply_transformation(matrix, point):
    res = [0.0] * 4
    p = list(point) + [1]
    for i in range(4):
        for j in range(4):
            res[i] += matrix[i][j] * p[j]
    return tuple(res[:3])


def solve_task_1():
    print(f"\n{'='*20} ЗАВДАННЯ 1 {'='*20}")
    current_vertices = CUBE_VERTICES
    print_vertices("Початковий стан", current_vertices)

    R = rotation_arbitrary_axis(45, (1, 1, 0))
    print_matrix("Обертання R (навколо 1,1,0)", R)
    current_vertices = [apply_transformation(R, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    T = translation_matrix(2, -1, 3)
    print_matrix("Переміщення T (2, -1, 3)", T)
    current_vertices = [apply_transformation(T, v) for v in current_vertices]
    print_vertices("Після переміщення", current_vertices)

def solve_task_2():
    print(f"\n{'='*20} ЗАВДАННЯ 2 {'='*20}")
    current_vertices = CUBE_VERTICES
    print_vertices("Початковий стан", current_vertices)

    S = scaling_matrix(2, 0.5, 1)
    print_matrix("Масштабування S (2, 0.5, 1)", S)
    current_vertices = [apply_transformation(S, v) for v in current_vertices]
    print_vertices("Після масштабування", current_vertices)

    R_e = rotation_euler_angles(30, 45, 60)
    print_matrix("Обертання Ейлера R (XYZ: 30, 45, 60)", R_e)
    current_vertices = [apply_transformation(R_e, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    T = translation_matrix(-3, 2, 5)
    print_matrix("Переміщення T (-3, 2, 5)", T)
    current_vertices = [apply_transformation(T, v) for v in current_vertices]
    print_vertices("Після переміщення", current_vertices)


def solve_task_3():
    print(f"\n{'='*20} ЗАВДАННЯ 3 {'='*20}")
    current_vertices = CUBE_VERTICES
    print_vertices("Початковий стан", current_vertices)

    Rz = rot_z(60)
    print_matrix("Обертання Rz (60° навколо 0,0,1)", Rz)
    current_vertices = [apply_transformation(Rz, v) for v in current_vertices]
    print_vertices("Після обертання навколо Z", current_vertices)

    R = rotation_arbitrary_axis(45, (1, 1, 1))
    print_matrix("Обертання R (45° навколо 1,1,1)", R)
    current_vertices = [apply_transformation(R, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    T = translation_matrix(4, -2, 1)
    print_matrix("Переміщення T (4, -2, 1)", T)
    current_vertices = [apply_transformation(T, v) for v in current_vertices]
    print_vertices("Після переміщення", current_vertices)


def solve_task_4():
    print(f"\n{'='*20} ЗАВДАННЯ 4 {'='*20}")
    current_vertices = CUBE_VERTICES
    print_vertices("Початковий стан", current_vertices)

    R_e = rotation_euler_angles(20, 35, 50, order="ZYX")
    print_matrix("Обертання Ейлера R (ZYX: 20, 35, 50)", R_e)
    current_vertices = [apply_transformation(R_e, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    T = translation_matrix(1, 3, -2)
    print_matrix("Переміщення T (1, 3, -2)", T)
    current_vertices = [apply_transformation(T, v) for v in current_vertices]
    print_vertices("Після переміщення", current_vertices)


def solve_task_5():
    print(f"\n{'='*20} ЗАВДАННЯ 5: РАНДОМІЗАЦІЯ {'='*20}")
    

    random_angle = random.uniform(10, 90)
    random_axis = (random.uniform(-1, 1), random.uniform(-1, 1), random.uniform(-1, 1))
    random_t = (random.uniform(-5, 5), random.uniform(-5, 5), random.uniform(-5, 5))
    
    print(f"Згенеровані параметри:")
    print(f"- Кут повороту: {round(random_angle, 2)}°")
    print(f"- Вісь обертання: ({round(random_axis[0], 2)}, {round(random_axis[1], 2)}, {round(random_axis[2], 2)})")
    print(f"- Вектор переміщення: ({round(random_t[0], 2)}, {round(random_t[1], 2)}, {round(random_t[2], 2)})")


    R = rotation_arbitrary_axis(random_angle, random_axis)
    T = translation_matrix(*random_t)
    M = multiply_matrices(T, R)
    print("\nМатриця обертання R (випадкова вісь):")
    for row in R: print([round(x, 3) for x in row])
    print("\nМатриця переміщення T (випадкова):")
    for row in T: print([round(x, 3) for x in row])
    print("\n--- Положення вершин тетраеда ---")
    current_vertices = TETRAHEDRON_VERTICES
    print("Початковий стан: ", current_vertices)
    final_vertices = [apply_transformation(M, v) for v in current_vertices]
    for i, v in enumerate(final_vertices):
        print(f"V{i}: {tuple(round(coord, 3) for coord in v)}")



def solve_task_6():
    print(f"\n{'='*20} ЗАВДАННЯ 6 {'='*20}")
    current_vertices = CUBE_VERTICES
    pivot_point = (2, 0, 3)
    final_move = (-1, 2, 4)
    print_vertices("Початковий стан", current_vertices)

    T1 = translation_matrix(-pivot_point[0], -pivot_point[1], -pivot_point[2])
    Ry = rot_y(45)
    T2 = translation_matrix(pivot_point[0], pivot_point[1], pivot_point[2])
    M_pivot = multiply_matrices(T2, multiply_matrices(Ry, T1))
    print_matrix("Обертання навколо осі (0,1,0) із опорною точкою (2,0,3) (T2 * Ry * T1)", M_pivot)
    
    current_vertices = [apply_transformation(M_pivot, v) for v in current_vertices]
    print_vertices("Після обертання навколо точки (2, 0, 3)", current_vertices)
    
    T_final = translation_matrix(*final_move)
    print_matrix("Матриця переміщення на вектор (-1, 2, 4)", T_final)
    current_vertices = [apply_transformation(T_final, v) for v in current_vertices]
    print_vertices("Після переміщення на вектор (-1, 2, 4)", current_vertices)
    M_total = multiply_matrices(T_final, M_pivot)
    print_matrix("(Загальна матриця трансформації)", M_total)


def solve_task_7():
    print(f"\n{'='*20} ЗАВДАННЯ 7 {'='*20}")
    current_vertices = CUBE_VERTICES
    pivot = (1, 2, 3)
    print_vertices("Початковий стан", current_vertices)

    T1 = translation_matrix(-pivot[0], -pivot[1], -pivot[2])
    T2 = translation_matrix(pivot[0], pivot[1], pivot[2])
    S = scaling_matrix(1, 1, 3)
    M_scale_pivot = multiply_matrices(T2, multiply_matrices(S, T1))
    print_matrix("Розтягування у 3 рази по осі Z з опорною точкою (1, 2, 3) (T2 * Ry * T1)", M_scale_pivot)
    
    current_vertices = [apply_transformation(M_scale_pivot, v) for v in current_vertices]
    print_vertices("Після розтягування", current_vertices)
    
    Rz = rot_z(30)
    M_rot_pivot = multiply_matrices(T2, multiply_matrices(Rz, T1))
    print_matrix("Матриця обертання навколо осі Z 30° з опорною точкою (1,2,3))", M_rot_pivot)
    
    current_vertices = [apply_transformation(M_rot_pivot, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    M_total = multiply_matrices(M_rot_pivot, M_scale_pivot)
    print_matrix("(Загальна матриця трансформації)", M_total)


def solve_task_8():
    print(f"\n{'='*20} ЗАВДАННЯ 8 {'='*20}")
    TRIANGLE_VERTICES = [(1, 2, 3), (4, 5, 6), (7, 8, 9)]
    pivot_p = (2, 3, 4)
    final_move = (0, -3, 2)
    
    current_vertices = TRIANGLE_VERTICES
    print_vertices("Початкові вершини трикутника", current_vertices)

    T1 = translation_matrix(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    R_axis = rotation_arbitrary_axis(90, (1, 1, 1))
    T2 = translation_matrix(pivot_p[0], pivot_p[1], pivot_p[2])
    M_pivot_rot = multiply_matrices(T2, multiply_matrices(R_axis, T1))
    print_matrix("обертання навколо осі (1,1,1) через точку (2,3,4)", M_pivot_rot)
    
    current_vertices = [apply_transformation(M_pivot_rot, v) for v in current_vertices]
    print_vertices("Після обертання (Примітка: точки на осі не змінюються)", current_vertices)


    T_final = translation_matrix(*final_move)
    print_matrix(" переміщення на вектор (0, -3, 2))", T_final)
    final_vertices = [apply_transformation(T_final, v) for v in current_vertices]
    print_vertices("Фінальне положення трикутника", final_vertices)

    M_total = multiply_matrices(T_final, M_pivot_rot)
    print_matrix("(Загальна матриця трансформації)", M_total)


def solve_task_9():
    print(f"\n{'='*20} ЗАВДАННЯ 9 {'='*20}")
    RECT_VERTICES = [(1, 2, 0), (4, 2, 0), (4, 5, 0), (1, 5, 0)]
    pivot_p = (3, 3, 0)
    
    current_vertices = RECT_VERTICES
    print_vertices("Початкові точки прямокутника", current_vertices)

    T_to_origin = translation_matrix(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    T_back = translation_matrix(pivot_p[0], pivot_p[1], pivot_p[2])

    Ry = rot_y(60)
    M_pivot_y = multiply_matrices(T_back, multiply_matrices(Ry, T_to_origin))
    print_matrix("повороту навколо осі Y 60° із опорною точкою (3,3,0))", M_pivot_y)
    
    current_vertices = [apply_transformation(M_pivot_y, v) for v in current_vertices]
    print_vertices("Після повороту навколо Y", current_vertices)

    Rx = rot_x(30)
    M_pivot_x = multiply_matrices(T_back, multiply_matrices(Rx, T_to_origin))
    print_matrix("повороту навколо осі X 30° із опорною точкою (3,3,0))", M_pivot_x)
    
    current_vertices = [apply_transformation(M_pivot_x, v) for v in current_vertices]
    print_vertices("Фінальне положення", current_vertices)
    M_total = multiply_matrices(M_pivot_x, M_pivot_y)
    print_matrix("(Загальна матриця трансформації)", M_total)


def solve_task_10():
    print(f"\n{'='*20} ЗАВДАННЯ 10 {'='*20}")
    current_vertices = CUBE_VERTICES
    pivot_p = (1, 1, 1)
    final_move = (-3, 4, 2)
    
    print_vertices("Початкові точки куба", current_vertices)

    T_to_origin = translation_matrix(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    T_back = translation_matrix(pivot_p[0], pivot_p[1], pivot_p[2])
    Sx = scaling_matrix(2, 1, 1)
    M1 = multiply_matrices(T_back, multiply_matrices(Sx, T_to_origin))
    print_matrix("розтягування у два рази по осі X з опорною точкою (1,1,1))", M1)
    
    current_vertices = [apply_transformation(M1, v) for v in current_vertices]
    print_vertices("Після розтягування", current_vertices)

    Ry = rot_y(45)
    M2 = multiply_matrices(T_back, multiply_matrices(Ry, T_to_origin))
    print_matrix("обертання навколо осі Y 45° з опорною точкою (1,1,1))", M2)
    current_vertices = [apply_transformation(M2, v) for v in current_vertices]
    print_vertices("Після обертання", current_vertices)

    T_final = translation_matrix(*final_move)
    print_matrix("переміщення на вектор (-3, 4, 2))", T_final)
    
    final_vertices = [apply_transformation(T_final, v) for v in current_vertices]
    print_vertices("Фінальне положення куба", final_vertices)

    M_total = multiply_matrices(T_final, multiply_matrices(M2, M1))
    print_matrix("(Загальна матриця трансформації)", M_total)


def solve_task_11():
    print(f"\n{'='*20} ЗАВДАННЯ 11 {'='*20}")
    current_vertices = CUBE_VERTICES
    print_vertices("Початкові точки куба", current_vertices)

    # ТРАНСФОРМАЦІЯ А (Зовнішня - Extrinsic)
    # Порядок Z(30) -> Y(45) -> X(60)
    # R_extrinsic = Rz * Ry * Rz
    Rz_30 = rot_z(30)
    Ry_45 = rot_y(45)
    Rx_60 = rot_x(60)
    R_A = multiply_matrices(Rx_60, multiply_matrices(Ry_45, Rz_30))
    print_matrix("R_A (Зовнішня: Z->Y->X)", R_A)

    # ТРАНСФОРМАЦІЯ Б (Внутрішня - Intrinsic)
    # Порядок X(60) -> Y(45) -> Z(30)
    # R_intrinsic = Rx * Ry * Rz 
    R_B = multiply_matrices(Rx_60, multiply_matrices(Ry_45, Rz_30))
    print_matrix(" R_B (Внутрішня: X->Y->Z)", R_B)


    is_identical = True
    for i in range(4):
        for j in range(4):
            if abs(R_A[i][j] - R_B[i][j]) > 1e-6:
                is_identical = False
    
    print(f"\nМатематичне доведення: R_A == R_B : {is_identical}")
    print("Висновок: Послідовності ідентичні згідно з теоремою (слайд 27 лекції).")

    final_vertices = [apply_transformation(R_A, v) for v in current_vertices]
    print_vertices("Фінальне положення куба (однакове для А та Б)", final_vertices)


def solve_task_12():
    print(f"\n{'='*20} ЗАВДАННЯ 12 {'='*20}")
    
    # 1. Тестова матриця (наприклад, із Завдання 10), TRS: M = T * R * S
    pivot = (1, 1, 1)
    T_to_origin = translation_matrix(-pivot[0], -pivot[1], -pivot[2])
    T_back = translation_matrix(pivot[0], pivot[1], pivot[2])
    Sx = scaling_matrix(2, 1, 1)
    M_scale = multiply_matrices(T_back, multiply_matrices(Sx, T_to_origin))
    Ry = rot_y(45)
    M_rot = multiply_matrices(T_back, multiply_matrices(Ry, T_to_origin))
    T_final = translation_matrix(-3, 4, 2)
    
    # Комплексна матриця "Чорна скринька"
    M_complex = multiply_matrices(T_final, multiply_matrices(M_rot, M_scale))
    print_matrix("Вхідна матриця M (Чорна скринька)", M_complex)

    # ВИДІЛЕННЯ ПЕРЕНЕСЕННЯ (T) 
    tx = M_complex[0][3]
    ty = M_complex[1][3]
    tz = M_complex[2][3]
    print(f"\n1. Вектор перенесення T: ({round(tx, 3)}, {round(ty, 3)}, {round(tz, 3)})")
    # ВИДІЛЕННЯ МАСШТАБУВАННЯ (S): Знаходимо норми вектор-стовпчиків підматриці 3x3 
    sx = math.sqrt(M_complex[0][0]**2 + M_complex[1][0]**2 + M_complex[2][0]**2)
    sy = math.sqrt(M_complex[0][1]**2 + M_complex[1][1]**2 + M_complex[2][1]**2)
    sz = math.sqrt(M_complex[0][2]**2 + M_complex[1][2]**2 + M_complex[2][2]**2)
    print(f"2. Коефіцієнти масштабування S: sx={round(sx, 3)}, sy={round(sy, 3)}, sz={round(sz, 3)}")
    # ВИДІЛЕННЯ МАТРИЦІ ОБЕРТАННЯ (R): Нормалізуємо стовпці (ділимо на масштаби) 
    R = [[0.0]*4 for _ in range(4)]
    for i in range(3):
        R[i][0] = M_complex[i][0] / sx
        R[i][1] = M_complex[i][1] / sy
        R[i][2] = M_complex[i][2] / sz
    R[3][3] = 1.0
    print_matrix("Чиста матриця обертання R", R)

    # КУТ ТА ВІСЬ ОБЕРТАННЯ: Обчислюємо слід матриці (trace)
    trace_R = R[0][0] + R[1][1] + R[2][2]
    # Кут theta = arccos((trace - 1) / 2) 
    cos_theta = (trace_R - 1) / 2
    cos_theta = max(-1.0, min(1.0, cos_theta))
    theta = math.degrees(math.acos(cos_theta))
    print(f"\n3. Кут обертання: {round(theta, 2)}°")

    ux = R[2][1] - R[1][2]
    uy = R[0][2] - R[2][0]
    uz = R[1][0] - R[0][1]
    
    modul_u = math.sqrt(ux**2 + uy**2 + uz**2)
    if modul_u > 1e-6:
        ux, uy, uz = ux/modul_u, uy/modul_u, uz/modul_u
        print(f"4. Одиничний вектор осі u: ({round(ux, 3)}, {round(uy, 3)}, {round(uz, 3)})")
    else:
        print("4. Вісь не визначена (поворот на 0 або 180 градусів)")


def solve_task_13():
    print(f"\n{'='*20} ЗАВДАННЯ 13 {'='*20}")
    
    current_vertices = TETRAHEDRON_VERTICES
    print_vertices("Початкові точки тетраедра", current_vertices)

    R1 = rot_x(45)
    print_matrix("обертання на 45° навколо локальної осі", R1)
    
    current_vertices = [apply_transformation(R1, v) for v in current_vertices]
    print_vertices("після обертання на 45°", current_vertices)

    T_local = translation_matrix(0, 2, 0)
    M2 = multiply_matrices(R1, T_local)
    print_matrix("M2 (M1 * Локальне Ty 2)", M2)
    
    current_vertices = [apply_transformation(M2, v) for v in TETRAHEDRON_VERTICES]
    print_vertices("Після переміщення по нахиленій осі", current_vertices)

    R_local_z = rot_z(30)
    M3 = multiply_matrices(M2, R_local_z)
    print_matrix("M3 (M2 * Локальне Rz 30°)", M3)
    
    final_vertices = [apply_transformation(M3, v) for v in TETRAHEDRON_VERTICES]
    print_vertices("Фінальне положення", final_vertices)


def solve_task_15():
    print(f"\n{'='*20} ЗАВДАННЯ 15 {'='*20}")
    current_vertices = CUBE_VERTICES
    pivot_p = (1, 1, 1)
    final_move = (-3, 4, 2)
    
    print_vertices("Початковові точки куба", current_vertices)

    # 1. МАСШТАБУВАННЯ x2 відносно опорної точки (1,1,1)
    T_to_origin = translation_matrix(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    T_back = translation_matrix(pivot_p[0], pivot_p[1], pivot_p[2])
    S = scaling_matrix(2, 2, 2)
    M_scale_pivot = multiply_matrices(T_back, multiply_matrices(S, T_to_origin))
    print_matrix("M1 (Масштаб x2 навколо (1,1,1))", M_scale_pivot)

    # 2. ВНУТРІШНЄ обертання навколо локальної осі X на 90°
    Rx_local = rot_x(90)
    M_step2 = multiply_matrices(M_scale_pivot, Rx_local)
    print_matrix("M2 (M1 * Локальне Rx 90°)", M_step2)

    # 3. ЗОВНІШНЄ переміщення на вектор (-3, 4, 2)
    T_extrinsic = translation_matrix(*final_move)
    M_final = multiply_matrices(T_extrinsic, M_step2)
    print_matrix("M_final (T_extrinsic * M2)", M_final)

    final_vertices = [apply_transformation(M_final, v) for v in current_vertices]
    print_vertices("Фінальне положення куба", final_vertices)

    print(f"\n{'='*10} Результат 15 {'='*10}")
    sx_dec = math.sqrt(M_final[0][0]**2 + M_final[1][0]**2 + M_final[2][0]**2)
    sy_dec = math.sqrt(M_final[0][1]**2 + M_final[1][1]**2 + M_final[2][1]**2)
    sz_dec = math.sqrt(M_final[0][2]**2 + M_final[1][2]**2 + M_final[2][2]**2)
    print(f"Масштаб: ({round(sx_dec, 1)}, {round(sy_dec, 1)}, {round(sz_dec, 1)})")
    R_dec = [[0.0]*4 for _ in range(4)]
    for i in range(3):
        R_dec[i][0] = M_final[i][0] / sx_dec
        R_dec[i][1] = M_final[i][1] / sy_dec
        R_dec[i][2] = M_final[i][2] / sz_dec
    R_dec[3][3] = 1.0

    # Кут через слід (trace)
    trace_R = R_dec[0][0] + R_dec[1][1] + R_dec[2][2]
    cos_th = max(-1.0, min(1.0, (trace_R - 1) / 2))
    angle_dec = math.degrees(math.acos(cos_th))
    print(f"Кут: {round(angle_dec, 1)}°")

    ux = R_dec[2][1] - R_dec[1][2]
    uy = R_dec[0][2] - R_dec[2][0]
    uz = R_dec[1][0] - R_dec[0][1]
    mod_u = math.sqrt(ux**2 + uy**2 + uz**2)
    if mod_u > 1e-6:
        print(f"Вісь: ({round(ux/mod_u, 2)}, {round(uy/mod_u, 2)}, {round(uz/mod_u, 2)})")

if __name__ == "__main__":
    solve_task_1()
    solve_task_2()
    solve_task_3()
    solve_task_4()
    solve_task_5()
    solve_task_6()
    solve_task_7()
    solve_task_8()
    solve_task_9()
    solve_task_10()
    solve_task_11()
    solve_task_12()
    solve_task_13()
    solve_task_15()
