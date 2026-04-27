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
        # R = Rz(psi) * Ry(theta) * Rx(phi)
        return multiply_matrices(rz, multiply_matrices(ry, rx))
    elif order == "ZXZ":
        # R = Rz(psi) * Rx(theta) * Rz(phi)
        rz_phi = rot_z(phi)
        return multiply_matrices(rz, multiply_matrices(rx, rz_phi))
    elif order == "ZYX":
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


def solve_task_2():
    print(f"\n{'='*20} ЗАВДАННЯ 2 {'='*20}")
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


def solve_task_3():
    print(f"\n{'='*20} ЗАВДАННЯ 3 {'='*20}")
    
    phi, theta, psi = 45, 30, 60
    
    print(f"\n--- Конвенція XYZ (зовнішні осі) для ({phi}, {theta}, {psi}) ---")
    R_xyz = rotation_euler_angles(phi, theta, psi, order="XYZ")
    print_matrix("R_XYZ", R_xyz)
    vertices_xyz = [apply_transformation(R_xyz, v) for v in CUBE_VERTICES]
    print_vertices("(після конвенції XYZ)", vertices_xyz)

    print(f"\n--- Конвенція ZXZ (зовнішні осі) для ({phi}, {theta}, {psi}) ---")
    R_zxz = rotation_euler_angles(phi, theta, psi, order="ZXZ")
    print_matrix("R_ZXZ", R_zxz)
    vertices_zxz = [apply_transformation(R_zxz, v) for v in CUBE_VERTICES]
    print_vertices("(після конвенції ZXZ)", vertices_zxz)

    print("\n--- АНАЛІЗ РЕЗУЛЬТАТІВ ---")
    v_orig = (1, 1, 1)
    v_xyz = vertices_xyz[6] 
    v_zxz = vertices_zxz[6]
    
    print(f"Початкова вершина: {v_orig}")
    print(f"Фінальна точка XYZ: {v_xyz}")
    print(f"Фінальна точка ZXZ: {v_zxz}")
    
    print("\nПояснення різниці:")
    print("1. Матриці відрізняються, оскільки порядок і вибір осей обертання є принциповими.")
    print("2. Обертання у 3D просторі некомутативні: зміна послідовності осей змінює фінальну орієнтацію.")
    print("3. Конфігурація XYZ використовує три різні осі (Тейта-Брайана), а ZXZ - дві (істинні кути Ойлера).")

def solve_task_5():
    print(f"\n{'='*20} ЗАВДАННЯ 5 {'='*20}")
    
    current_vertices = CUBE_VERTICES
    print_vertices("Початкові вершини куба", current_vertices)


    phi1, theta1, psi1 = 30, 90, 45
    print(f"\n--- Експеримент 1: Кути (X:{phi1}, Y:{theta1}, Z:{psi1}) ---")
    R1 = rotation_euler_angles(phi1, theta1, psi1, order="XYZ")
    print_matrix("R1 (30, 90, 45)", R1)
    
    vertices1 = [apply_transformation(R1, v) for v in current_vertices]
    print_vertices("після R1", vertices1)

    phi2, theta2, psi2 = 40, 90, 55
    print(f"\n--- Експеримент 2: Зміна кутів (X:+10, Z:-10) -> (X:{phi2}, Y:{theta2}, Z:{psi2}) ---")
    R2 = rotation_euler_angles(phi2, theta2, psi2, order="XYZ")
    print_matrix("R2 (40, 90, 55)", R2)
    
    vertices2 = [apply_transformation(R2, v) for v in current_vertices]
    print_vertices("після R2", vertices2)


    print("\n--- АНАЛІЗ ПРАКТИЧНОГО ЕКСПЕРИМЕНТУ ---")
    
    matrices_equal = all(
        abs(R1[i][j] - R2[i][j]) < 1e-9 
        for i in range(4) for j in range(4)
    )
    
    print(f"Матриця R1 == R2: {matrices_equal}")
    
    if matrices_equal:
        print("\nВИСНОВОК ДО ЗАВДАННЯ 5:")
        print("1. Фінальне положення куба залишилося НЕЗМІННИМ, попри зміну двох параметрів (X та Z).")
        print("2. Це практично доводить 'склеювання' осей X та Z при куті нахилу Y = 90°.")
        print("3. Система втратила один ступінь вільності: обертання навколо X тепер еквівалентне обертанню навколо Z.")

def solve_task_6():
    print(f"\n{'='*20} ЗАВДАННЯ 6 {'='*20}")
    

    start_angles = (0, 0, 0)
    end_angles = (90, 90, 90)
    view_vector = (0, 0, 1) 
    
    steps = 10
    trajectory = []

    print(f"Інтерполяція від {start_angles} до {end_angles} за {steps} кроків:")
    print(f"{'Крок':<6} | {'Кути (deg)':<20} | {'Вектор погляду (X, Y, Z)':<30}")
    print("-" * 65)

    for i in range(steps + 1):
        t = i / steps
        phi = start_angles[0] + t * (end_angles[0] - start_angles[0])
        theta = start_angles[1] + t * (end_angles[1] - start_angles[1])
        psi = start_angles[2] + t * (end_angles[2] - start_angles[2])

        R = rotation_euler_angles(phi, theta, psi, order="XYZ")
        
        v_out = apply_transformation(R, view_vector)
        trajectory.append(v_out)

        angles_str = f"({phi:>2.0f}, {theta:>2.0f}, {psi:>2.0f})"
        vec_str = f"({v_out[0]:>6.3f}, {v_out[1]:>6.3f}, {v_out[2]:>6.3f})"
        print(f"{i:<6} | {angles_str:<20} | {vec_str:<30}")

    print("\n--- АНАЛІЗ ТРАЄКТОРІЇ ---")
    print("При наближенні середнього кута (theta) до 90°, виникає Gimbal Lock")
    print("1. Хоча кути змінюються лінійно (+9° щокроку), компоненти вектора змінюються нерівномірно.")
    print("2. В зоні сингулярності (кроки 8-10) рух виглядає 'смиканим', оскільки обертання навколо")
    print("   осей X та Z вироджуються в одну фізичну вісь. ")
    print("3. Це призводить до того, що об'єкт змінює орієнтацію за траєкторією, яка не є ")
    print("   найкоротшим шляхом (дугою) у просторі.")


if __name__ == "__main__":
    solve_task_1()
    solve_task_2()
    solve_task_3()
    solve_task_5()
    solve_task_6()
    
