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

        cube = Cube(self.plt_axis, alpha=0.2, color='lightgreen')
        self[CUBE_KEY] = cube
        
        cube.show_pivot()
        cube.show_local_frame()

if __name__ == '__main__':
    animated_scene = AnimScene(
        title="Завдання 3: Конвертація з кутів Ойлера та розв'язання Gimbal Lock \n (Yaw:20°, Pitch:90°, Roll:50°)",
        image_size=(10, 10),
        coordinate_rect=(-2, -2, -2, 2, 2, 2),
    )

    alpha = math.radians(20)
    beta = math.radians(90)
    gamma = math.radians(50)

    qz = Quaternion.rotation_z(alpha)
    qy = Quaternion.rotation_y(beta)
    qx = Quaternion.rotation_x(gamma)
    q_final = qz * qy * qx

    animation_quat = QuaternionAnimation(
        end_quaternion=q_final,
        channel=CUBE_KEY,
        frames=150,
        repeat=False
    )

    animated_scene.add_animations(
        animation_quat,
    )

    animated_scene.show()