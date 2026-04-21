import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4


if __name__ == '__main__':
    CUBE_START_KEY = "cube_start"
    CUBE_ANIMATED_KEY = "cube_animated"

    class CubeScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            cube_start = Cube(self.plt_axis, alpha=0.1, color="blue")
            self[CUBE_START_KEY] = cube_start

            cube_animated = Cube(self.plt_axis,
                                 alpha=0.1, color="grey",
                                 line_width=0.8, line_style="-.")
            self[CUBE_ANIMATED_KEY] = cube_animated


    animated_scene = CubeScene(
        coordinate_rect=(-1, -2, -1, 6, 5, 6),
        title="Завдання 1: Оберення на 45° навколо (1,1,0) та переміщення на (2,-1,3)"
    )

    axis_v = Vec4(1, 1, 0)
    angle_psi = 45
    t_vector = Vec4(2, -1, 3)

    phi = math.degrees(math.atan2(axis_v.x, axis_v.z))
    v_z_prime = math.sqrt(axis_v.x**2 + axis_v.z**2)
    theta = math.degrees(math.atan2(axis_v.y, v_z_prime))
    # Ry^-1 * Rx^-1 * Rz * Rx * Ry 
    Ry = Mat4x4.rotation_y(-phi, is_radians=False)
    Rx = Mat4x4.rotation_x(theta, is_radians=False)
    Rz = Mat4x4.rotation_z(angle_psi, is_radians=False)
    # Транспонована матриця повороту = обернена матриця
    R_final = Ry.transpose() * Rx.transpose() * Rz * Rx * Ry
    T_final = Mat4x4.translation(*t_vector.xyz)
    # TRS: M = T * R * S 
    M_total = T_final * R_final


    animation_move = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=120,             
        repeat=False          
    )

    animated_scene.add_animations(animation_move)
    animated_scene.show()