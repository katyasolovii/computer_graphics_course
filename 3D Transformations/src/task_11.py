import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4

    
if __name__ == '__main__':
    CUBE_START_KEY = "cube_start"
    CUBE_A_KEY = "cube_extrinsic"  
    CUBE_B_KEY = "cube_intrinsic"

    class RotationComparisonScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_START_KEY] = Cube(self.plt_axis, alpha=0.05, color="blue")
            self[CUBE_A_KEY] = Cube(self.plt_axis, alpha=0.2, color="green", 
                                    line_width=1.5, line_style="-")
            self[CUBE_B_KEY] = Cube(self.plt_axis, alpha=0.2, color="orange", 
                                    line_width=1.0, line_style="--")

    animated_scene = RotationComparisonScene(
        coordinate_rect=(-1, -1, -1, 2, 2, 2),
        title="Завдання 11: Порівняння зовнішніх (зелений) та внутрішніх (помаранчевий) обертань"
    )

    Rz_30 = Mat4x4.rotation_z(30, is_radians=False)
    Ry_45 = Mat4x4.rotation_y(45, is_radians=False)
    Rx_60 = Mat4x4.rotation_x(60, is_radians=False)
    M_A = Rx_60 * Ry_45 * Rz_30
    M_B = Rx_60 * Ry_45 * Rz_30

    anim_A = TrsTransformationAnimation(
        end=M_A,            
        channel=CUBE_A_KEY,
        frames=150,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )
    anim_B = TrsTransformationAnimation(
        end=M_B,            
        channel=CUBE_B_KEY,
        frames=150,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )

    animated_scene.add_animations(anim_A, anim_B)
    animated_scene.show()
