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
        coordinate_rect=(-1, -1, -3, 5, 5, 3),
        title="Завдання 4: Euler ZYX(50,35,20), переміщення (1,3,-2)"
    )

    # Кути Ейлера: Z=50, Y=35, X=20
    Rz = Mat4x4.rotation_z(50, is_radians=False)
    Ry = Mat4x4.rotation_y(35, is_radians=False)
    Rx = Mat4x4.rotation_x(20, is_radians=False)
    R_final = Rx * Ry * Rz
    T_final = Mat4x4.translation(1, 3, -2)
    M_total = T_final * R_final

    animation_move = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=120,             
        repeat=False          
    )

    animated_scene.add_animations(animation_move)
    animated_scene.show()