import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4

if __name__ == '__main__':
    CUBE_1_KEY = "cube_gimbal_1"
    CUBE_2_KEY = "cube_gimbal_2"

    class GimbalLockScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_1_KEY] = Cube(self.plt_axis, alpha=0.2, color="blue")
            
            self[CUBE_2_KEY] = Cube(self.plt_axis, alpha=0.1, color="red", 
                                    line_style="--", line_width=2)

    animated_scene = GimbalLockScene(
        coordinate_rect=(-2, -2, -2, 2, 2, 2),
        title="Завдання 4: Візуалізація Gimbal Lock (β = 90°)"
    )

    # Набір 1: alpha=45, beta=90, gamma=0
    R1_x = Mat4x4.rotation_x(45, is_radians=False)
    R1_y = Mat4x4.rotation_y(90, is_radians=False)
    R1_z = Mat4x4.rotation_z(0, is_radians=False)
    # Послідовність за умовою: Rz * Ry * Rx
    M1 = R1_z * R1_y * R1_x

    # Набір 2: alpha=90, beta=90, gamma=45
    # Різниця (alpha - gamma) така сама: 90 - 45 = 45
    R2_x = Mat4x4.rotation_x(90, is_radians=False)
    R2_y = Mat4x4.rotation_y(90, is_radians=False)
    R2_z = Mat4x4.rotation_z(45, is_radians=False)
    M2 = R2_z * R2_y * R2_x

    anim1 = TrsTransformationAnimation(
        end=M1, channel=CUBE_1_KEY, frames=150, repeat=False
    )
    anim2 = TrsTransformationAnimation(
        end=M2, channel=CUBE_2_KEY, frames=150, repeat=False
    )

    animated_scene.add_animations(anim1, anim2)
    animated_scene.show()