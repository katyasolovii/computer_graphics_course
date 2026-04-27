import math

def extract_euler_gimbal_lock(R):
    theta = math.asin(R[0][2])
    sin_theta = R[0][2]
    
    if abs(sin_theta) > 0.999999: 
        # Випадок Gimbal Lock: осі X та Z збігаються 
        # Нехай psi = 0, тоді phi = atan2(r10, r11)
        psi = 0.0
        phi = math.atan2(R[1][0], R[1][1])
    else:
        phi = math.atan2(-R[1][2], R[2][2])
        psi = math.atan2(-R[0][1], R[0][0])
        
    return math.degrees(phi), math.degrees(theta), math.degrees(psi)

test_matrix = [
    [0.0,    0.0,   1.0, 0.0],
    [0.707,  0.707, 0.0, 0.0],
    [-0.707, 0.707, 0.0, 0.0],
    [0.0,    0.0,   0.0, 1.0]
]

angles = extract_euler_gimbal_lock(test_matrix)
if angles:
    p, t, ps = angles
    print(f"Відновлені кути: Roll(phi)={p:.1f}°, Pitch(theta)={t:.1f}°, Yaw(psi)={ps:.1f}°")