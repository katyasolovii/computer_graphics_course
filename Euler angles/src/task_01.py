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
        coordinate_rect=(-3, -2, -2, 5, 6, 5),
        title="Завдання 1: Розтяг(2, 0.5, 1), Euler XYZ(30,45,60), переміщення(-3,2,5)"
    )


    S_final = Mat4x4.scale(2, 0.5, 1)
    Rx = Mat4x4.rotation_x(30, is_radians=False)
    Ry = Mat4x4.rotation_y(45, is_radians=False)
    Rz = Mat4x4.rotation_z(60, is_radians=False)
    R_final = Rz * Ry * Rx
    T_final = Mat4x4.translation(-3, 2, 5)
    M_total = T_final * R_final * S_final


    animation_move = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=150,        
        repeat=False          
    )

    animated_scene.add_animations(animation_move)
    animated_scene.show()