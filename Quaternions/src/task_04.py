import numpy as np
import math

from src.engine.animation.QuaternionAnimation import QuaternionAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Quaternion import Quaternion

CUBE_KEY = "cube"

class AnimScene(AnimatedScene):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        cube = Cube(self.plt_axis, alpha=0.3, color='lightgreen')
        self[CUBE_KEY] = cube
        
        cube.show_pivot()
        cube.show_local_frame()

if __name__ == '__main__':
    animated_scene = AnimScene(
        title="Завдання 4: Декомпозиція матриці R в кватерніон q",
        image_size=(10, 10),
        coordinate_rect=(-1.5, -1.5, -1.5, 1.5, 1.5, 1.5),
    )

    R = np.array([
    [0, -1, 0],
    [1,  0, 0],
    [0,  0, 1]
    ])

    r00, r11, r22 = R[0, 0], R[1, 1], R[2, 2]
    r10, r01 = R[1, 0], R[0, 1]
    r21, r12 = R[2, 1], R[1, 2]
    r02, r20 = R[0, 2], R[2, 0]

    q0 = 0.5 * math.sqrt(1 + r00 + r11 + r22)
    q1 = (r21 - r12) / (4 * q0)
    q2 = (r02 - r20) / (4 * q0)
    q3 = (r10 - r01) / (4 * q0)
    q_from_matrix = Quaternion(q0, q1, q2, q3)

    animation_quat = QuaternionAnimation(
        end_quaternion=q_from_matrix,
        channel=CUBE_KEY,
        frames=100,
        repeat=False
    )

    animated_scene.add_animations(animation_quat)
    animated_scene.show()